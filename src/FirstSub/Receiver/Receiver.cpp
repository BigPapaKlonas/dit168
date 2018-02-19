#include <iostream>
#include <thread>

#include <cluon/UDPReceiver.hpp>


int main(int /*argc*/, char** /*argv*/) {

    /* Constructs a UPDReceiver object and establishes UDP connection to specific IP and port (local host in this case) */
    /* Lambda expression prints the data received and its origin*/

    cluon::UDPReceiver receiver("127.0.0.1", 1234,
                                [](std::string &&data, std::string && from,
                                   std::chrono::system_clock::time_point && /*timepoint**/) noexcept {
                                    std::cout << "From " << from << ": " << data << std::endl;
                                });

    /* Allows for use of time literals such as '1s' */
    using namespace std::literals::chrono_literals;

    /* Loops while the recevier is running */
    while (receiver.isRunning()) {
        /* Sleeps for one second */
        std::this_thread::sleep_for(1s);
    }
    return 0;
}