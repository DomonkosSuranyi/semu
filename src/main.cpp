#include <norbit/emu/Emulator.hpp>
#include <norbit/emu/Updateable.hpp>

using namespace norbit;

int main()
{
    
    norbit::emulate(std::vector<std::unique_ptr<Updateable>>());

    return 0;
}
