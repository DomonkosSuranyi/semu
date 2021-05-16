#ifndef NORBIT_TIMESTAMPED_TIMING_SENSOR
#define NORBIT_TIMESTAMPED_TIMING_SENSOR

#include "TimingSensor.hpp"

namespace norbit
{
    // TODO T should be Timestamped
    template <typename T>
    class TimestampedTimingSensor : public TimingSensor<T>
    {
    public:
        TimestampedTimingSensor(
                MockSensor<T>&& sensor,
                const std::chrono::microseconds& firstOffset):
            TimingSensor<T>(std::forward(sensor)),
            initialOffset(firstOffset)
        {}

    void start(const time_point& startTime)
    {
        TimingSensor<T>::start(startTime + initialOffset);
    }

    protected:
        time_point calcNextUpdateTime() const
        {
            return MockSensor<T>::getNextData().timestamp - MockSensor<T>::read().timestamp;
        }

    private:
        const std::chrono::microseconds initialOffset;
    };

}
#endif
