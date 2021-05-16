#ifndef NORBIT_FIXED_RATE_TIMING_SENSOR
#define NORBIT_FIXED_RATE_TIMING_SENSOR

#include "TimingSensor.hpp"

namespace norbit
{
    template <typename T>
    class FixedRateTimingSensor : public TimingSensor<T>
    {
    public:
        FixedRateTimingSensor(MockSensor<T>&& sensor, const std::chrono::microseconds& updateDelta):
            TimingSensor<T>(std::forward(sensor)),
            updateDelta(updateDelta)
        {}

    protected:
        time_point calcNextUpdateTime() const
        {
            return TimingSensor<T>::lastUpdateTime + updateDelta;
        }

    private:
        const std::chrono::microseconds updateDelta;
    };
}

#endif
