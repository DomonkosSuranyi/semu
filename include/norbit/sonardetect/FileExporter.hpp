#ifndef NORBIT_FILE_EXPORTER
#define NORBIT_FILE_EXPORTER

#include <fstream>
#include <filesystem>
#include "evaluation.hpp"

namespace norbit
{
    class FileExporter
    {
    public:
        FileExporter(const std::filesystem::path& outputPath):
            ofstream(std::ofstream(outputPath))
        {}

        ~FileExporter()
        {
            ofstream.close();
        }

        void exportPoint(std::vector<DetectionPoint> points)
        {
            for(const auto& point : points)
            {
                ofstream
                    << point.coords.UTMUPSRepresentation()
                    << " ALT: "
                    << point.altitude
                    << std::endl;
            }
        }

    private:
        std::ofstream ofstream;
    };
}

#endif
