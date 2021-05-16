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
                const std::filesystem::path& sensorFilePath,
                const std::chrono::microseconds& firstOffset):
            TimingSensor<T>(sensorFilePath),
            initialOffset(firstOffset)
        {}

    void start(const time_point& startTime)
    {
        TimingSensor<T>::start(startTime + initialOffset);
    }

    protected:
        time_point calcNextUpdateTime() const
        {
            auto delta = TimingSensor<T>::sensor.getNextData()->timestamp -
                TimingSensor<T>::sensor.read().timestamp;

            return TimingSensor<T>::lastUpdateTime.value() + delta;
        }

    private:
        const std::chrono::microseconds initialOffset;
    };

}
#endif
