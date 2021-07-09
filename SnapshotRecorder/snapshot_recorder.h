#ifndef SNAPSHOT_RECORDER_H
#define SNAPSHOT_RECORDER_H

#include "ChiPhysics/FieldFunction/fieldfunction.h"
#include <string>

typedef chi_physics::FieldFunction FF;

//############################################################
/**Snapshot recorder for writing outputs to files.*/
class SnapshotRecorder
{
public:
  std::string output_directory;
  std::vector<std::shared_ptr<FF>> field_functions;

public:
  static void RegisterLuaMacros();

  void WriteSnapshot(double time, size_t n_step);
  void WriteTime(double time, size_t n_step) const;
  void WriteGrid();

private:
  static std::ofstream OpenFile(std::stringstream& filename,
                                size_t n_step);

};

#endif //SNAPSHOT_RECORDER_H
