#pragma once

#define UNTITLED_SHOW_STATS

#define MIN(a, b) ((a) < (b) ? (a) : (b)
#define MAX(a, b) ((a) > (b) ? (a) : (b)

#define IS_WINDOWS (defined(_WIN32) || defined(_WIN64))

#define LOG(format, ...) std::cout << ( format, __VA_ARGS__ ) << std::endl;
#define LOG_ERROR(format, ...) std::cout << stderr << ( format, __VA_ARGS__ ) << std::endl;

#define FOLDER_CONTENT std::vector<std::tuple<std::string, std::string>>

#define CALL_FUNCTION(Address, ...) reinterpret_cast<int(*)(void)>(Address)(__VA_ARGS__)

#define PERFORMANCE_TIMER_START() std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now()
#define PERFORMANCE_TIMER_STOP() std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - start;

#define PE32 0x400000
#define PE64 0x140000000

#if _WIN64
#define PE 0x140000000
#define Win64
#else
#define PE 0x400000
#define Win32
#endif