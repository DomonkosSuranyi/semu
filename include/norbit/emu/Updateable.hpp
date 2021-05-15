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
        virtual ~Updateable() = default;
        /**
         * Updates the output data
         */
        virtual void update() = 0;

        /**
         * Tells if the output data can be updated
         * with a next available data
         */
        virtual bool hasNext() const = 0;

        /**
         */
        virtual std::optional<time_point> nextUpdateTime() = 0;
    };
}
#endif
