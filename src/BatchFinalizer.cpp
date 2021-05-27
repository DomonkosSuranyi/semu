#include <norbit/sonardetect/BatchFinalizer.hpp>

using namespace norbit;

BatchFinalizer::BatchFinalizer(
        const std::filesystem::path& outputPath):
            exporter(FileExporter(outputPath))
{}

void BatchFinalizer::finalize(std::unique_ptr<DetectionPointBatch>&& batch)
{
    exporter.exportPoint(evaluateBatch(std::move(batch)));
}
