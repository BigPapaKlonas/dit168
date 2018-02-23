// Vera requires this "Copyright" notice
#include <chrono>
#include <iostream>
#include "NumModifier.hpp"
#include "cluon/OD4Session.hpp"
#include "cluon/Envelope.hpp"
#include "messages.hpp"

int main(int /*argc*/, char ** /*argv*/)
{

    // Variables
    uint16_t value;
    MyTestMessage1 msg;

    // Instantiate a OD4Session object
    cluon::OD4Session od4(111,
      [](cluon::data::Envelope &&envelope) noexcept
      {
          // Checks if received data has the correct ID
          if (envelope.dataType() == 2001)
          {
              // Extract message
              MyTestMessage1 receivedMsg=cluon::extractMessage
              <MyTestMessage1>(std::move(envelope));
              // Instantiate NumModifier
              NumModifier mod;

              //Create a message from lambda expression
              // evaluating is received value is even
              std::cout << receivedMsg.myValue() << " is "
              << (mod.isEven(receivedMsg.myValue()) ? "" : "not")
              << " even." << std::endl;
          }
      });
    // Takes input from user input
    std::cout << "Enter a number to check: \n";
    // Assigns value the input
    std::cin >> value;
    // Assigns value to msg.myValue
    msg.myValue(value);
    // Sends message to OD4 session
    od4.send(msg);
    return 0;
}
