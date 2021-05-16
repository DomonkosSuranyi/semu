#ifndef NORBIT_SENSOR_DATA_PARSER
#define NORBIT_SENSOR_DATA_PARSER

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
}

#endif
