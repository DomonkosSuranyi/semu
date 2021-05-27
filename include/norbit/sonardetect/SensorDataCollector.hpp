#ifndef NORBIT_SENSOR_DATA_COLLECTOR
#define NORBIT_SENSOR_DATA_COLLECTOR

#include "sensor_data.hpp"
#include "DetectionPointBatch.hpp"
#include "BatchFinalizer.hpp"
#include <memory>
#include <thread>

namespace norbit
{
    /**
     * This class collects received sensor data into a structured form.
     * When a Speed of Sound sensor data received
     * the collected data since last Speed of Sound will be finalized.
     */
    class SensorDataCollector
    {
    public:
        SensorDataCollector(const std::filesystem::path& outputPath);
        /**
         * Callback for incoming sonar sensor data
         */
        void sonarDataUpdate(Timestamped<SonarData> sonarData);

        /**
         * Callback for incoming GNSS/INS sensor data
         */
        void gnssUpdate(const Timestamped<GNSSData>& gnss);

        /**
         * Callback for incoming speed of sound sensor data
         * Creates a new DetectionPointBatch
         */
        void speedOfSoundUpdate(const Timestamped<SpeedOfSound>& speedOfSound);

        /**
         * Blocks current thread until finalizing is in progress
         */
        void flush();

    private:
        /**
         * When a batches last slot has not been closed yet
         * with GNSS but a SpeedOfSound occurs the evaluation
         * of the batch cannot be started until next GNSS received
         */
        std::unique_ptr<DetectionPointBatch> waitForGNSSBatch;

        /**
         * The batch where the received sonar data is stored in.
         * Changes to a new batch when SpeedOfSound occurs.
         */
        std::unique_ptr<DetectionPointBatch> openBatch;

        BatchFinalizer finalizer;

        std::optional<std::thread> finalizingThread;

        void finalizeBatch(std::unique_ptr<DetectionPointBatch>&& batch);
    };
}
#endif
