#include <windows.h>
#include <thread>
#include <string>
#include <vector>

// @note: DirectX 11
#include <d3d11.h>

#include "third-party/xorstr.hpp"
#include "third-party/MinHook/MinHook.h"

#include "include/memory/memory.hpp"
#include "include/hooks/hooks.hpp"

// @note: entry_point ( DllMain )
#include "include/context/context.hpp"