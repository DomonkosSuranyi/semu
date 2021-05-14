#ifndef NORBIT_FIXED_RATE_MOCK_SENSOR
#define NORBIT_FIXED_RATE_MOCK_SENSOR

#include "MockSensor.hpp"

namespace norbit
{
    template <typename T>
    class FixedRateMockSensor : public MockSensor<T>
    {
    public:
            FixedRateMockSensor<T>(
                const std::filesystem::path& sensorFilePath,
                const std::chrono::microseconds updateRate):
            MockSensor<T>(sensorFilePath),
            updateRate(updateRate) {}

        std::optional<time_point> nextUpdateTime() override
        {
            if(MockSensor<T>::hasNext())
                return std::chrono::time_point_cast<std::chrono::microseconds>(
                        std::chrono::steady_clock::now() + updateRate);

            return std::nullopt;
        }

    private:
        const std::chrono::microseconds updateRate;
    };
}
#endif
