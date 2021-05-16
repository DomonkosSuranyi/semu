#include <norbit/sonardetect/sensor_data_parsers.hpp>
#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>

using namespace norbit;

const std::string getLineFromFile(const std::string& filename, const unsigned int lineIdx)
{
    auto path = std::filesystem::current_path();
    path /= "resources";
    path /= filename;

    std::ifstream in(path.c_str());

    std::string line;

    //skip N lines
    for(unsigned int i = 0; i < lineIdx; ++i)
    {
        std::getline(in, line);
    }

    std::getline(in,line);
    return line;
}

TEST(SensorDataParserTest, parse_sonar_data)
{
    SensorDataParser<SonarData> parser;
    auto sonarData = parser.parse(getLineFromFile("sonar_test_data.txt", 0));
    ASSERT_EQ(
        std::chrono::microseconds(1553886508657940),
        sonarData.timestamp);

    DetectionPoint detPoint{-1.441, 2848};

    ASSERT_EQ(
        detPoint,
        sonarData.detectionPoints[0]);

    detPoint = DetectionPoint{-1.4, 2508};

    ASSERT_EQ(
        detPoint,
        sonarData.detectionPoints[7]);

    detPoint = DetectionPoint{1.444, 2743};

    ASSERT_EQ(
        detPoint,
        sonarData.detectionPoints[511]);
}

