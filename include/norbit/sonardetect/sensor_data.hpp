#ifndef NORBIT_SENSOR_DATA
#define NORBIT_SENSOR_DATA

#include <chrono>
#include <vector>

#define SONAR_N_SAMPLES 512

// 1/78125
#define SONAR_SAMPLING_PERIOD_SEC 0.0000128

namespace norbit
{
    using Timestamp = std::chrono::time_point<std::chrono::steady_clock>;

    template <typename T>
    struct Timestamped
    {
        Timestamp timestamp;
        T data;
    };

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

    struct SonarData: public Timestamped<std::vector<SonarMeasurePoint>>
    {
        SonarData(const Timestamp& timestamp, std::vector<SonarMeasurePoint>&& measurePoints):
            Timestamped<std::vector<SonarMeasurePoint>>{timestamp, std::move(measurePoints)}
        {}

        SonarData() = default;

        SonarData(SonarData&&) = default;

        SonarData(const SonarData&) = delete;

        SonarData& operator=(SonarData&&) = default;
    };

    using SpeedOfSound = float;

    struct GNSSData
    {
        double roll, pitch, heading;
        double latitude, longitude, altitude;
    };
}
#endif
