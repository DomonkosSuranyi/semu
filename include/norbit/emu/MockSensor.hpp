#ifndef NORBIT_MOCKSENSOR
#define NORBIT_MOCKSENSOR

#include <optional>
#include "SensorFileReader.hpp"
#include <norbit/Sensor.hpp>

namespace norbit
{
    /**
     * This class represents an emulated sensor
     * that reads the data from a file.
     *
     * T - the sensor output data type
     */
    template <typename T>
    class MockSensor: public Sensor<T>
    {
    public:
        MockSensor(const std::filesystem::path& sensorDataPath):
            filereader(sensorDataPath, SensorDataParser<T>())
        {
            prepare();
        }

        virtual ~MockSensor() = default;

        T read() const override
        {
            return actualSensorData;
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

        bool hasNext() const
        {
            return nextSensorData.has_value();
        }

        const std::optional<T>& getNextData() const
        {
            return nextSensorData;
        }

    protected:
        /**
         * The sensor data prepared for next update() call.
         * Preparation is needed to ensure update timing accuracy.
         */
        std::optional<T> nextSensorData;

        T actualSensorData;

        /**
         *  Load next sensor data.
         */
        void prepare()
        {
            nextSensorData = filereader.next();
        }
    private:

        SensorFileReader<T> filereader;

    };
}

#endif
