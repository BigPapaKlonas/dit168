#define CATCH_CONFIG_MAIN  
// This tells Catch to provide a main() – only do this once per test-runner

#include "catch.hpp"
#include "../src/IMU/yawDegrees.hpp"

TEST_CASE("Test yawDegrees given I/O from roboticscape.") {
    yawDegrees yd;
    double result =  -80.8;		
    REQUIRE(result == (yd.getSteeringAngle(1.05, -0.17)));
}

TEST_CASE("Test yawDegrees 1.") {
    yawDegrees yd;
    double result =  17.41;		
    REQUIRE(result == (yd.getSteeringAngle(0.16, 0.51)));
}

TEST_CASE("Test heading ver 2.") {
    yawDegrees yd;
    double result =  15.5;		
    REQUIRE(result == (yd.getHeading(0.16, 0.51, 0.27, 1.0)));
}
