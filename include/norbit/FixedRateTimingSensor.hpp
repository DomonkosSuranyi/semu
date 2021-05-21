#ifndef NORBIT_FIXED_RATE_TIMING_SENSOR
#define NORBIT_FIXED_RATE_TIMING_SENSOR

#include "TimingSensor.hpp"

namespace norbit
{
    template <typename T>
    class FixedRateTimingSensor : public TimingSensor<T>
    {
    public:
        FixedRateTimingSensor(
                const std::filesystem::path& sensorFilePath,
                const std::chrono::microseconds& updateDelta,
                typename MockSensor<T>::UpdateCallback updateCallback):
            TimingSensor<T>(sensorFilePath, updateCallback),
            updateDelta(updateDelta)
        {}

    protected:
        time_point calcNextUpdateTime() const
        {
            return *TimingSensor<T>::lastUpdateTime + updateDelta;
        }

    private:
        const std::chrono::microseconds updateDelta;
    };
}

#endif
