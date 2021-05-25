#ifndef DETECTION_POINT_BATCH
#define DETECTION_POINT_BATCH

#include "sensor_data.hpp"
#include <optional>
#include <vector>

namespace norbit
{

    class DetectionPointSlot
    {
    public:
        enum class State { COMPLETE, GNSS_MISSING };

        DetectionPointSlot(
                Timestamped<SonarData> sonarData,
                const Timestamped<GNSSData>& gnssData);

        State getState() const;

        void setAfterGNSSData(const Timestamped<GNSSData>& afterGNSS);

        const Timestamped<SonarData>& getSonarData() const;

        const Timestamped<GNSSData>& getGNSSData() const;

    private:
        Timestamped<SonarData> sonarData;
        Timestamped<GNSSData> gnssData;
        State currentState;
    };

    class DetectionPointBatch
    {
    public:
        enum class State { COMPLETE, SPEED_OF_SOUND_MISSING, GNSS_MISSING };

        DetectionPointBatch(const Timestamped<SpeedOfSound>& sosBefore);

        DetectionPointBatch(DetectionPointBatch&&) = default;

        DetectionPointBatch(const DetectionPointBatch&) = delete;

        State getState() const;

        State registerSpeedOfSound(const Timestamped<SpeedOfSound>& sosAfter);

        State registerGNSS(const Timestamped<GNSSData>& newGNSS);

        State registerSonarData(Timestamped<SonarData> sonarData);

        const std::vector<DetectionPointSlot>& getSlots() const;

    private:
        const Timestamped<SpeedOfSound> sosBefore;
        std::optional<Timestamped<SpeedOfSound>> sosAfter;
        std::vector<DetectionPointSlot> slots;
        Timestamped<GNSSData> lastGNSS;
        State currentState;
    };
}
#endif
