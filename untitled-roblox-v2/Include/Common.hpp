#pragma once

#include <windows.h>
#include <thread>
#include <vector>
#include <string>

#define ENV "UntitledRobloxV2"
#define VERSION "1.00"

#define INLINE __forceinline
#define MIN(a, b) ((a) < (b) ? (a) : (b)
#define MAX(a, b) ((a) > (b) ? (a) : (b)
#define LOG(format, ...) printf(format "\n", __VA_ARGS__)
#define LOG_ERROR(format, ...) fprintf(stderr, format "\n", __VA_ARGS__)
#define IS_WINDOWS (defined(_WIN32) || defined(_WIN64))

#define PERFORMANCE_TIMER_START() std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now()
#define PERFORMANCE_TIMER_STOP() std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - start;

//Thirdparty
#include "../Thirdparty/xorstr.hpp"

//Include
#include "Memory/Memory.hpp"
#include "Context/Context.hpp"