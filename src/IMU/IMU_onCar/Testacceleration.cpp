#define CATCH_CONFIG_MAIN  
// This tells Catch to provide a main() – only do this once per test-runner

#include "catch.hpp"
#include "acceleration.hpp"

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
    float result = 1.06; // m/s²		
    REQUIRE(result == (a.getAcceleration(1.05, -0.17)));
}

TEST_CASE("Test 4. reduce noice") {
    	acceleration a;		
    	float result = 4.5;
    	float samples[10];	
	for(int i = 0; i < 10; i++){
		samples[i] = i;	
	}		
    	REQUIRE(result == (a.reduceAccelNoice(samples)));
}

TEST_CASE("Test 5. reduce noice negative values.") {
    	acceleration a;		
    	float result = 0;
    	float samples[10];	
	for(int i = 0; i < 10; i++){
		if(i % 2 == 0){samples[i] = 0.5;}
		else{samples[i] = -0.5;}	
	}		
    	REQUIRE(result == (a.reduceAccelNoice(samples)));
}



