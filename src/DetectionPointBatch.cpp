#include <norbit/sonardetect/DetectionPointBatch.hpp>

using namespace norbit;
using BatchState = DetectionPointBatch::State;

// Slot
DetectionPointSlot::DetectionPointSlot(
        Timestamped<SonarData> sonarData,
        const Timestamped<GNSSData>& gnssData):
    sonarData(std::move(sonarData)),
    gnssData(gnssData),
    currentState(State::GNSS_MISSING)
{}

DetectionPointSlot::State DetectionPointSlot::getState() const
{
    return currentState;
}

void DetectionPointSlot::setAfterGNSSData(const Timestamped<GNSSData>& afterGNSS)
{
    auto timeSinceFirstGNSS = sonarData.timestamp - gnssData.timestamp;
    auto timeToAfterGNSS = afterGNSS.timestamp - sonarData.timestamp;

    if(timeToAfterGNSS < timeSinceFirstGNSS)
        gnssData = afterGNSS;

    currentState = State::COMPLETE;
}

const Timestamped<SonarData>& DetectionPointSlot::getSonarData() const
{
    return sonarData;
}

const Timestamped<GNSSData>& DetectionPointSlot::getGNSSData() const
{
    return gnssData;
}

// Batch
DetectionPointBatch::DetectionPointBatch(const Timestamped<SpeedOfSound>& sosBefore):
    sosBefore(sosBefore),
    currentState(State::SPEED_OF_SOUND_MISSING)
{
    // 20Hz is the average SonarData update rate
    slots.reserve(22);
}

BatchState DetectionPointBatch::getState() const
{
    return currentState;
}

BatchState DetectionPointBatch::registerSpeedOfSound(const Timestamped<SpeedOfSound>& sosAfter)
{
    if(currentState != State::SPEED_OF_SOUND_MISSING)
    {
        return currentState;
    }

    this->sosAfter = std::optional(sosAfter);
    if(!slots.empty()
            && slots.back().getState() == DetectionPointSlot::State::GNSS_MISSING)
    {
        currentState = State::GNSS_MISSING;
    }
    else
    {
        currentState = State::COMPLETE;
    }
    return currentState;
}

BatchState DetectionPointBatch::registerGNSS(const Timestamped<GNSSData>& newGNSS)
{
    if(currentState == State::COMPLETE)
    {
        return currentState;
    }

    // The unfinished elements are at the end of the vector.
    // So iterate backwards until finding non-complete ones
    for(auto it = slots.rbegin() ;
            it != slots.rend() && it->getState() == DetectionPointSlot::State::GNSS_MISSING ;
            ++it) {
        it->setAfterGNSSData(newGNSS);
    }

    if(currentState == State::GNSS_MISSING)
    {
        currentState = State::COMPLETE;
    }
    else
    {
        lastGNSS = newGNSS;
    }


    return currentState;
}

BatchState DetectionPointBatch::registerSonarData(Timestamped<SonarData> sonarData)
{
    if(currentState == State::SPEED_OF_SOUND_MISSING)
    {
        slots.push_back(DetectionPointSlot(std::move(sonarData), lastGNSS));
    }

    return currentState;
}

const std::vector<DetectionPointSlot>& DetectionPointBatch::getSlots() const
{
    return slots;
}
