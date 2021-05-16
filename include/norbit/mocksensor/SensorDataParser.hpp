#ifndef NORBIT_SENSOR_DATA_PARSER
#define NORBIT_SENSOR_DATA_PARSER

// TODO remove
#include "../SensorData.hpp"

#include <string>

namespace norbit
{
    /**
     * To define a parser for any type of sensor data
     * add a parse function.
     *
     * e.g:
     * template<>
     * class SensorDataParser<MySensorData>
     * {
     * public:
     *     MySensorData parse(const std::string& line)
     *     {
     *         ...
     *     }
     * };
     */
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
