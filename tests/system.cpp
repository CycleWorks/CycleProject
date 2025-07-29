#include <catch2/catch.hpp>
#include <climits>

TEST_CASE("System validity check", "[System][C++]"){
    REQUIRE(CHAR_BIT == 8);
    REQUIRE(sizeof(float) == 4);
    REQUIRE(sizeof(double) == 8);
}
