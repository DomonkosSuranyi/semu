#ifndef NORBIT_SENSOR_DATA
#define NORBIT_SENSOR_DATA

#include <chrono>
#include <vector>

#define SONAR_N_SAMPLES 512

namespace norbit
{
    struct DetectionPoint
    {
        float angle;
        int sampleIndex;
    };

    inline bool operator==(const DetectionPoint& one, const DetectionPoint& other)
    {
        return one.angle == other.angle
            && one.sampleIndex == other.sampleIndex;
    }

    struct SonarData
    {
        std::chrono::microseconds timestamp;
        std::vector<DetectionPoint> detectionPoints;

        SonarData(const std::chrono::microseconds& timestamp, const std::vector<DetectionPoint>& detectionPoints):
            timestamp(timestamp),
            detectionPoints(detectionPoints) {}

        SonarData() = default;

        SonarData(SonarData&& other) = default;

        SonarData(const SonarData& other) = default;

        SonarData& operator=(SonarData&& other) = default;
    };
}
#endif
