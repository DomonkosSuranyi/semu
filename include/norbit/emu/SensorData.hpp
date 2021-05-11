#ifndef NORBIT_SENSOR_DATA
#define NORBIT_SENSOR_DATA

#include <chrono>
#include <array>

#define SONAR_N_SAMPLES 512

namespace norbit
{
    struct DetectionPoint
    {
        float angle;
        int sampleIndex;
    };

    struct SonarData
    {
        std::chrono::microseconds timestamp;
        std::array<DetectionPoint, SONAR_N_SAMPLES> detectionPoints;
    };
}
#endif
