#include "write.hpp"
#include "errors.hpp"

#include "LLVM.hpp"

int main(int argc, char** argv){
    try {
    }
    catch (const std::exception& error){
        Cycle::writeln("{}", error.what());
        Cycle::exit_failure();
    }
    Cycle::exit_success();
}
