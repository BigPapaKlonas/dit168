#define CATCH_CONFIG_MAIN  
// This tells Catch to provide a main() – only do this once per test-runner

#include "catch.hpp"
#include "acceleration.hpp"

TEST_CASE("Test acceleration 1.") {
    acceleration a;
    float result =  12.0; //distance in meter		
    REQUIRE(result == (a.getDistanceTraveled(0.75)));
}

TEST_CASE("Test speed 2.") {
    acceleration a;		
    float result = 3.0; // m/s		
    REQUIRE(result == (a.getSpeed(0.75, 0.0)));
}
