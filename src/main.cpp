#include <norbit/emu/Emulator.hpp>
#include <norbit/emu/Updateable.hpp>
#include <norbit/emu/SensorData.hpp>
#include <norbit/emu/TimestampedTimingSensor.hpp>

#include <filesystem>

using namespace norbit;
using namespace std::literals;

int main()
{
    auto sonarPathEnv = std::getenv("SONAR_FILE_PATH");

    auto sonarPath = std::filesystem::path(sonarPathEnv);

    auto sonarSensor = new TimestampedTimingSensor<SonarData>(sonarPath);

    std::vector<std::unique_ptr<Updateable>> sensorVec;
    sensorVec.push_back(std::unique_ptr<Updateable>(sonarSensor));


    norbit::emulate(std::move(sensorVec));

    return 0;
}
