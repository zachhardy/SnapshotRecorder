#include "snapshot_recorder.h"

#include "ChiConsole/chi_console.h"

#include "chi_log.h"
extern ChiLog& chi_log;

#include <iomanip>
#include <fstream>

//############################################################
/**Write a snapshot of the field-functions of interest to a file.*/
void SnapshotRecorder::WriteSnapshot(double time)
{
  chi_log.Log(LOG_ALLVERBOSE_1)
      << "Writing snapshot at time "
      << std::setprecision(8) << time;

  const bool initialize = time == 0.0;

  //======================================== Loop over field functions
  for (const auto& ff : field_functions)
  {
    std::string filename = output_directory + "/" + ff->text_name;
    std::ofstream file = OpenFile(filename, initialize);

    //============================== Loop over vector elements
    file << std::setprecision(8) << time << " ";
    for (auto& v : *ff->field_vector_local)
      file << std::setprecision(12) << v << " ";
    file << std::endl;
    file.close();
  }
}

//############################################################
/**Open an output file to write to.*/
std::ofstream SnapshotRecorder::
OpenFile(std::string& filename,
         bool initialize)
{
  std::ofstream file;
  if (initialize)
    file.open(filename, std::ofstream::trunc);
  else
    file.open(filename, std::ofstream::app);

  //check that file is open
  if (not file.is_open())
  {
    chi_log.Log(LOG_ALLERROR)
        << "Failed to open file " << filename << " to record snapshots.";
    exit(EXIT_FAILURE);
  }
  return file;
}

//############################################################
/**Static method to register the Lua macros to the console.*/
void SnapshotRecorder::RegisterLuaMacros()
{
  ChiConsole& console = ChiConsole::GetInstance();

  auto L = console.consoleState;

  #include "ChiMacros/lua_register_macro.h"
  #include "lua/lua_register.h"
}

