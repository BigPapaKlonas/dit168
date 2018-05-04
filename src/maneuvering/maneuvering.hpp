#ifndef MANEUVERING_MANEUVERING_HPP
#define MANEUVERING_MANEUVERING_HPP

#include <cluon/OD4Session.hpp>
#include <cstdint>
#include <cstdlib>
#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>
#include <queue>
#include <sys/time.h>
#include "cluon/OD4Session.hpp"
#include "cluon/Envelope.hpp"
#include "messages.hpp"

// OD4 session used for controlling the servo and engine
std::shared_ptr<cluon::OD4Session> od4PwmOds;

#endif
