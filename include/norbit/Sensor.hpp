#ifndef NORBIT_SENSOR
#define NORBIT_SENSOR

namespace norbit
{
    // This is an interface for sensors
    template <typename T>
    class Sensor
    {
    public:
        virtual T read() const = 0;
    };
}
#endif
