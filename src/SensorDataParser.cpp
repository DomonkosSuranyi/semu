#include <norbit/emu/SensorDataParser.hpp>
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

// Sonar
const SonarData SensorDataParser<SonarData>::parse(const std::string& line) const
{
    auto splitted = split(line);
    auto microsec = static_cast<long unsigned int>(std::stod(splitted[0]) * 1000000);

    auto timestamp = std::chrono::microseconds(microsec);

    std::vector<DetectionPoint> detectionPoints;
    detectionPoints.reserve(SONAR_N_SAMPLES);
    for(int i = 1 ; i < SONAR_N_SAMPLES + 1 ; ++i)
    {
        auto tuple = splitted[i];
        auto splitterPos = tuple.find(',');
        auto angle = std::stof(tuple.substr(0, splitterPos));
        auto sampleIndex = std::stoi(tuple.substr(splitterPos + 1, tuple.length()));

        detectionPoints.push_back(DetectionPoint {angle, sampleIndex});
    }

    return SonarData(timestamp, detectionPoints);
}
