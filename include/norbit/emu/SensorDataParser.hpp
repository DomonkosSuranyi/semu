#ifndef NORBIT_SENSOR_DATA_PARSER
#define NORBIT_SENSOR_DATA_PARSER

#include "SensorData.hpp"
#include <string>

namespace norbit
{

    template <typename T>
    class SensorDataParser;

    template<>
    class SensorDataParser<SonarData>
    {
    public:
        const SonarData parse(const std::string& line) const;
    };
}

#endif
