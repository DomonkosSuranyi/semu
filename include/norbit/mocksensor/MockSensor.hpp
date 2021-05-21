#ifndef NORBIT_MOCKSENSOR
#define NORBIT_MOCKSENSOR

#include <functional>
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
        using UpdateCallback = std::function<void(T)>;

        MockSensor(const std::filesystem::path& sensorDataPath):
            filereader(sensorDataPath, SensorDataParser<T>())
        {
            prepare();
        }

        T read() const
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
                for(const auto& listener : listeners)
                {
                    listener(actualSensorData);
                }
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

        void registerUpdateListener(UpdateCallback callback)
        {
            listeners.push_back(callback);
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
        std::vector<UpdateCallback> listeners;

    };
}

#endif
