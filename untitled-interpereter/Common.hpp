#include <filesystem>
#include <functional>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <d3d11.h>
#include <variant>
#include <thread>
#include <vector>
#include <regex>
#include <map>

#pragma comment(lib, "d3dcompiler")
using namespace std::chrono_literals;

// Settings
#define ENVIORNMENT "UntitledAPI"

// Shortcuts
#define IS_WINDOWS (defined(_WIN32) || defined(_WIN64))
#define FOLDER_CONTENT std::vector<std::tuple<std::string, std::string>>
#define CALL_FUNCTION(Address, ...) reinterpret_cast<int(*)(void)>(Address)(__VA_ARGS__)
#define PERFORMANCE_TIMER_START() std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now()
#define PERFORMANCE_TIMER_STOP() std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - start;

// Third party
#include "Thirdparty/ImGui/ImGui.h"
#include "Thirdparty/ImGui/ImGui_STDLib.h"
#include "Thirdparty/ImGui/ImGui_TextEditor.h"
#include "Thirdparty/ImGui/Backend/ImGui_Impl_Dx11.h"
#include "Thirdparty/ImGui/Backend/ImGui_Impl_Win32.h"

// Includes
#include "Include/Window/Window.hpp"
#include "Include/Window/Graphics.hpp"
#include "Thirdparty/ImGui/Wrapper/Wrapper.hpp"

#include "Include/Definitions.hpp"

#include "Include/Utilities/Utilities.hpp"

#include "Include/Lexer/Lexer.hpp"
#include "Include/Interpreter/Interpreter.hpp"

#include "Include/Context/Context.hpp"