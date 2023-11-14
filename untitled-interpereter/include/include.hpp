#include <filesystem>
#include <functional>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <variant>
#include <thread>
#include <vector>
#include <regex>
#include <map>

using namespace std::chrono_literals;

#define ENVIORNMENT "UntitledAPI"
#define VERSION "1.00"

#define INLINE __forceinline
#define MIN(a, b) ((a) < (b) ? (a) : (b)
#define MAX(a, b) ((a) > (b) ? (a) : (b)
#define BOLD(word) "\x1b[1m"+word+"\x1b[0m"
#define IS_WINDOWS (defined(_WIN32) || defined(_WIN64))
#define CALL_FUNCTION(Address, ...) reinterpret_cast<int(*)(void)>(Address)(__VA_ARGS__)
#define PERFORMANCE_TIMER_START() std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now()
#define PERFORMANCE_TIMER_STOP() std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - start;

#define FOLDER_CONTENT std::vector<std::tuple<std::string, std::string>>

// 0 = RESTRICTED, 1 = HIGH RESTRICTIONS, 2 = LOW RESTRICTIONS, 3 = NONE
#define LEVEL 3

// Error codes.
#define VARIABLE_UNDEFINED 105238212

// Includes
#include "Utilities/Utilities.hpp"

#include "Interpreter/Types.hpp"
#include "Tokenizer/Tokenizer.hpp"
#include "Interpreter/Memory/Memory.hpp"
#include "Interpreter/Functions/Functions.hpp"
#include "Interpreter/Interpreter.hpp"

#include "Context/Context.hpp"