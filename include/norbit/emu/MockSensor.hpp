#ifndef NORBIT_MOCKSENSOR
#define NORBIT_MOCKSENSOR

#include <optional>
#include "SensorFileReader.hpp"
#include "Updateable.hpp"
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
    class MockSensor: public Sensor<T>, Updateable
    {
    public:
        MockSensor(const std::filesystem::path& sensorDataPath):
            filereader(sensorDataPath, SensorDataParser<T>())
        {
            prepare();
        }

        T read() const override
        {
            return actualSensorData;
        }

        /**
         * Updates the actualSensorData.
         */
        void update() override
        {
            if(hasNext())
            {
                actualSensorData = std::move(*nextSensorData);
                prepare();
            }
        }

        bool hasNext() const override
        {
            return nextSensorData.has_value();
        }

    protected:
        /**
         * The sensor data prepared for next update() call.
         * Preparation is needed to ensure update timing accuracy.
         */
        std::optional<T> nextSensorData;

    private:
        T actualSensorData;

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
