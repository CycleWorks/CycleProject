#include "write.hpp"
#include "errors.hpp"

// NOTE: No pointer pointer shall be nullptr

int main(int argc, char** argv){
    try {
    }
    catch (const std::exception& error){
        Cycle::writeln("{}", error.what());
        Cycle::exit_failure();
    }
    Cycle::exit_success();
}
