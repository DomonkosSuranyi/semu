#ifndef NORBIT_SD_SENSOR_DATA_PARSERS
#define NORBIT_SD_SENSOR_DATA_PARSERS

#include <norbit/mocksensor/SensorDataParser.hpp>
namespace norbit
{
    template<>
    class SensorDataParser<SonarData>
    {
    public:
        const SonarData parse(const std::string& line) const;
    };
}
#endif
