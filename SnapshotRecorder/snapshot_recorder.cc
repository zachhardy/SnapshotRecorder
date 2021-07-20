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

  // TODO: Eventually, it would be nice to export each snapshot
  //       of each field function to VTK files. This would
  //       necessitate having separate directories for each field
  //       function and a naming convention for the separate output
  //       files from each time step.

  //======================================== Loop over field functions
  for (const auto& ff : field_functions)
  {
    std::string filename = output_directory + "/" +
                           ff->text_name + ".txt";
    std::ofstream file = OpenFile(filename, initialize);
    file << std::setprecision(8) << time << " ";

    const auto& grid = ff->grid;
    const auto& discretization = ff->spatial_discretization;
    const auto& vector = ff->field_vector_local;
    const auto& uk_man = ff->unknown_manager;
    const auto& unknown_id = ff->ref_variable;
    const auto& component_id = ff->ref_component;

    for (const auto& cell : grid->local_cells)
    {
      const auto& num_nodes = discretization->GetCellNumNodes(cell);

      for (int i = 0; i < num_nodes; ++i)
      {
        const size_t ir =
            discretization->MapDOFLocal(cell, i, uk_man, unknown_id, component_id);
        file << std::setprecision(12) << (*vector)[ir] << " ";
      }
    }
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

