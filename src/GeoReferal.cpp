#include <norbit/sonardetect/GeoReferal.hpp>
#include <stdexcept>

using namespace norbit;
using State = DetectionPointBatch::State;

void GeoReferal::sonarDataUpdate(const Timestamped<SonarData>& sonarData)
{
    if(openBatch)
    {
        openBatch->registerSonarData(sonarData);
    }
}

void GeoReferal::gnssUpdate(const Timestamped<GNSSData>& gnss)
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

void GeoReferal::speedOfSoundUpdate(const Timestamped<SpeedOfSound>& speedOfSound)
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

void GeoReferal::finalizeBatch(std::unique_ptr<DetectionPointBatch>&& batch)
{
    // TODO evaluate and export
}
