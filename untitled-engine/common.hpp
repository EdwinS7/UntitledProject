#include <windows.h>
#include <thread>
#include <chrono>
#include <vector>
#include <format>

#define ALWAYS_INLINE __forceinline

// @note: import user settings.
#include "config.hpp"

#include "include/win32/win32.hpp"
#include "include/buffer/buffer.hpp"

#include "include/context/context.hpp"