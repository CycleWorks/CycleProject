#include "Common/numerics.hpp"
#include "Common/write.hpp"
#include "Common/errors.hpp"
#include "HIR/type.hpp"
#include "HIR/value.hpp"
#include <limits>
#include <stdint.h>

// NOTE: No pointer shall be nullptr

using namespace Cycle::HIR;
using namespace Cycle;

int main(int argc, char** argv){
    try {
        const Type* field_a = NumericType::get_i32();
        const Type* field_b = NumericType::get_float();
        const Type* field_c = NumericType::get_double();
        const Type* struct_type = UnnamedStructType::get_struct({field_a, field_b, field_c});
        const auto& struct_value = struct_type->create_worst_value();

        const ValueSet* value_set_0 = ((StructValueSet*)struct_value->get_value_set())->get_value_at_index(0)->get_value_set();
        NumericValueSet<int32_t>* number_value_set_0 = (NumericValueSet<int32_t>*)value_set_0;

        number_value_set_0->remove_values_under(-40);

        writeln("Struct contents: {}", struct_value->get_value_set()->print_value_set());
    }
    catch (const std::exception& error){
        writeln("{}", error.what());
        exit_failure();
    }
    exit_success();
}
