#include "write.hpp"
#include "errors.hpp"

// Project rules:
// 1: No pointer shall be null (unless uninitialized)
// 2: Handle all edge cases, always assume that others will make stupid mistakes
// 3: Think about growth when coding, making it easy to implement new features later on
// 4: Never repeat the same code, use classes to make everything more complicated

int main(int argc, char** argv){
    try {
    }
    catch (const std::exception& error){
        Cycle::writeln("{}", error.what());
        Cycle::exit_failure();
    }
    Cycle::exit_success();
}
