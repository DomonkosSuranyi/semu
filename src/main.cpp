#include <norbit/emulator/emulator.hpp>
#include <norbit/emulator/Updateable.hpp>
#include <norbit/sonardetect/sensor_data_parsers.hpp>
#include <norbit/sonardetect/SensorDataCollector.hpp>
#include <norbit/TimestampedTimingSensor.hpp>
#include <norbit/FixedRateTimingSensor.hpp>

#include <filesystem>
#include <iostream>

using namespace norbit;
using namespace std::literals;

std::filesystem::path getPathFromEnv(const char* envName)
{
    auto pathEnv = std::getenv(envName);
    if(pathEnv == nullptr)
    {
        throw std::runtime_error(std::string(envName) + " environment variable not found");
    }

    return std::filesystem::path(pathEnv);
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

auto createSonarSensor(SensorDataCollector& collector)
{
    auto sonarPath = getPathFromEnv("SONAR_PATH");
    auto sonarCallback = [&collector](SonarData sonarData)
    {
        collector.sonarDataUpdate(
            Timestamped<SonarData>
            {
                .timestamp = std::chrono::steady_clock::now(),
                .data = std::move(sonarData)
            }
        );
    };
    return std::unique_ptr<Updateable>(
            new TimestampedTimingSensor<SonarData>(sonarPath, sonarCallback));
}

auto createSpeedOfSoundSensor(SensorDataCollector& collector)
{
    auto speedOfSoundPath = getPathFromEnv("SPEED_OF_SOUND_PATH");
    auto speedOfSoundCallback = [&collector](const SpeedOfSound& sos)
    {
        collector.speedOfSoundUpdate(toTimestamped(sos));
    };
    return std::unique_ptr<Updateable>(
            new FixedRateTimingSensor<SpeedOfSound>(speedOfSoundPath, 1s, speedOfSoundCallback));
}

auto createGNSSSensor(SensorDataCollector& collector)
{
    auto gnssPath = getPathFromEnv("GNSS_PATH");
    auto gnssCallback = [&collector](const GNSSData& gnss)
    {
        collector.gnssUpdate(toTimestamped(gnss));
    };
    return std::unique_ptr<Updateable>(new FixedRateTimingSensor<GNSSData>(gnssPath, 20ms, gnssCallback));
}

int main()
{
    SensorDataCollector collector(std::filesystem::path("output.txt"));

    std::vector<std::unique_ptr<Updateable>> sensorVec;
    sensorVec.push_back(createSpeedOfSoundSensor(collector));
    sensorVec.push_back(createGNSSSensor(collector));
    sensorVec.push_back(createSonarSensor(collector));

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

    collector.flush();

    return 0;
}
