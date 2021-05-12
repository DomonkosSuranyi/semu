#ifndef NORBIT_SENSOR_FILE_READER
#define NORBIT_SENSOR_FILE_READER

#include <fstream>
#include <filesystem>
#include <optional>
#include "SensorDataParser.hpp"

namespace norbit
{
    template <typename T>
    class SensorFileReader
    {
    public:
        SensorFileReader(const std::filesystem::path& path, const SensorDataParser<T>& parser);
        const std::optional<T> next();

    private:
        std::ifstream stream;
        const SensorDataParser<T>& parser;
    };

    template <typename T>
    SensorFileReader<T>::SensorFileReader(const std::filesystem::path& path, const SensorDataParser<T>& parser): parser(parser)
    {
        stream = std::ifstream(path);

        // TODO check if file exists

        // First line contains comment
        stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    template <typename T>
    const std::optional<T> SensorFileReader<T>::next()
    {
        std::string nextLine;

        if(std::getline(stream, nextLine))
            return parser.parse(nextLine);


        return {};
    }
}
#endif
