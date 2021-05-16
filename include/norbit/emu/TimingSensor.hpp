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
        TimingSensor(MockSensor<T>&& sensor):
            sensor(std::forward(sensor)),
            lastUpdateTime(std::nullopt),
            nextUpdateTime(std::nullopt)
        {}

        virtual ~TimingSensor() = default;

        std::optional<time_point> getNextUpdateTime() const override
        {
            return nextUpdateTime;
        }

        void update() override
        {
            sensor->update();
            updateTimes(nextUpdateTime);
        }

        bool isFinished() const override
        {
            return !sensor.hasNext();
        }

        void start(const time_point& startTime) override
        {
            updateTimes(startTime);
        }
    protected:
        virtual time_point calcNextUpdateTime() const = 0;

    private:
        MockSensor<T> sensor;
        std::optional<time_point> lastUpdateTime;
        std::optional<time_point> nextUpdateTime;

        void updateTimes(const time_point& lastUpdateTime)
        {
            if(isFinished())
            {
                nextUpdateTime = std::nullopt;
            }
            else
            {
                this->lastUpdateTime = std::make_optional(lastUpdateTime);
                nextUpdateTime = std::make_optional(calcNextUpdateTime());
            }
        }
    };
}
#endif
