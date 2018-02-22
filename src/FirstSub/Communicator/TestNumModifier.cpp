/*
* Vera requires this "Copyright" notice
* This tells Catch to provide a main() –
* only do this once per test-runner.
*/
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "NumModifier.hpp"

TEST_CASE("Test NumModifier.isEven() 1")
{
    NumModifier NM;
    REQUIRE(NM.isEven(2));
}

TEST_CASE("Test NumModifier.isEven() 2")
{
    NumModifier NM;
    REQUIRE_FALSE(NM.isEven(3));
}
