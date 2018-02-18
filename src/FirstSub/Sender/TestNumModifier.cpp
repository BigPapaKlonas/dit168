#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() – only do this once per test-runner

#include "catch.hpp"
#include "NumModifier.hpp"

TEST_CASE("Test NumModifier.doubleIt()") {
    NumModifier NM;
 
    REQUIRE(NM.doubleIt(2) == 4);
}