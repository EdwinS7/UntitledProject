#include <filesystem>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <variant>
#include <thread>
#include <vector>
#include <regex>
#include <map>

using namespace std::chrono_literals;

#define DEV

// 0 = RESTRICTED, 1 = HIGH RESTRICTIONS, 2 = LOW RESTRICTIONS, 3 = NONE
#define LEVEL 3

// Includes
#include "Utilities/Utilities.hpp"

#include "Interpreter/Types.hpp"
#include "Tokenizer/Tokenizer.hpp"
#include "Interpreter/Functions/Functions.hpp"
#include "Interpreter/Memory/Memory.hpp"
#include "Interpreter/Interpreter.hpp"

#include "Context/Context.hpp"