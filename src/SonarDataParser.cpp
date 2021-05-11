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

const SonarData SonarDataParser::parse(const std::string& line) const
{
    auto splitted = split(line);
    auto microsec = static_cast<unsigned int>(std::stof(splitted[0]) * 1000000);
    auto timestamp = std::chrono::microseconds(microsec);

    std::array<DetectionPoint, SONAR_N_SAMPLES> detectionPoints;
    for(int i = 1 ; i < SONAR_N_SAMPLES + 1 ; ++i)
    {
        auto tuple = splitted[i];
        auto splitterPos = tuple.find(',');
        auto angle = std::stof(tuple.substr(0, splitterPos));
        auto sampleIndex = std::stoi(tuple.substr(splitterPos + 1, tuple.length()));

        detectionPoints[i-1] = DetectionPoint {angle, sampleIndex};
    }

    return SonarData {timestamp, detectionPoints};
}
