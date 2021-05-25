#include <norbit/emulator/emulator.hpp>
#include <norbit/emulator/Updateable.hpp>
#include <norbit/sonardetect/sensor_data_parsers.hpp>
#include <norbit/sonardetect/GeoReferal.hpp>
#include <norbit/TimestampedTimingSensor.hpp>
#include <norbit/FixedRateTimingSensor.hpp>

#include <filesystem>
#include <iostream>

using namespace norbit;
using namespace std::literals;

std::optional<std::filesystem::path> getPathFromEnv(const char* envName)
{
    auto pathEnv = std::getenv(envName);
    if(pathEnv == nullptr)
    {
        std::cerr << "Error: Missing path. Please define "
            << envName
            << " environment variable."
            << std::endl;
        return std::nullopt;
    }

    auto path = std::filesystem::path(pathEnv);

    return std::optional(path);
}

template <typename T>
Timestamped<T> toTimestamped(const T& data)
{
    return Timestamped<T>
    {
        .timestamp = std::chrono::steady_clock::now(),
        .data = data
    };
}

int main()
{
    GeoReferal gref;

    auto sonarPath = getPathFromEnv("SONAR_PATH");
    if(!sonarPath.has_value())
        return 0;
    auto sonarCallback = [&gref](SonarData sonarData)
    {
        gref.sonarDataUpdate(
            Timestamped<SonarData>
            {
                .timestamp = std::chrono::steady_clock::now(),
                .data = std::move(sonarData)
            }
        );
    };
    auto sonarSensor = new TimestampedTimingSensor<SonarData>(*sonarPath, sonarCallback);

    auto speedOfSoundPath = getPathFromEnv("SPEED_OF_SOUND_PATH");
    if(!speedOfSoundPath.has_value())
        return 0;
    auto speedOfSoundCallback = [&gref](const SpeedOfSound& sos)
    {
        gref.speedOfSoundUpdate(toTimestamped(sos));
    };
    auto speedOfSoundSensor = new FixedRateTimingSensor<SpeedOfSound>(*speedOfSoundPath, 1s, speedOfSoundCallback);

    auto gnssPath = getPathFromEnv("GNSS_PATH");
    if(!gnssPath.has_value())
        return 0;
    auto gnssCallback = [&gref](const GNSSData& gnss)
    {
        gref.gnssUpdate(toTimestamped(gnss));
    };
    auto gnssSensor = new FixedRateTimingSensor<GNSSData>(*gnssPath, 20ms, gnssCallback);

    std::vector<std::unique_ptr<Updateable>> sensorVec;
    sensorVec.push_back(std::unique_ptr<Updateable>(sonarSensor));
    sensorVec.push_back(std::unique_ptr<Updateable>(speedOfSoundSensor));
    sensorVec.push_back(std::unique_ptr<Updateable>(gnssSensor));


    std::cout << "Start emulation" << std::endl;

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
