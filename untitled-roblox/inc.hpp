#include <windows.h>
#include <cstdint>
#include <sstream>
#include <iomanip> 
#include <fstream>
#include <random>
#include <thread>
#include <vector>
#include <string>
#include <format>
#include <array>

#include "third-party/xorstr.hpp"
#include "third-party/ZStd/inc/zstd.h"
#include "third-party/XXHash/src/xxhash.h"
#include "third-party/MinHook/MinHook.h"

#include "third-party/LuaU/inc/lualib.h"
#include "third-party/LuaU/Compiler/src/Compiler.h"
#include "third-party/LuaU/Compiler/src/BytecodeBuilder.h"
#include "third-party/LuaU/Compiler/src/luacode.h"

#define ENVIORNMENT_NAME xorstr_( "UntitledProject" )

#include "include/util/enum.hpp"
#include "include/util/util.hpp"
#include "include/mem/memory.hpp"
#include "include/hooks/hooks.hpp"
#include "include/rbx/rbx.hpp"

//@note: used for communication between external client and internal (this).
#include "include/bridge/bridge.hpp"

#include "include/ctx/ctx.hpp"