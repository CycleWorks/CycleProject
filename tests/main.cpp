#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

TEST_CASE("Basic C++ logic", "[C++]"){
    REQUIRE(1 + 1 == 2);
}
