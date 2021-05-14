#include <norbit/emu/FixedRateMockSensor.hpp>
#include <norbit/emu/TimestampedMockSensor.hpp>
#include <gtest/gtest.h>

using namespace norbit;

using namespace std::chrono_literals;

TEST(MockSensorTest, has_no_next_when_eof)
{

    std::filesystem::path sensorFilePath = std::filesystem::current_path();
    sensorFilePath /= "resources/file_reader_test_data.txt";

    FixedRateMockSensor<SonarData> sensor(sensorFilePath, 1us);

    ASSERT_TRUE(sensor.hasNext()) << "Sensor has first value";
    sensor.update();
    ASSERT_TRUE(sensor.hasNext()) << "Sensor has second value";
    sensor.update();
    ASSERT_FALSE(sensor.hasNext()) << "Sensor has no more value";
    ASSERT_NO_THROW(sensor.update()) << "Update can be called more";
}

TEST(TimestampedMockSensor, nextUpdateTime)
{
    auto sensorFilePath = std::filesystem::current_path() / "resources/file_reader_test_data.txt";

    TimestampedMockSensor<SonarData> sensor(sensorFilePath, -1s);

    auto nextUpdateTime = sensor.nextUpdateTime();

    ASSERT_TRUE(sensor.hasNext()) << "Has next";
    ASSERT_TRUE(nextUpdateTime.has_value()) << "Next update time exists";

    // The timestamp is 1553886508.65794
    ASSERT_EQ(*nextUpdateTime, time_point(1553886507657940us));
}
