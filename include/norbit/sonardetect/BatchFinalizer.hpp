#ifndef NORBIT_BATCH_FINALIZER
#define NORBIT_BATCH_FINALIZER

#include "DetectionPointBatch.hpp"
#include "FileExporter.hpp"

namespace norbit
{
    class BatchFinalizer
    {
    public:
        BatchFinalizer(const std::filesystem::path& outputPath);

        void finalize(std::unique_ptr<DetectionPointBatch>&& batch);

    private:
        FileExporter exporter;
    };
}
#endif
