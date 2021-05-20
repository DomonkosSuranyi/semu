#include <gmock/gmock.h>
#include <norbit/emulator/emulator.hpp>
#include <norbit/emulator/Updateable.hpp>

using namespace norbit;
using namespace testing;

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
    void callStart()
    {
        startCalled = true;
        if(!updateCalled)
            startCalledFirst = true;
    }

    void callUpdate()
    {
        updateCalled = true;
    }

    inline void expectStartCalledFirst()
    {
        EXPECT_TRUE(startCalledFirst) << "start() invoked before update()";
    }

private:
    bool startCalled;
    bool updateCalled;
    bool startCalledFirst;
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
                seqHelper.callStart();
                timepoint = tp;
            }));

    EXPECT_CALL(*mockUpdateable, getNextUpdateTime())
        .WillRepeatedly(Return(timepoint));

    EXPECT_CALL(*mockUpdateable, update())
        .WillOnce(Invoke([&seqHelper]()
            {
                seqHelper.callUpdate();
            }));

    auto updateables = updateablesVec();
    updateables.push_back(std::unique_ptr<Updateable>(mockUpdateable));

    auto result = norbit::emulate(std::move(updateables));

    seqHelper.expectStartCalledFirst();

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

