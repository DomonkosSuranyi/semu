#include <gmock/gmock.h>
#include <norbit/emulator/emulator.hpp>
#include <norbit/emulator/Updateable.hpp>

using namespace norbit;
using namespace testing;
using namespace std::literals;

class MockUpdateable : public Updateable
{
public:
    MOCK_METHOD((std::optional<time_point>), getNextUpdateTime, (), (const));
    MOCK_METHOD(void, update, ());
    MOCK_METHOD(bool, isFinished, (), (const));
    MOCK_METHOD(void, start, (const time_point&));
};

class CallSequenceTestHelper
{
public:
    void callFirstExpected()
    {
        firstInvoked = true;
        if(!secondInvoked)
            properSequence = true;
    }

    void callSecondExpected()
    {
        secondInvoked = true;
    }

    inline bool wasProperSequence()
    {
        return properSequence;
    }

private:
    bool firstInvoked;
    bool secondInvoked;
    bool properSequence;
};

inline auto updateablesVec()
{
    return std::vector<std::unique_ptr<Updateable>>();
}

TEST(EmulatorTest, starts_sensors_before_update)
{
    CallSequenceTestHelper seqHelper;
    auto mockUpdateable = new MockUpdateable();

    EXPECT_CALL(*mockUpdateable, isFinished())
        // not finished when init checked
        .WillOnce(Return(false))
        // finished after updated
        .WillRepeatedly(Return(true));

    time_point timepoint;

    EXPECT_CALL(*mockUpdateable, start(_))
        .WillOnce(Invoke([&seqHelper, &timepoint](auto tp)
            {
                seqHelper.callFirstExpected();
                timepoint = tp;
            }));

    EXPECT_CALL(*mockUpdateable, getNextUpdateTime())
        .WillRepeatedly(Return(timepoint));

    EXPECT_CALL(*mockUpdateable, update())
        .WillOnce(Invoke([&seqHelper]()
            {
                seqHelper.callSecondExpected();
            }));

    auto updateables = updateablesVec();
    updateables.push_back(std::unique_ptr<Updateable>(mockUpdateable));

    auto result = norbit::emulate(std::move(updateables));

    EXPECT_TRUE(seqHelper.wasProperSequence()) << "start() has been ivoked before update()";

    ASSERT_EQ(EmulationResult::FINISHED, result) << "Emulation finished properly";
}

TEST(EmulatorTest, return_error_when_empty)
{
    EXPECT_EQ(EmulationResult::COULD_NOT_START, emulate(updateablesVec())) << "Empty vector";

    auto mockUpdateable = new MockUpdateable();

    EXPECT_CALL(*mockUpdateable, isFinished())
        .WillOnce(Return(true));

    auto updateables = updateablesVec();
    updateables.push_back(std::unique_ptr<Updateable>(mockUpdateable));
    EXPECT_EQ(EmulationResult::COULD_NOT_START, emulate(std::move(updateables))) << "No next data in updateable";
}

class MockClock
{
public:
    MockClock(std::initializer_list<time_point> returnSequence):
        returnSequence(std::move(returnSequence)),
        callIdx(0)
    {}

    time_point getTimePoint()
    {
        if(callIdx <= returnSequence.size())
        {
            return returnSequence[callIdx];
        }

        return returnSequence[returnSequence.size() - 1];
    }

    inline void step()
    {
        ++callIdx;
    }
private:
    const std::vector<time_point> returnSequence;
    unsigned int callIdx;
};

inline time_point steady(std::chrono::microseconds duration)
{
    return std::chrono::time_point_cast<std::chrono::microseconds>(
            std::chrono::time_point<std::chrono::steady_clock>(duration));
}

TEST(EmulatorTest, updates_earliest_first)
{
    CallSequenceTestHelper seqHelper;

    auto firstToUpdate = new NiceMock<MockUpdateable>();

    auto firstTimePointSequence = MockClock {
        steady(5s),
        steady(10s)
    };

    EXPECT_CALL(*firstToUpdate, isFinished())
        .WillOnce(Return(false))
        .WillOnce(Return(false))
        .WillRepeatedly(Return(true));

    EXPECT_CALL(*firstToUpdate, getNextUpdateTime())
        .WillRepeatedly(Invoke([&firstTimePointSequence]()
            {
                return firstTimePointSequence.getTimePoint();
            }));

    EXPECT_CALL(*firstToUpdate, update())
        .WillOnce(Invoke([&seqHelper, &firstTimePointSequence]()
            {
                seqHelper.callFirstExpected();
                firstTimePointSequence.step();
            }));

    auto secondToUpdate = new NiceMock<MockUpdateable>();

    EXPECT_CALL(*secondToUpdate, isFinished())
        .WillOnce(Return(false))
        .WillRepeatedly(Return(true));

    EXPECT_CALL(*secondToUpdate, getNextUpdateTime())
        .WillRepeatedly(Return(steady(8s)));

    EXPECT_CALL(*secondToUpdate, update())
        .WillOnce(Invoke([&seqHelper]()
            {
                seqHelper.callSecondExpected();
            }));

    auto updateables = updateablesVec();
    updateables.push_back(std::unique_ptr<Updateable>(secondToUpdate));
    updateables.push_back(std::unique_ptr<Updateable>(firstToUpdate));

    EXPECT_EQ(EmulationResult::FINISHED, emulate(std::move(updateables))) << "Emulation finished properly";

    EXPECT_TRUE(seqHelper.wasProperSequence()) << "Update has been executed in proper order";
}

#ifdef RUN_TIMING_TEST
TEST(EmulatorTest, timing)
{
    MockClock clock {
        // First will be late
        steady(6s),
        // Half second before next update
        steady(7500ms)
    };

    auto firstToUpdate = new NiceMock<MockUpdateable>();

    auto firstTimePointSequence = MockClock {
        steady(5s),
        steady(10s)
    };

    std::chrono::time_point<std::chrono::steady_clock> firstUpdateTime, secondUpdateTime;

    EXPECT_CALL(*firstToUpdate, isFinished())
        .WillOnce(Return(false))
        .WillOnce(Return(false))
        .WillRepeatedly(Return(true));

    EXPECT_CALL(*firstToUpdate, getNextUpdateTime())
        .WillRepeatedly(Invoke([&firstTimePointSequence]()
            {
                return firstTimePointSequence.getTimePoint();
            }));

    EXPECT_CALL(*firstToUpdate, update())
        .WillOnce(Invoke([&firstUpdateTime, &clock, &firstTimePointSequence]()
            {
                firstUpdateTime = std::chrono::steady_clock::now();
                clock.step();
                firstTimePointSequence.step();
            }));

    auto secondToUpdate = new NiceMock<MockUpdateable>();

    EXPECT_CALL(*secondToUpdate, isFinished())
        .WillOnce(Return(false))
        .WillRepeatedly(Return(true));

    EXPECT_CALL(*secondToUpdate, getNextUpdateTime())
        .WillRepeatedly(Return(steady(8s)));

    EXPECT_CALL(*secondToUpdate, update())
        .WillOnce(Invoke([&secondUpdateTime, &clock]()
            {
                secondUpdateTime = std::chrono::steady_clock::now();
                clock.step();
            }));

    auto updateables = updateablesVec();
    updateables.push_back(std::unique_ptr<Updateable>(secondToUpdate));
    updateables.push_back(std::unique_ptr<Updateable>(firstToUpdate));

    auto mockNowSupplier = [&clock](){ 
        return clock.getTimePoint(); };

    EXPECT_EQ(EmulationResult::FINISHED, emulate(std::move(updateables), mockNowSupplier)) << "Emulation finished properly";

    EXPECT_EQ(500ms, std::chrono::duration_cast<std::chrono::milliseconds>(secondUpdateTime - firstUpdateTime)) << "The difference between update times was under 1ms";

}
#endif
