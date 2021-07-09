#include "snapshot_recorder.h"

#include "ChiMath/SpatialDiscretization/FiniteElement/PiecewiseLinear/pwl.h"

#include "ChiConsole/chi_console.h"

#include "chi_log.h"
extern ChiLog& chi_log;

#include <iomanip>
#include <fstream>

typedef SpatialDiscretization_PWLD PWLD;

//############################################################
/**Write a snapshot of the field-functions of interest to a file.*/
void SnapshotRecorder::WriteSnapshot(double time, size_t n_step)
{
  chi_log.Log(LOG_0)
      << "Writing snapshot at output step "
      << n_step << " and time "
      << std::setprecision(8) << time << ".";

  WriteTime(time, n_step); //write the time

  //TODO: This should be based on the field function name,
  //      not hardcoded to use the scalar flux.
  //open the output file
  std::stringstream filename;
  filename << output_directory << "/scalar_flux.txt";
  std::ofstream outfile = OpenFile(filename, n_step);

  //TODO: This is not really good for multiple field functions.
  //      This prints everything on one line, leaving the user
  //      to know where one field function vector ends and another
  //      begins. This needs to be more thoroughly thought out.

  //======================================== Loop over field functions
  for (const auto &ff : field_functions)
  {
    //============================== Loop over vector elements
    for (auto &v : *ff->field_vector_local)
      outfile << v << " ";
  }
  outfile.close();
}


//############################################################
/**Write the simulation time to a file. This appends to the
 * `times.txt` file each time the method is called.
 */
void SnapshotRecorder::WriteTime(double time, size_t n_step) const
{
  //output filename
  std::stringstream filename;
  filename << output_directory << "/times.txt";

  //open file, write time, and close
  std::ofstream outfile = OpenFile(filename, n_step);
  outfile << time;
  outfile.close();
}


//############################################################
/**Write the spatial grid to a file. For static grids, this is
 * only necessary at the start of a simulation.*/
void SnapshotRecorder::WriteGrid()
{
  chi_log.Log(LOG_0) << "Writing the grid points.";

  //open the output file
  std::stringstream filename;
  filename << output_directory << "/grid.txt";
  std::ofstream  outfile = OpenFile(filename, 0);

  //get a discretization
  const auto ff = field_functions[0];
  const auto grid_fe_view =
      std::dynamic_pointer_cast<PWLD>(ff->spatial_discretization);

  //======================================== Loop over cells
  for (const auto& cell : ff->grid->local_cells)
  {
    const auto cell_fe_mapping = grid_fe_view->GetCellMappingFE(cell.local_id);

    //======================================== Loop over nodes
    const int num_nodes = cell_fe_mapping->num_nodes;
    for (int i = 0; i < num_nodes; ++i)
    {
      const chi_mesh::Vertex& v = ff->grid->vertices[cell.vertex_ids[i]];

      // Write vertex to file
      outfile << v.x << " " << v.y << " " << v.z << "\n";
    }
  }
  outfile.close();
}


//############################################################
/**Open an output file to write to.*/
std::ofstream SnapshotRecorder::
OpenFile(std::stringstream& filename, size_t n_step)
{
  std::ofstream outfile;
  if (n_step == 0) outfile.open(filename.str());
  else
  {
    //Open the file to append to it
    outfile.open(filename.str(), std::ofstream::out |
                                 std::ofstream::app);
    outfile << "\n";
  }

  //check that file is open
  if (not outfile.is_open())
  {
    chi_log.Log(LOG_ALLERROR)
        << "Failed to open file " << filename.str()
        << " to record snapshots.";
    exit(EXIT_FAILURE);
  }
  return outfile;
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

