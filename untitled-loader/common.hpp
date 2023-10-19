#include <d3dcompiler.h>
#include <TlHelp32.h>
#include <windows.h>
#include <iostream>
#include <d3d11.h>
#include <thread>
#include <vector>

#pragma comment(lib, "d3dcompiler")

using namespace std::chrono_literals;

#define REGION_NAME "UntitledLoader"
#define DEVELOPER_BUILD 1
#define APP_VERSION 1.00

#define MANUAL_USER "admin"
#define MANUAL_PASSWORD "admin"

#define INLINE __forceinline
#define MIN(a, b) ((a) < (b) ? (a) : (b)
#define MAX(a, b) ((a) > (b) ? (a) : (b)
#define LOG(format, ...) printf(format "\n", __VA_ARGS__)
#define LOG_ERROR(format, ...) fprintf(stderr, format "\n", __VA_ARGS__)
#define IS_WINDOWS (defined(_WIN32) || defined(_WIN64))

#define PERFORMANCE_TIMER_START() std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now()
#define PERFORMANCE_TIMER_STOP() std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - start;

// Cheat modules are stored here.
#include "binary.hpp"

#include "third-party/imgui/imgui.h"
#include "third-party/imgui/imgui_stdlib.h"
#include "third-party/imgui/backend/imgui_impl_dx11.h"
#include "third-party/imgui/backend/imgui_impl_win32.h"

#include "include/win32/win32.hpp"
#include "include/graphics/graphics.hpp"
#include "include/utilities/utilities.hpp"
#include "third-party/imgui/wrapper/wrapper.hpp"
#include "include/context/context.hpp"