#ifndef NORBIT_GEOREFERAL
#define NORBIT_GEOREFERAL

#include "sensor_data.hpp"

#define PI 3.14159265358979323846

namespace norbit
{
    /**
     * This represents a point on a 2 dimensional plane.
     * The plane is the detection plane of the sonar.
     * For visual explanation see: polarpoint.png
     */
    struct PolarPoint
    {
        double angle, radius;
    };

    class GeoReferal
    {
    public:
        // Temporarily speedOfSound set to constant
        GeoReferal():
            speedOfSound(1435.0)
        {}

        void sonarDataUpdate(const SonarData& newSonarData);
    private:
        const float speedOfSound;
    };
}
#endif
