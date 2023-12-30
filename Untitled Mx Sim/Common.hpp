#include <filesystem>
#include <windows.h>
#include <iostream>
#include <variant>
#include <fstream>
#include <thread>
#include <vector>
#include <string>
#include <format>

using namespace std::chrono_literals;

// Third party & Definitions
#include "Thirdparty/Xorstr.hpp"
#include "Thirdparty/MinHook/MinHook.h"
#include "Definitions.hpp"

// Local
#include "Include/Utilities/Utilities.hpp"
#include "Include/Memory/Memory.hpp"
#include "Include/Hooking/Hooks.hpp"
#include "Include/Context/Context.hpp"