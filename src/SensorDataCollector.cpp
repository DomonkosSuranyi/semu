#include <norbit/sonardetect/SensorDataCollector.hpp>
#include <stdexcept>

using namespace norbit;
using State = DetectionPointBatch::State;

void SensorDataCollector::sonarDataUpdate(Timestamped<SonarData> sonarData)
{
    if(openBatch)
    {
        openBatch->registerSonarData(std::move(sonarData));
    }
}

void SensorDataCollector::gnssUpdate(const Timestamped<GNSSData>& gnss)
{
    if(openBatch)
    {
        openBatch->registerGNSS(gnss);
        if(waitForGNSSBatch)
        {
            if(State::COMPLETE == waitForGNSSBatch->registerGNSS(gnss))
            {
                finalizeBatch(std::move(waitForGNSSBatch));
                // replace to a nullptr to avert undefined behavior
                waitForGNSSBatch = std::unique_ptr<DetectionPointBatch>();
            }
        }
    }
}

void SensorDataCollector::speedOfSoundUpdate(const Timestamped<SpeedOfSound>& speedOfSound)
{
    if(openBatch)
    {
        switch(openBatch->registerSpeedOfSound(speedOfSound))
        {
        case State::COMPLETE:
            finalizeBatch(std::move(openBatch));
            break;

        case State::GNSS_MISSING:
            if(waitForGNSSBatch)
            {
                // Should not occur
                throw std::logic_error("Multiple batches would waiting for GNSS");
            }
            waitForGNSSBatch = std::move(openBatch);
            break;

        case State::SPEED_OF_SOUND_MISSING:
            // Should not occur
            throw std::logic_error("Illegal batch state after apply speed of sound");
            break;
        }
    }

    // open a new batch
    openBatch = std::make_unique<DetectionPointBatch>(speedOfSound);
}

void SensorDataCollector::finalizeBatch(std::unique_ptr<DetectionPointBatch>&& batch)
{
    // TODO evaluate and export
}
