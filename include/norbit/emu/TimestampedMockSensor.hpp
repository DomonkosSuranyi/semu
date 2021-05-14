#ifndef NORBIT_TIMESTAMPED_MOCK_SENSOR
#define NORBIT_TIMESTAMPED_MOCK_SENSOR

#include "MockSensor.hpp"

namespace norbit
{
    template <typename T>
    class TimestampedMockSensor: public MockSensor<T>
    {
    public:
        TimestampedMockSensor<T>(
                const std::filesystem::path& sensorFilePath,
                const std::chrono::microseconds timeOffset):
            MockSensor<T>(sensorFilePath),
            timeOffset(timeOffset) {}

        std::optional<time_point> nextUpdateTime() override
        {
            if(MockSensor<T>::hasNext())
            {
                return time_point(MockSensor<T>::nextSensorData->timestamp + timeOffset);
            }

            return std::nullopt;
        }

    private:
        const std::chrono::microseconds timeOffset;
    };
}

#endif
