#include "thread_pool.hpp"
#include "colors.hpp"
#include "errors.hpp"
#include "write.hpp"
#include <mutex>
#include <thread>

using namespace Cycle;

ThreadPool::ThreadPool(std::size_t thread_count):
    _abort_promise(std::make_shared<std::promise<void>>()),
    _abort_future(_abort_promise->get_future())
{
    if (thread_count == 0) thread_count = 1;
    std::vector<std::thread> tmp;
    tmp.reserve(thread_count);

    for (size_t i = 0; i < thread_count; ++i){
        tmp.emplace_back([this] {
            while (true){
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(_work_mutex);
                    _work_condition.wait(lock, [this]{
                        return _stop.load() || !_tasks.empty();
                    });
                    if (_stop.load()){
                        if (!_tasks.empty()){
                            writeln("{}[WARNING in THREAD #{}]: ThreadPool stopped before finishing all the tasks", GREY_COLOR, std::this_thread::get_id());
                        }
                        return;
                    }
                    task = std::move(_tasks.front());
                    _tasks.pop();
                    _active_tasks.fetch_add(1, std::memory_order_relaxed);
                }
                try {
                    task();
                } catch (...){
                    if (!_error_set.exchange(true)){
                        _abort_promise->set_exception(std::current_exception());
                    }
                }
                // mark done
                _work_mutex.lock();
                _active_tasks.fetch_sub(1, std::memory_order_relaxed);
                _work_mutex.unlock();
                _work_condition.notify_all();
                _wait_condition.notify_all();
            }
        });
    }
    _workers = std::move(tmp);
}

ThreadPool::~ThreadPool(){
    this->stop();
}

std::size_t ThreadPool::queue_size() const {
    std::lock_guard<std::mutex> lock(_work_mutex);
    return _tasks.size();
}

bool ThreadPool::is_idle() const {
    std::lock_guard<std::mutex> lock(_work_mutex);
    return _tasks.empty() && _active_tasks.load(std::memory_order_relaxed) == 0;
}

void ThreadPool::enqueue(std::function<void()> task){
    if (_stop.load()){
        throw InternalError("Couldn't enqueue task because ThreadPool is stopped");
        return;
    }
    _work_mutex.lock();
    _tasks.push(std::move(task));
    _work_mutex.unlock();
    _work_condition.notify_one();
}

void ThreadPool::stop(){
    bool expected = false;
    if (_stop.compare_exchange_strong(expected, true)){
        _work_condition.notify_all();
        _wait_condition.notify_all();
        for (auto& worker : _workers){
            if (worker.joinable()) worker.join();
        }
    }
}

void ThreadPool::wait_until_idle_or_throw(){
    std::unique_lock<std::mutex> lock(_wait_mutex);
    _wait_condition.wait(lock, [this]{
        return this->is_idle() || _abort_future.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
    });
    if (_abort_future.wait_for(std::chrono::seconds(0)) == std::future_status::ready){
        this->stop();
        _abort_future.get(); // Throws if exception
    }
}
