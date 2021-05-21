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
        TimestampedTimingSensor(const std::filesystem::path& sensorFilePath, typename MockSensor<T>::UpdateCallback updateCallback):
            TimingSensor<T>(sensorFilePath, updateCallback)
        {}

    protected:
        time_point calcNextUpdateTime() const
        {
            auto delta = TimingSensor<T>::sensor.getNextData()->timestamp -
                TimingSensor<T>::sensor.read().timestamp;

            return TimingSensor<T>::lastUpdateTime.value() + delta;
        }
    };

}
#endif
