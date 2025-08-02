#include "Common/write.hpp"
#include "Common/errors.hpp"
#include "type.hpp"
#include "value.hpp"

// NOTE: No pointer shall be nullptr

using namespace Cycle;

int main(int argc, char** argv){
    try {
        const Type* field_a = NumericType::get_i32();
        const Type* field_b = NumericType::get_float();
        const Type* field_c = NumericType::get_double();
        const Type* struct_type = UnnamedStructType::get_struct({field_a, field_b, field_c});
        const auto& struct_value_set = struct_type->create_default_value_set();

        writeln("Struct contents: {}", struct_value_set->print_value_set());
    }
    catch (const std::exception& error){
        writeln("{}", error.what());
        exit_failure();
    }
    exit_success();
}
