#define CATCH_CONFIG_MAIN  
// This tells Catch to provide a main() – only do this once per test-runner

#include "catch.hpp"
#include "yawDegrees.hpp"

TEST_CASE("Test yawDegrees 1.") {
    yawDegrees yd;
    double result =  -81.35;		
    REQUIRE(result == (yd.getSteeringAngle(1.05, -0.17)));
}

TEST_CASE("Test heading ver 2.") {
    yawDegrees yd;
    double result =  -81.35;		
    REQUIRE(result == (yd.getHeading(1.05, 0.17, 0.1, 0.0, 0.1)));
}
