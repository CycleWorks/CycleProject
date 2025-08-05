#include <catch2/catch.hpp>
#include "Common/write.hpp"
#include "value.hpp"

TEMPLATE_TEST_CASE("NumberValueSet verification", "[NumberValueSet]",
    int8_t, int16_t, int32_t, int64_t, uint8_t, uint16_t, uint32_t, uint64_t, float, double)
{
    using ValueSetType = Cycle::NumberValueSet<TestType>;
    std::vector<TestType> test_signs;

    if constexpr (Cycle::IsUnsignedInteger<TestType>){
        test_signs = {1};
    } else {
        test_signs = {1, -1};
    }

    for (const TestType& sign : test_signs){
        ValueSetType set;

        set.add_value(5 * sign);
        set.add_value(10 * sign);
        set.add_value(15 * sign);
        set.add_value(20 * sign);
        set.add_value(25 * sign);

        set.add_value(11 * sign);
        set.add_value(12 * sign);
        set.add_value(16 * sign);
        set.add_value(17 * sign);

        REQUIRE(set.contains(5 * sign));
        REQUIRE(set.contains(10 * sign));
        REQUIRE(set.contains(15 * sign));
        REQUIRE(set.contains(20 * sign));
        REQUIRE(set.contains(25 * sign));
        REQUIRE(set.contains(11 * sign));
        REQUIRE(set.contains(12 * sign));
        REQUIRE(set.contains(16 * sign));
        REQUIRE(set.contains(17 * sign));
        REQUIRE_FALSE(set.contains(19 * sign));
        REQUIRE_FALSE(set.contains(21 * sign));
        REQUIRE_FALSE(set.contains(22 * sign));
        REQUIRE_FALSE(set.contains(23 * sign));
        REQUIRE_FALSE(set.contains(24 * sign));
        REQUIRE_FALSE(set.contains(26 * sign));

        set.add_range(typename ValueSetType::Range(40 * sign, 120 * sign, 20 * sign));
        set.add_range(typename ValueSetType::Range(21 * sign, 70 * sign, 7 * sign));

        REQUIRE(set.contains(40 * sign));
        REQUIRE(set.contains(60 * sign));
        REQUIRE(set.contains(80 * sign));
        REQUIRE(set.contains(100 * sign));
        REQUIRE(set.contains(120 * sign));
        REQUIRE(set.contains(21 * sign));
        REQUIRE(set.contains(28 * sign));
        REQUIRE(set.contains(35 * sign));
        REQUIRE(set.contains(42 * sign));
        REQUIRE(set.contains(49 * sign));
        REQUIRE(set.contains(56 * sign));
        REQUIRE(set.contains(63 * sign));
        REQUIRE(set.contains(70 * sign));

        REQUIRE_FALSE(set.contains(39 * sign));
        REQUIRE_FALSE(set.contains(41 * sign));
        REQUIRE_FALSE(set.contains(119 * sign));
        REQUIRE_FALSE(set.contains(121 * sign));

        REQUIRE_FALSE(set.contains(0));
        REQUIRE_FALSE(set.contains(Cycle::NumericWrapper<TestType>::min()));
        REQUIRE_FALSE(set.contains(Cycle::NumericWrapper<TestType>::max()));

        Cycle::writeln("Test NumberValueSet: {}", set.print_value_set());
    }
}
