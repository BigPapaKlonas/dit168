#ifndef IMU
#define IMU

#include <iostream>
#include <stdint.h>
#include <chrono>

#include "cluon/OD4Session.hpp"
#include "cluon/Envelope.hpp"
#include "messages.hpp"
#include <math.h>
#include "acceleration.hpp"
#include "yawDegrees.hpp"

extern "C"
{
#include <rc_usefulincludes.h>
#include <roboticscape.h>
}
#endif

