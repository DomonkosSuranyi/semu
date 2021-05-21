#include <norbit/sonardetect/GeoReferal.hpp>

using namespace norbit;

void GeoReferal::sonarDataUpdate(const SonarData& newSonarData)
{
    std::vector<PolarPoint> points;
    for(const auto& measurePoint : newSonarData.measurePoints)
    {
        const auto delaySincePing = measurePoint.sampleIndex * SONAR_SAMPLING_PERIOD_SEC;
        const auto radius = delaySincePing * speedOfSound / 2.0;
        points.push_back(PolarPoint {measurePoint.angle, radius});
    }
}
