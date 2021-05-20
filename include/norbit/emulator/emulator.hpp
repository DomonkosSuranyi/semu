#ifndef NORBIT_EMULATOR
#define NORBIT_EMULATOR

#include <thread>
#include <vector>
#include <functional>
#include "Updateable.hpp"

namespace norbit
{
    enum class EmulationResult
    {
        FINISHED, COULD_NOT_START
    };

    EmulationResult emulate(
            std::vector<std::unique_ptr<Updateable>>&& sensors,
            const std::function<time_point()> nowSupplier);

    EmulationResult emulate(std::vector<std::unique_ptr<Updateable>>&& sensors);
}
#endif
