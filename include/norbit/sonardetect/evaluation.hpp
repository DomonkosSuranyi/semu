#ifndef NORBIT_EVALUATION
#define NORBIT_EVALUATION

#include "DetectionPointBatch.hpp"
#include <GeographicLib/GeoCoords.hpp>
#include <memory>

namespace norbit
{
    struct DetectionPoint
    {
        GeographicLib::GeoCoords coords;
        double altitude;
    };


    struct SonarPolarCoords
    {
        double angle, radius;
    };

    class SpeedOfSoundLine
    {
    public:
        SpeedOfSoundLine(
                const Timestamped<SpeedOfSound>& speed0,
                const Timestamped<SpeedOfSound>& speed1)
        {
            gradient = (speed1.data - speed0.data) /
                (speed1.timestamp.time_since_epoch().count() - speed0.timestamp.time_since_epoch().count());

            yIntercept = speed1.data - gradient*speed1.timestamp.time_since_epoch().count();
        }

        SpeedOfSound getSpeedAt(const Timestamp& timestamp)
        {
            return gradient*timestamp.time_since_epoch().count() + yIntercept;
        }

    private:
        float gradient, yIntercept;
    };


    inline std::vector<DetectionPoint> evaluateBatch(std::unique_ptr<DetectionPointBatch>&& batch)
    {
        std::vector<DetectionPoint> evaluatedPoints;
        evaluatedPoints.reserve(SONAR_N_SAMPLES * batch->getSlots().size());

        SpeedOfSoundLine interpolationLine(
                batch->getSpeedOfSoundBefore(),
                batch->getSpeedOfSoundAfter());
        for(const auto& slot : batch->getSlots())
        {
            SpeedOfSound speedOfSound = interpolationLine.getSpeedAt(slot.getSonarData().timestamp);

            for(const auto& measurementPoint : slot.getSonarData().data.data)
            {
                auto measurementPointCoords = SonarPolarCoords {
                    .angle = measurementPoint.angle,
                    .radius = SONAR_SAMPLING_PERIOD_SEC * measurementPoint.sampleIndex * speedOfSound / 2.0
                };

                // TODO apply INS rotation
                // TODO apply GNSS position
                auto gnss = slot.getGNSSData().data;

                // Filled with dummy data
                evaluatedPoints.push_back(DetectionPoint{
                    .coords = GeographicLib::GeoCoords(gnss.latitude, gnss.longitude),
                    .altitude = gnss.altitude - measurementPointCoords.radius
                });
            }
        }
        return evaluatedPoints;
    }
}

#endif
