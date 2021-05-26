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

        const T& read() const;

        /**
         * Updates the actualSensorData.
         */
        void update();

        /**
         * Tells if there's next available sensor data
         */
        bool hasNext() const;

        const std::optional<T>& getNextData() const;

        void registerUpdateListener(UpdateCallback callback);

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
        void prepare();
    private:
        SensorFileReader<T> filereader;
        std::vector<UpdateCallback> listeners;

    };

    template <typename T>
    void MockSensor<T>::update()
    {
        if(hasNext())
        {
            actualSensorData = std::move(*nextSensorData);
            for(const auto& listener : listeners)
            {
                listener(std::move(actualSensorData));
            }
            prepare();
        }
    }

    template <typename T>
    const T& MockSensor<T>::read() const
    {
        return actualSensorData;
    }

    template <typename T>
    bool MockSensor<T>::hasNext() const
    {
        return nextSensorData.has_value();
    }

    template <typename T>
    const std::optional<T>& MockSensor<T>::getNextData() const
    {
        return nextSensorData;
    }

    template <typename T>
    void MockSensor<T>::registerUpdateListener(UpdateCallback callback)
    {
        listeners.push_back(callback);
    }

    template <typename T>
    void MockSensor<T>::prepare()
    {
        nextSensorData = filereader.next();
    }
}

#endif
