#include <chrono>
#include <iostream>
#include <cluon/UDPReceiver.hpp>
#include <cluon/FromProtoVisitor.hpp>
#include "NumModifier.hpp"

#include "cluon/UDPSender.hpp"


int main(int /*argc*/, char** /*argv*/) {

    /* Instantiate NumModifier */
    NumModifier NM;

    /* Variables used for random number generation */
    int randomValue;
    int doubled;

    /* Constructs a UPDSender object and establishes UDP connection to specific IP and port (local host in this case) */
    cluon::UDPSender sender{"127.0.0.1", 1234};

    /* Randomizes seed for randomization */
    srand(time(0));

    /* Allows for use of time literals such as '1s' */
    using namespace std::literals::chrono_literals;


    while(1){

        /* Assigns a random value */
        randomValue = rand() % 100;

        /* Calls function doubleIt from NumModifier and assigns the return value to doubled */
        doubled = NM.doubleIt(randomValue);

        /* Sends the following string */
        sender.send(std::to_string(randomValue) + " * 2 = " + std::to_string(doubled));

        /* Sleeps for one second */
        std::this_thread::sleep_for(1s);

    }
    return 0;
}
