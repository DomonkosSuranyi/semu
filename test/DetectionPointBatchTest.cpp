#include <norbit/sonardetect/DetectionPointBatch.hpp>
#include <gtest/gtest.h>

#define SLOT_TEST DetectionPointSlot
#define BATCH_TEST DetectionPointBatch

using namespace norbit;
using namespace std::literals;

TEST(SLOT_TEST, initial_state)
{
    Timestamped<SonarData> sonarData{};
    DetectionPointSlot slot(Timestamped<SonarData>{}, Timestamped<GNSSData>{});

    ASSERT_EQ(DetectionPointSlot::State::GNSS_MISSING, slot.getState());
}

TEST(SLOT_TEST, update_GNSS_to_closest)
{
    Timestamp gnssInitialTimestamp(3ns);
    Timestamped<SonarData> sonarData;
    sonarData.timestamp = Timestamp(10ns);

    Timestamped<GNSSData> gnssData;
    gnssData.timestamp = Timestamp(gnssInitialTimestamp);

    DetectionPointSlot slot(std::move(sonarData), gnssData);

    EXPECT_EQ(gnssData.timestamp, slot.getGNSSData().timestamp)
        << "Initial GNSS data";

    gnssData.timestamp = Timestamp(18ns);
    slot.setAfterGNSSData(gnssData);

    EXPECT_EQ(gnssInitialTimestamp, slot.getGNSSData().timestamp)
        << "no update to further GNSS data";

    EXPECT_EQ(DetectionPointSlot::State::COMPLETE, slot.getState())
        << "state set to COMPLETED";

    gnssData.timestamp = Timestamp(15ns);
    slot.setAfterGNSSData(gnssData);

    EXPECT_EQ(gnssData.timestamp, slot.getGNSSData().timestamp)
        << "update to closer GNSS data";

    EXPECT_EQ(DetectionPointSlot::State::COMPLETE, slot.getState())
        << "state still to COMPLETED";
}

TEST(BATCH_TEST, initial_state)
{
    DetectionPointBatch batch(Timestamped<SpeedOfSound>{});
    ASSERT_EQ(
            DetectionPointBatch::State::SPEED_OF_SOUND_MISSING,
            batch.getState())
        << "Proper state";

    ASSERT_TRUE(batch.getSlots().empty()) << "No slots";
}

TEST(BATCH_TEST, finished_slot_before_speed_of_sound)
{
    DetectionPointBatch batch(Timestamped<SpeedOfSound>{});
    Timestamped<GNSSData> lastGNSS;

    EXPECT_EQ(
            DetectionPointBatch::State::SPEED_OF_SOUND_MISSING,
            batch.registerGNSS(lastGNSS))
        << "State after GNSS data before sonar data";

    EXPECT_EQ(
            DetectionPointBatch::State::SPEED_OF_SOUND_MISSING,
            batch.registerSonarData(Timestamped<SonarData>()))
        << "State after sonarData registration";

    const auto& slots = batch.getSlots();
    EXPECT_EQ(1, slots.size()) << "Slot created";
    EXPECT_EQ(
            DetectionPointSlot::State::GNSS_MISSING,
            slots[0].getState())
        << "Slot state after sonarData registration";

    EXPECT_EQ(
            DetectionPointBatch::State::SPEED_OF_SOUND_MISSING,
            batch.registerGNSS(Timestamped<GNSSData>()));

    EXPECT_EQ(
            DetectionPointSlot::State::COMPLETE,
            batch.getSlots()[0].getState())
        << "Slot state after GNSS registration";

    EXPECT_EQ(
            DetectionPointBatch::State::COMPLETE,
            batch.registerSpeedOfSound(Timestamped<SpeedOfSound>{}))
        << "State after speed of sound registration";
}

TEST(BATCH_TEST, unfinished_slot_before_speed_of_sound)
{
    DetectionPointBatch batch(Timestamped<SpeedOfSound>{});
    Timestamped<GNSSData> lastGNSS;

    batch.registerGNSS(lastGNSS);
    batch.registerSonarData(Timestamped<SonarData>());

    EXPECT_EQ(
            DetectionPointBatch::State::GNSS_MISSING,
            batch.registerSpeedOfSound(Timestamped<SpeedOfSound>{}))
        << "State after speed of sound registration";

    EXPECT_EQ(
            DetectionPointBatch::State::COMPLETE,
            batch.registerGNSS(Timestamped<GNSSData>{}))
        << "State after GNSS registration";
}
