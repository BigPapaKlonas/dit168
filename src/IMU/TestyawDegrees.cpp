#define CATCH_CONFIG_MAIN  
// This tells Catch to provide a main() – only do this once per test-runner

#include "catch.hpp"
#include "yawDegrees.hpp"

TEST_CASE("Test yawDegrees 1.") {
    yawDegrees yd;
    double result =  -81.35;		
    REQUIRE(result == (yd.getSteeringAngle(1.05, -0.17, 9.8)));
}
