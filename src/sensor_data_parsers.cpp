#include <norbit/sonardetect/sensor_data_parsers.hpp>
#include <vector>
#include <sstream>

using namespace norbit;

const std::vector<std::string> split(const std::string& text)
{
    std::vector<std::string> result;
    std::istringstream stream(text);
    for(std::string str; stream >> str;)
        result.push_back(str);
    return result;
}

// SonarData
SonarData SensorDataParser<SonarData>::parse(const std::string& line) const
{
    auto splitted = split(line);
    auto microsec = static_cast<long unsigned int>(std::stod(splitted[0]) * 1000000);

    auto timestamp = std::chrono::microseconds(microsec);

    std::vector<SonarMeasurePoint> measurePoints;
    measurePoints.reserve(SONAR_N_SAMPLES);
    for(int i = 1 ; i < SONAR_N_SAMPLES + 1 ; ++i)
    {
        auto tuple = splitted[i];
        auto splitterPos = tuple.find(',');
        auto angle = std::stof(tuple.substr(0, splitterPos));
        auto sampleIndex = std::stoi(tuple.substr(splitterPos + 1, tuple.length()));

        measurePoints.push_back(SonarMeasurePoint {angle, sampleIndex});
    }

    return SonarData(timestamp, measurePoints);
}

// SpeedOfSound
SpeedOfSound SensorDataParser<SpeedOfSound>::parse(const std::string& line) const
{
    return std::stof(line);
}

// GNSSData
GNSSData SensorDataParser<GNSSData>::parse(const std::string& line) const
{
    auto splitted = split(line);

    GNSSData gnssData;
    gnssData.roll = std::stod(splitted[0]);
    gnssData.pitch = std::stod(splitted[1]);
    gnssData.heading = std::stod(splitted[2]);

    gnssData.latitude = std::stod(splitted[3]);
    gnssData.longitude = std::stod(splitted[4]);
    gnssData.altitude = std::stod(splitted[5]);

    return gnssData;
}
