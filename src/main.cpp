#include <norbit/emu/Emulator.hpp>
#include <norbit/emu/Updateable.hpp>

#include <filesystem>

using namespace norbit;

int main()
{
    auto sonarPathEnv = std::getenv("SONAR_FILE_PATH");

    auto sonarPath = std::filesystem::path(sonarPathEnv);

    norbit::emulate(std::vector<std::unique_ptr<Updateable>>());

    return 0;
}
