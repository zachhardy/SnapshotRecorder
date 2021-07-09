#include "chi_runtime.h"
#include "SnapshotRecorder/snapshot_recorder.h"


int main(int argc, char** argv)
{
    ChiTech::Initialize(argc,argv);

    SnapshotRecorder::RegisterLuaMacros();

    ChiTech::RunBatch(argc, argv);

    ChiTech::Finalize();
    return 0;
}
