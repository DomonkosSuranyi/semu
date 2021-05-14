#ifndef NORBIT_EMULATOR
#define NORBIT_EMULATOR

#include <thread>
#include <vector>
#include "Updateable.hpp"

using namespace std::literals;

#define MAX_WAIT_TIME 24h

namespace norbit
{
    void emulate(std::vector<std::unique_ptr<Updateable>>&& sensors)
    {
        for(const auto& sensor : sensors)
        {
            if(!sensor->hasNext())
                // TODO log
                return;
        }

        for(;;) {
            auto nextUpdateTime = time_point(MAX_WAIT_TIME);
            std::unique_ptr<Updateable>* nextSensorToUpdate;
            for(auto& sensor : sensors)
            {
                auto sensorNextUpdate = sensor->nextUpdateTime();
                if(sensorNextUpdate < nextUpdateTime)
                {
                    nextUpdateTime = sensorNextUpdate.value();
                    nextSensorToUpdate = &sensor;
                }
            }

            if(nextSensorToUpdate)
                // All time exceeds the MAX_WAIT_TIME
                return;

            // WAIT
            if(nextUpdateTime > std::chrono::steady_clock::now())
                std::this_thread::sleep_until(nextUpdateTime);

            nextSensorToUpdate->get()->update();
            if(!nextSensorToUpdate->get()->hasNext())
                return;
        }
    }
}
#endif
