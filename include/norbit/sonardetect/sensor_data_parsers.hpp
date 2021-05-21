#ifndef NORBIT_SD_SENSOR_DATA_PARSERS
#define NORBIT_SD_SENSOR_DATA_PARSERS

#include <norbit/mocksensor/SensorDataParser.hpp>
#include "sensor_data.hpp"
#include <string>

namespace norbit
{
    template<>
    class SensorDataParser<SonarData>
    {
    public:
        SonarData parse(const std::string& line) const;
    };

    template<>
    class SensorDataParser<SpeedOfSound>
    {
    public:
        SpeedOfSound parse(const std::string& line) const;
    };
}
#endif
