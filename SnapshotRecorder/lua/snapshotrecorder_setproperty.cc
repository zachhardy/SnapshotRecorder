#include "ChiLua/chi_lua.h"

#include "chi_log.h"
extern ChiLog& chi_log;

#include "ChiPhysics/chi_physics.h"
extern ChiPhysics&  chi_physics_handler;

#include "../snapshot_recorder.h"
extern SnapshotRecorder snapshot_recorder;

//############################################################
int chiSnapshotRecorderAddFieldFunction(lua_State* L)
{
  int num_args = lua_gettop(L);

  if (num_args != 1)
    LuaPostArgAmountError(__FUNCTION__, 1, num_args);
  LuaCheckNilValue(__FUNCTION__ , L, 1);

  int ff_index = lua_tointeger(L, 1);

  std::shared_ptr<chi_physics::FieldFunction> ff;
  try
  {
    ff = chi_physics_handler.fieldfunc_stack.at(ff_index);
  }
  catch (const std::out_of_range& o)
  {
    chi_log.Log(LOG_ALLERROR)
        << __FUNCTION__ << ": Invalid field function handle.";
    exit(EXIT_FAILURE);
  }

  snapshot_recorder.field_functions.push_back(ff);
  chi_log.Log(LOG_0)
      << __FUNCTION__ << ": Field function with index "
      << ff_index << " added to SnapshotRecorder.";
  return 0;
}
