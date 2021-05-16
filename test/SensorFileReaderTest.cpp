#include <norbit/mocksensor/SensorFileReader.hpp>
#include <norbit/sonardetect/sensor_data_parsers.hpp>
#include <gtest/gtest.h>

using namespace norbit;

TEST(SensorFileReaderTest, lists_all_line_except_first)
{
    auto path = std::filesystem::current_path();
    path /= "resources/file_reader_test_data.txt";

    SensorFileReader reader(path, SensorDataParser<SonarData>());

    // The file has 1 comment line and 2 data lines
    ASSERT_TRUE(reader.next().has_value()) << "File contains 1 data line";
    ASSERT_TRUE(reader.next().has_value()) << "File contains 2 data lines";
    ASSERT_FALSE(reader.next().has_value()) << "File ended";

}
