#include <norbit/emulator/emulator.hpp>
#include <norbit/mocksensor/MockSensor.hpp>

using namespace norbit;

EmulationResult norbit::emulate(
        std::vector<std::unique_ptr<Updateable>>&& sensors,
        const std::function<time_point()> nowSupplier)
{
    if(sensors.empty())
        return EmulationResult::COULD_NOT_START;

    auto now = nowSupplier();

    for(auto& sensor : sensors)
    {
        if(sensor->isFinished())
            return EmulationResult::COULD_NOT_START;
        sensor->start(now);
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
            return EmulationResult::FINISHED;

        now = nowSupplier();
    }
}

EmulationResult norbit::emulate(std::vector<std::unique_ptr<Updateable>>&& sensors)
{
    auto getNow = []()
    {
        return std::chrono::time_point_cast<std::chrono::microseconds>(
                std::chrono::steady_clock::now());
    };
    return emulate(std::move(sensors), getNow);
}

