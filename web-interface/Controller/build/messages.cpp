#include <messages.hpp>

/*
 * THIS IS AN AUTO-GENERATED FILE. DO NOT MODIFY AS CHANGES MIGHT BE OVERWRITTEN!
 */
namespace opendlv { namespace proxy {

int32_t GroundSteeringReading::ID() {
    return 1045;
}

const std::string GroundSteeringReading::ShortName() {
    return "GroundSteeringReading";
}
const std::string GroundSteeringReading::LongName() {
    return "opendlv.proxy.GroundSteeringReading";
}

GroundSteeringReading& GroundSteeringReading::steeringAngle(const float &v) noexcept {
    m_steeringAngle = v;
    return *this;
}
float GroundSteeringReading::steeringAngle() const noexcept {
    return m_steeringAngle;
}

}}

#include <messages.hpp>

/*
 * THIS IS AN AUTO-GENERATED FILE. DO NOT MODIFY AS CHANGES MIGHT BE OVERWRITTEN!
 */
namespace opendlv { namespace proxy {

int32_t PedalPositionReading::ID() {
    return 1041;
}

const std::string PedalPositionReading::ShortName() {
    return "PedalPositionReading";
}
const std::string PedalPositionReading::LongName() {
    return "opendlv.proxy.PedalPositionReading";
}

PedalPositionReading& PedalPositionReading::percent(const float &v) noexcept {
    m_percent = v;
    return *this;
}
float PedalPositionReading::percent() const noexcept {
    return m_percent;
}

}}

