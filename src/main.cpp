#include "colors.hpp"
#include "write.hpp"
#include "errors.hpp"

int main(int argc, char** argv){
    try {
    }
    catch (const std::exception& error){
        Cycle::writeln("{}{}", Cycle::RED_COLOR, error.what());
        Cycle::exit_failure();
    }
    Cycle::exit_success();
}
