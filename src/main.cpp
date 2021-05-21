#include <norbit/emulator/emulator.hpp>
#include <norbit/emulator/Updateable.hpp>
#include <norbit/sonardetect/sensor_data_parsers.hpp>
#include <norbit/TimestampedTimingSensor.hpp>
#include <norbit/FixedRateTimingSensor.hpp>

#include <filesystem>
#include <iostream>

using namespace norbit;
using namespace std::literals;

int main()
{
    auto sonarPathEnv = std::getenv("SONAR_FILE_PATH");
    if(sonarPathEnv == nullptr)
    {
        std::cerr << "Error: Missing path. Please define SONAR_FILE_PATH environment variable." << std::endl;
        return 0;
    }

    auto sonarPath = std::filesystem::path(sonarPathEnv);

    auto sonarSensor = new TimestampedTimingSensor<SonarData>(sonarPath);


    auto speedOfSoundPathEnv = std::getenv("SPEED_OF_SOUND_FILE_PATH");
    if(speedOfSoundPathEnv == nullptr)
    {
        std::cerr << "Error: Missing path. Please define SPEED_OF_SOUND_FILE_PATH environment variable." << std::endl;
        return 0;
    }

    auto speedOfSoundPath = std::filesystem::path(speedOfSoundPathEnv);

    auto speedOfSoundSensor = new FixedRateTimingSensor<SpeedOfSound>(speedOfSoundPath, 1s);

    std::vector<std::unique_ptr<Updateable>> sensorVec;
    sensorVec.push_back(std::unique_ptr<Updateable>(sonarSensor));
    sensorVec.push_back(std::unique_ptr<Updateable>(speedOfSoundSensor));

    auto result = norbit::emulate(std::move(sensorVec));

    if(result == EmulationResult::COULD_NOT_START)
    {
        std::cerr << "Error: Emulation could not be started (Empty sensor file)" << std::endl;
    }
    else
    {
        std::cout << "Emulation finished" << std::endl;
    }

    return 0;
}
