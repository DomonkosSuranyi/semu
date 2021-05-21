#ifndef NORBIT_SENSOR_DATA
#define NORBIT_SENSOR_DATA

#include <chrono>
#include <vector>

#define SONAR_N_SAMPLES 512

// 1/78125
#define SONAR_SAMPLING_PERIOD_SEC 0.0000128

namespace norbit
{
    struct SonarMeasurePoint
    {
        float angle;
        int sampleIndex;
    };

    inline bool operator==(const SonarMeasurePoint& one, const SonarMeasurePoint& other)
    {
        return one.angle == other.angle
            && one.sampleIndex == other.sampleIndex;
    }

    struct SonarData
    {
        std::chrono::microseconds timestamp;
        std::vector<SonarMeasurePoint> measurePoints;

        SonarData(const std::chrono::microseconds& timestamp, const std::vector<SonarMeasurePoint>& measurePoints):
            timestamp(timestamp),
            measurePoints(measurePoints) {}

        SonarData() = default;

        SonarData(SonarData&& other) = default;

        SonarData(const SonarData& other) = default;

        SonarData& operator=(SonarData&& other) = default;
    };

    using SpeedOfSound = float;

    struct GNSSData
    {
        double roll, pitch, heading;
        double latitude, longitude, altitude;
    };
}
#endif
