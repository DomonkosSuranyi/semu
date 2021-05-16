#ifndef NORBIT_UPDATEABLE
#define NORBIT_UPDATEABLE

#include <chrono>
#include <optional>

namespace norbit
{
    typedef std::chrono::time_point<std::chrono::steady_clock, std::chrono::microseconds> time_point;

    class Updateable
    {

    public:
        virtual ~Updateable() = 0;

        /**
         * Returns the time_point of the next update.
         * Or returns std::nullopt if finished.
         */
        virtual std::optional<time_point> getNextUpdateTime() const = 0;

        /**
         * Updates the output data
         */
        virtual void update() = 0;

        /**
         * Tells if the output data can be updated
         * with a next available data
         */
        virtual bool isFinished() const = 0;

        virtual void start(const time_point& startTime) = 0;
    };
}
#endif
