#include <filesystem>
#include <windows.h>
#include <iostream>
#include <variant>
#include <fstream>
#include <thread>
#include <vector>
#include <string>
#include <format>

#define KIERO_INCLUDE_OPENGL

// Third party & Definitions
#include <glfw3.h>
#include "Thirdparty/Xorstr.hpp"
#include "Thirdparty/MinHook/MinHook.h"
#include "Thirdparty/KieroHook/imgui_hook.h"
#include "Thirdparty/ImGui/ImGui.h"

#include "Definitions.hpp"

// Local
#include "Include/Console/Console.hpp"
#include "Include/Memory/Memory.hpp"
#include "Include/Hooks/Hooks.hpp"

#include "Include/Interface/Interface.hpp"

#include "Include/Context/Context.hpp"