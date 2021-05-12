#ifndef NORBIT_MOCKSENSOR
#define NORBIT_MOCKSENSOR

#include <optional>
#include "SensorFileReader.hpp"

namespace norbit
{
    /**
     * This class represents an emulated sensor
     * that reads the data from a file.
     *
     * T - the sensor output data type
     */
    template <typename T>
    class MockSensor
    {
    public:
        MockSensor<T>(const std::filesystem::path& sensorDataPath):
            filereader(sensorDataPath, SensorDataParser<T>())
        {
            prepare();
        }

        /**
         * Updates the actualSensorData.
         */
        void update()
        {
            if(hasNext())
            {
                actualSensorData = std::move(*nextSensorData);
                prepare();
            }
        }

        T read() const
        {
            return actualSensorData;
        }

        bool hasNext() const
        {
            return nextSensorData.has_value();
        }
    private:
        T actualSensorData;

        /**
         * The sensor data prepared for next update() call.
         * Preparation is needed to ensure update timing accuracy.
         */
        std::optional<T> nextSensorData;

        SensorFileReader<T> filereader;

        /**
         *  Load next sensor data.
         */
        void prepare()
        {
            nextSensorData = filereader.next();
        }
    };
}

#endif
