#ifndef NORBIT_TIMING_SENSOR
#define NORBIT_TIMING_SENSOR

#include "Updateable.hpp"
#include "MockSensor.hpp"

namespace norbit
{
    template <typename T>
    class TimingSensor : public Updateable
    {
    public:
        TimingSensor(MockSensor<T>&& sensor, const time_point& startTime, const time_point& firstUpdateTime):
            sensor(std::forward(sensor)),
            lastUpdateTime(startTime)
        {
            nextUpdateTime = firstUpdateTime;
        }

        virtual ~TimingSensor() = default;

        std::optional<time_point> getNextUpdateTime() const override
        {
            return isFinished() ? nextUpdateTime : std::nullopt;
        }

        void update() override
        {
            sensor->update();

            lastUpdateTime = nextUpdateTime;
            nextUpdateTime = calcNextUpdateTime();
        }

        bool isFinished() const override
        {
            return !sensor.hasNext();
        }

    protected:
        virtual time_point calcNextUpdateTime() const = 0;

    private:
        MockSensor<T> sensor;
        time_point lastUpdateTime;
        time_point nextUpdateTime;
    };
}
#endif
