#include <filesystem>
#include <windows.h>
#include <iostream>
#include <variant>
#include <fstream>
#include <thread>
#include <vector>
#include <string>
#include <format>

// Third party & Definitions
#include "Thirdparty/Xorstr.hpp"
#include "Thirdparty/MinHook/MinHook.h"
#include "Definitions.hpp"

// Local
#include "Include/Utilities/Utilities.hpp"
#include "Include/Console/Console.hpp"
#include "Include/Memory/Memory.hpp"
#include "Include/Hooks/Hooks.hpp"

#include "Include/Context/Context.hpp"