#ifndef NORBIT_TIMESTAMPED_TIMING_SENSOR
#define NORBIT_TIMESTAMPED_TIMING_SENSOR

#include "TimingSensor.hpp"

namespace norbit
{
    // TODO T should be Timestamped
    template <typename T>
    class TimestampedTimingSensor : public TimingSensor<T>
    {
        TimestampedTimingSensor(
                MockSensor<T>&& sensor,
                const time_point& startTime,
                const std::chrono::microseconds& firstOffset):
            TimingSensor<T>(std::forward(sensor), startTime, startTime + firstOffset)
        {}

    protected:
        time_point calcNextUpdateTime() const
        {
            return MockSensor<T>::getNextData().timestamp - MockSensor<T>::read().timestamp;
        }
    };

}
#endif
