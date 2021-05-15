#include <norbit/emu/Emulator.hpp>
#include <norbit/emu/MockSensor.hpp>

using namespace norbit;

using time_point = std::chrono::time_point<std::chrono::steady_clock, std::chrono::microseconds>;

enum class UpdateResult
{
    OK, FINISHED
};

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

template <typename T>
class FixedRateTimingSensor : public TimingSensor<T>
{
public:
    FixedRateTimingSensor(MockSensor<T>&& sensor, const time_point& startTime, const std::chrono::microseconds& updateDelta):
        TimingSensor<T>(std::forward(sensor), startTime, startTime + updateDelta),
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

// TODO T should be Timestamped
template <typename T>
class TimestampedTimingSensor : public TimingSensor<T>
{
    TimestampedTimingSensor(MockSensor<T>&& sensor, const time_point& startTime, const std::chrono::microseconds& firstOffset):
        TimingSensor<T>(std::forward(sensor), startTime, startTime + firstOffset)
    {}

protected:
    time_point calcNextUpdateTime() const
    {
        return MockSensor<T>::getNextData().timestamp - MockSensor<T>::read().timestamp;
    }
};

void norbit::emulate(std::vector<std::unique_ptr<Updateable>>&& sensors)
{
    auto now = std::chrono::time_point_cast<std::chrono::microseconds>(
            std::chrono::steady_clock::now());

    for(auto& sensor : sensors)
    {
        if(sensor->isFinished())
            // TODO log
            return;
    }

    for(;;) {
        Updateable* earliestUpdateSensor = &*sensors[0];
        for(const auto& sensor : sensors)
        {
            if(sensor->getNextUpdateTime() < *earliestUpdateSensor->getNextUpdateTime())
            {
                earliestUpdateSensor = &*sensor;
            }
        }

        if(*earliestUpdateSensor->getNextUpdateTime() > now)
            std::this_thread::sleep_until(
                    *earliestUpdateSensor->getNextUpdateTime());

        earliestUpdateSensor->update();
        if(earliestUpdateSensor->isFinished())
            return;
    }
}
