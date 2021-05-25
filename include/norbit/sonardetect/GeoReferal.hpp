#ifndef NORBIT_GEOREFERAL
#define NORBIT_GEOREFERAL

#include "sensor_data.hpp"
#include "DetectionPointBatch.hpp"
#include <memory>

namespace norbit
{
    /**
     * This class collects received sensor data into a structured form.
     * TODO write here how it does the finalization
     * TODO rename maybe
     */
    class GeoReferal
    {
    public:
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

        void finalizeBatch(std::unique_ptr<DetectionPointBatch>&& batch);
    };
}
#endif
