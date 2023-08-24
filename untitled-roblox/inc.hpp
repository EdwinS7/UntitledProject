#include <windows.h>
#include <cstdint>
#include <sstream>
#include <iomanip> 
#include <thread>
#include <vector>
#include <string>
#include <format>

#define CHUNK_NAME "Untitled_RBX"

#include "third-party/xorstr.hpp"
#include "third-party/ZStd/inc/zstd.h"
#include "third-party/XXHash/src/xxhash.h"

#include "third-party/LuaU/inc/lualib.h"
#include "third-party/LuaU/compiler/src/Compiler.h"
#include "third-party/LuaU/compiler/src/BytecodeBuilder.h"
#include "third-party/LuaU/compiler/src/luacode.h"

#include "include/util/enum.hpp"
#include "include/util/util.hpp"
#include "include/mem/memory.hpp"
#include "include/hooks/hooks.hpp"
#include "include/rbx/rbx.hpp"

#include "include/ctx/ctx.hpp"