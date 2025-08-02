#include <catch2/catch.hpp>
#include "Common/utils.hpp"
#include "Common/write.hpp"
#include "value.hpp"

// TODO Add tests for uint, float and double

TEST_CASE("NumberValueSet validation (positive integer values)", "[Value][Integer][Positive]"){
    using ValueSetI32 = Cycle::NumberValueSet<int32_t>;
    ValueSetI32 set;

    set.add_value(10);
    set.add_value(20);
    set.add_value(30);
    set.add_value(40);

    set.add_value(11);
    set.add_value(13);
    set.add_value(17);
    set.add_value(19);

    set.add_range(ValueSetI32::Range(200, 300, 20));
    set.add_range(ValueSetI32::Range(150, 300, 50));
    set.add_range(ValueSetI32::Range(300, 12000, 50));

    Cycle::writeln("Printed value: {}", set.print_value_set());

    REQUIRE(set.contains(10));
    REQUIRE(set.contains(20));
    REQUIRE(set.contains(30));
    REQUIRE(set.contains(40));

    REQUIRE(set.contains(11));
    REQUIRE(set.contains(13));
    REQUIRE(set.contains(17));
    REQUIRE(set.contains(19));

    REQUIRE(set.contains(200));
    REQUIRE(set.contains(200));
    REQUIRE(set.contains(240));
    REQUIRE(set.contains(280));
    REQUIRE(set.contains(300));

    REQUIRE(set.contains(150));
    REQUIRE(set.contains(200));
    REQUIRE(set.contains(250));
    REQUIRE(set.contains(300));

    REQUIRE_FALSE(set.contains(0));
    REQUIRE_FALSE(set.contains(5));
    REQUIRE_FALSE(set.contains(15));
    REQUIRE_FALSE(set.contains(25));

    REQUIRE_FALSE(set.contains(151));
    REQUIRE_FALSE(set.contains(199));
    REQUIRE_FALSE(set.contains(249));
    REQUIRE_FALSE(set.contains(299));
    REQUIRE_FALSE(set.contains(301));
}

TEST_CASE("NumberValueSet validation (negative integer values)", "[Value][Integer][Negative]"){
    using ValueSetI32 = Cycle::NumberValueSet<int32_t>;
    ValueSetI32 set;

    set.add_value(-10);
    set.add_value(-20);
    set.add_value(-30);
    set.add_value(-40);

    set.add_value(-11);
    set.add_value(-13);
    set.add_value(-17);
    set.add_value(-19);

    set.add_range(ValueSetI32::Range(-200, -300, -20));
    set.add_range(ValueSetI32::Range(-150, -300, -50));
    set.add_range(ValueSetI32::Range(-300, -12000, -50));

    Cycle::writeln("Printed value: {}", set.print_value_set());

    REQUIRE(set.contains(-10));
    REQUIRE(set.contains(-20));
    REQUIRE(set.contains(-30));
    REQUIRE(set.contains(-40));

    REQUIRE(set.contains(-11));
    REQUIRE(set.contains(-13));
    REQUIRE(set.contains(-17));
    REQUIRE(set.contains(-19));

    REQUIRE(set.contains(-200));
    REQUIRE(set.contains(-200));
    REQUIRE(set.contains(-240));
    REQUIRE(set.contains(-280));
    REQUIRE(set.contains(-300));

    REQUIRE(set.contains(-150));
    REQUIRE(set.contains(-200));
    REQUIRE(set.contains(-250));
    REQUIRE(set.contains(-300));

    REQUIRE_FALSE(set.contains(0));
    REQUIRE_FALSE(set.contains(-5));
    REQUIRE_FALSE(set.contains(-15));
    REQUIRE_FALSE(set.contains(-25));

    REQUIRE_FALSE(set.contains(-151));
    REQUIRE_FALSE(set.contains(-199));
    REQUIRE_FALSE(set.contains(-249));
    REQUIRE_FALSE(set.contains(-299));
    REQUIRE_FALSE(set.contains(-301));
}
