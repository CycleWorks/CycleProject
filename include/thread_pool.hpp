#pragma once

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>

namespace Cycle {
    struct ThreadPool {
        ThreadPool(const ThreadPool&) = delete;
        ThreadPool& operator=(const ThreadPool&) = delete;
        ThreadPool(ThreadPool&&) = delete;
        ThreadPool& operator=(ThreadPool&&) = delete;

        ThreadPool(std::size_t thread_count);
        ~ThreadPool();

        std::size_t queue_size() const;
        bool is_idle() const;
        void enqueue(std::function<void()> task);
        void stop();
        void wait_until_idle_or_throw();
    private:
        mutable std::mutex _work_mutex;
        std::condition_variable _work_condition;
        std::atomic<bool> _stop = false;
        std::vector<std::thread> _workers;
        std::queue<std::function<void()>> _tasks;
        std::atomic<size_t> _active_tasks = 0;

        std::mutex _wait_mutex;
        std::condition_variable _wait_condition;
        std::shared_ptr<std::promise<void>> _abort_promise;
        std::future<void> _abort_future;
        std::atomic<bool> _error_set = false;
    };
}
