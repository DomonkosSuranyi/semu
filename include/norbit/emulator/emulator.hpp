#ifndef NORBIT_EMULATOR
#define NORBIT_EMULATOR

#include <thread>
#include <vector>
#include "Updateable.hpp"

using namespace std::literals;

#define MAX_WAIT_TIME 24h

namespace norbit
{
    void emulate(std::vector<std::unique_ptr<Updateable>>&& sensors);
}
#endif
