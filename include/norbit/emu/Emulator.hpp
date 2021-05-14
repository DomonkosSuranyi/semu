#ifndef NORBIT_EMULATOR
#define NORBIT_EMULATOR

#include <thread>
#include <vector>
#include "Updateable.hpp"

using namespace std::literals;

namespace norbit
{
    void emulate(std::vector<Updateable>& sensors)
    {
        for(;;) {
            auto nextUpdateTime = std::chrono::steady_clock::now() + 24h;
            Updateable* nextUpdateSensor;
            for(auto& sensor : sensors)
            {
                auto sensorNextUpdate = sensor.nextUpdateTime();
                if(sensorNextUpdate < nextUpdateTime)
                {
                    nextUpdateTime = sensorNextUpdate;
                    nextUpdateSensor = &sensor;
                }
            }

            // wait until necessary
            std::this_thread::sleep_until(nextUpdateTime);

            nextUpdateSensor->update();
            if(!nextUpdateSensor->hasNext())
                return;
        }
    }
}
#endif
