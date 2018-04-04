#define CATCH_CONFIG_MAIN  
// This tells Catch to provide a main() – only do this once per test-runner

#include "catch.hpp"
#include "yawDegrees.hpp"

TEST_CASE("Test yawDegrees 1.") {
    yawDegrees yd;
    float result =  57.29578;		
    REQUIRE(result == (yd.getSteeringAngle(1.0)));
}
