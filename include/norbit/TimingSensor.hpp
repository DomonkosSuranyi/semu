#ifndef NORBIT_TIMING_SENSOR
#define NORBIT_TIMING_SENSOR

#include <norbit/emulator/Updateable.hpp>
#include <norbit/mocksensor/MockSensor.hpp>

namespace norbit
{
    template <typename T>
    class TimingSensor : public Updateable
    {
    public:
        TimingSensor<T>(const std::filesystem::path& sensorFilePath):
            sensor(MockSensor<T>(sensorFilePath)),
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
            if(isFinished())
            {
                nextUpdateTime = std::nullopt;
            }
            else
            {
                sensor.update();
                updateTimes(*nextUpdateTime);
            }
        }

        bool isFinished() const override
        {
            return !sensor.hasNext();
        }

        void start(const time_point& startTime) override
        {
            sensor.update();
            updateTimes(startTime);
        }
    protected:
        virtual time_point calcNextUpdateTime() const = 0;
        MockSensor<T> sensor;
        std::optional<time_point> lastUpdateTime;

    private:
        std::optional<time_point> nextUpdateTime;

        void updateTimes(const time_point& lastUpdateTime)
        {
            this->lastUpdateTime = std::make_optional(lastUpdateTime);

            nextUpdateTime = std::make_optional(calcNextUpdateTime());
        }
    };
}
#endif