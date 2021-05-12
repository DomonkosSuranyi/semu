#include <norbit/emu/MockSensor.hpp>
#include <gtest/gtest.h>

using namespace norbit;

TEST(MockSensorTest, has_no_next_when_eof)
{

    std::filesystem::path sensorFilePath = std::filesystem::current_path();
    sensorFilePath /= "resources/file_reader_test_data.txt";

    MockSensor<SonarData> sensor(sensorFilePath);

    ASSERT_TRUE(sensor.hasNext()) << "Sensor has first value";
    sensor.update();
    ASSERT_TRUE(sensor.hasNext()) << "Sensor has second value";
    sensor.update();
    ASSERT_FALSE(sensor.hasNext()) << "Sensor has no more value";
    ASSERT_NO_THROW(sensor.update()) << "Update can be called more";
}