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


class Example {
public:
    // Default Constructor
    Example() {

        /*
        cluon::OD4Session od4(111, [](cluon::data::Envelope &&envelope) noexcept {});

        if (od4.isRunning() == 0) {
            std::cout << "ERROR: No OD4 is running!" << std::endl;
            exit(0);
        }
         */
    }

    void forward(void);

    void stop(void);

    void left(void);

    void right(void);

    void setSpeed(float requestedSpeed);

    void setAngle(float requestedAngle);

private:
    float speed;
    float angle;
    opendlv::proxy::GroundSteeringReading msgSteering;
    opendlv::proxy::PedalPositionReading msgPedal;
};

static cluon::OD4Session od4(111, [](cluon::data::Envelope &&envelope) noexcept {});

void Example::forward() {
    std::cout << "Forward" << std::endl;
    msgSteering.steeringAngle(0.0);
    od4.send(msgSteering);
    msgPedal.percent(speed);
    od4.send(msgPedal);
}

void Example::stop() {
    std::cout << "Stop" << std::endl;
    msgSteering.steeringAngle(0.0);
    od4.send(msgSteering);
    msgPedal.percent(0.0);
    od4.send(msgPedal);
}

void Example::left() {
    std::cout << "Left" << std::endl;
    msgSteering.steeringAngle(angle);
    od4.send(msgSteering);
    msgPedal.percent(speed);
    od4.send(msgPedal);
}

void Example::right() {
    std::cout << "Right" << std::endl;
    msgSteering.steeringAngle(-angle);
    od4.send(msgSteering);
    msgPedal.percent(speed);
    od4.send(msgPedal);
}

void Example::setAngle(float requestedAngle) {
    angle = requestedAngle;
}

void Example::setSpeed(float requestedSpeed) {
    speed = requestedSpeed;
}

int main(int argc, char **argv) {

    // TEST MAIN

    Example e;

    e.setSpeed(0.16);
    e.forward();
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    e.stop();

    e.setSpeed(0.5);
    e.forward();
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    e.right();
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    e.left();
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    /* ACTUAL MAIN
    (void) argc;
    (void) argv;
    return 0;

     **/
}


#ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS(libcluon) {
    function("forward", &forward);
    function("left", &left);
    function("right", &right);
    function("stop", &stop);
    function("setSpeed", &setSpeed);
    function("setAngle", &setAngle);

}
#endif