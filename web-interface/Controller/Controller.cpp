#ifdef __EMSCRIPTEN__
#include <emscripten.h>
    #include <emscripten/bind.h>
    using namespace emscripten;
#endif

#include <cstdint>
#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>

#include "cluon-complete.hpp"
#include "build/messages.hpp"


void forward(void);

void stop(void);

void left(void);

void right(void);

void setSpeed(float requestedSpeed);

void setAngle(float requestedAngle);

float speed = 0.2;
float angle = 0.2;;
opendlv::proxy::GroundSteeringReading msgSteering;
opendlv::proxy::PedalPositionReading msgPedal;


static cluon::OD4Session od4(111, [](cluon::data::Envelope &&envelope) noexcept {});

void forward() {
    std::cout << "Forward" << std::endl;
    msgSteering.steeringAngle(0.0);
    od4.send(msgSteering);
    msgPedal.percent(speed);
    od4.send(msgPedal);
}

void stop() {
    std::cout << "Stop" << std::endl;
    msgSteering.steeringAngle(0.0);
    od4.send(msgSteering);
    msgPedal.percent(0.0);
    od4.send(msgPedal);
}

void left() {
    std::cout << "Left" << std::endl;
    msgSteering.steeringAngle(angle);
    od4.send(msgSteering);
    msgPedal.percent(speed);
    od4.send(msgPedal);
}

void right() {
    std::cout << "Right" << std::endl;
    msgSteering.steeringAngle(-angle);
    od4.send(msgSteering);
    msgPedal.percent(speed);
    od4.send(msgPedal);
}

void setAngle(float requestedAngle) {
    angle = requestedAngle;
}

void setSpeed(float requestedSpeed) {
    speed = requestedSpeed;
}

int main(int argc, char **argv) {

    // TEST MAIN

    forward();

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    stop();

    setSpeed(0.5);
    forward();

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    stop();

    right();

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    stop();

    left();

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    stop();

    /* ACTUAL MAIN
    (void) argc;
    (void) argv;
    return 0;

     **/
}


#ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS(controller) {

        function("forward", &forward, allow_raw_pointers());
        function("left", &left, allow_raw_pointers());
        function("right", &right, allow_raw_pointers());
        function("stop", &stop, allow_raw_pointers());
        function("setSpeed", &setSpeed, allow_raw_pointers());
        function("setAngle", &setAngle, allow_raw_pointers());

}
#endif