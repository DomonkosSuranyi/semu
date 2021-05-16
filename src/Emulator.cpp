#include <norbit/emu/Emulator.hpp>
#include <norbit/emu/MockSensor.hpp>

using namespace norbit;

using time_point = std::chrono::time_point<std::chrono::steady_clock, std::chrono::microseconds>;

void norbit::emulate(std::vector<std::unique_ptr<Updateable>>&& sensors)
{
    if(sensors.empty())
        // TODO log
        return;

    auto now = std::chrono::time_point_cast<std::chrono::microseconds>(
            std::chrono::steady_clock::now());

    for(auto& sensor : sensors)
    {
        if(sensor->isFinished())
            // TODO log
            return;
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
            return;
    }
}

