#define CATCH_CONFIG_MAIN  
// This tells Catch to provide a main() – only do this once per test-runner

#include "catch.hpp"
#include "../src/IMU/acceleration.hpp"

TEST_CASE("Test 1. distance traveled") {
    acceleration a;
    float result = 25; //distance in meter		
    REQUIRE(result == (a.getDistanceTraveled(2.0, 5.0, 0.0)));
}

TEST_CASE("Test 2. speed") {
    acceleration a;		
    float result = 0.0075; // m/s		
    REQUIRE(result == (a.getSpeed(0.75, 0.0)));
}

TEST_CASE("Test 3. acceleration") {
    acceleration a;		
    double result = 1.06; // m/s²		
    REQUIRE(result == floor(a.getAcceleration(1.05, -0.17)*100)/100);
}



