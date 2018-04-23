//
// Created by erik on 2018-04-23.
//

#ifndef MANEUVERING_MANEUVERING_H
#define MANEUVERING_MANEUVERING_H

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



/** OD4 Session *****************************/
    std::shared_ptr<cluon::OD4Session> od4PwmOds;

#endif //MANEUVERING_MANEUVERING_H
