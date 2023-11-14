#include "Context.hpp"

#define FIX_IMPORT( Base, Offset, Module, Function ) *reinterpret_cast<FARPROC*>( Base + Offset ) = GetProcAddress( LoadLibraryA( Module ), Function );
#define CALL_FUNCTION( Address ) reinterpret_cast<int( * )( void )>( Address )( )

void CContext::Init( ) {
	AllocateConsole( );
	
	void* CheatAllocatedMemory = VirtualAlloc( 0, sizeof( CheatBin ), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE );
	if ( !CheatAllocatedMemory ) {
		LOG_ERROR( "Failed to allocate cheat memory!" );
		return;
	}

	void* StubAllocatedMemory = VirtualAlloc( 0, sizeof( CheatBin ), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE );
	if ( !StubAllocatedMemory ) {
		LOG_ERROR( "Failed to allocate stub memory!" );
		return;
	}

	LOG( "Cheat allocated: {}\nStub allocated: {}", CheatAllocatedMemory, StubAllocatedMemory );

	memcpy( CheatAllocatedMemory, CheatBin, sizeof( CheatBin ) );
	memcpy( StubAllocatedMemory, StubBin, sizeof( StubBin ) );

	LOG( "Waiting for {}...", LAST_MODULE_LOADED );

	while ( !GetModuleHandleA( LAST_MODULE_LOADED ) )
		std::this_thread::sleep_for( 200ms );

	LOG( "Fixing relocations" );
	FixRelocations( reinterpret_cast< uint32_t >( CheatAllocatedMemory ), reinterpret_cast< uint32_t >( StubAllocatedMemory ) );
	LOG( "Fixed relocations" );

	LOG( "Fixing imports" );
	FixImports( reinterpret_cast< uint32_t >( CheatAllocatedMemory ) + 0x226000 ); //0x226000 RVA
	LOG( "Fixed imports" );

	__asm
	{
		push 0
		push 1
		push cheat_mem
		mov eax, cheat_mem
		add eax, 0x159A16 //0x159A16 RVA
		call eax
	}

	LOG( "Calling DLLMain: {}", reinterpret_cast< uint32_t >( CheatAllocatedMemory ) + 0x159A16 );
	CALL_FUNCTION( reinterpret_cast< uint32_t >( CheatAllocatedMemory ) + 0x159A16 );
	LOG( "Called DLLMain" );
}

void CContext::FixImports( std::uintptr_t RVA ) {
	FIX_IMPORT( RVA, 0x0, "gdi32.dll", "AddFontResourceA" );
	FIX_IMPORT( RVA, 0x8, "imm32.dll", "ImmSetCompositionWindow" );
	FIX_IMPORT( RVA, 0xC, "imm32.dll", "ImmReleaseContext" );
	FIX_IMPORT( RVA, 0x10, "imm32.dll", "ImmGetContext" );
	FIX_IMPORT( RVA, 0x18, "kernel32.dll", "ExitThread" );
	FIX_IMPORT( RVA, 0x1C, "kernel32.dll", "VirtualQuery" );
	FIX_IMPORT( RVA, 0x20, "kernel32.dll", "GetModuleHandleA" );
	FIX_IMPORT( RVA, 0x24, "kernel32.dll", "WideCharToMultiByte" );
	FIX_IMPORT( RVA, 0x28, "kernel32.dll", "MultiByteToWideChar" );
	FIX_IMPORT( RVA, 0x2C, "kernel32.dll", "Sleep" );
	FIX_IMPORT( RVA, 0x30, "kernel32.dll", "GlobalAlloc" );
	FIX_IMPORT( RVA, 0x34, "kernel32.dll", "GlobalFree" );
	FIX_IMPORT( RVA, 0x38, "kernel32.dll", "GlobalLock" );
	FIX_IMPORT( RVA, 0x3C, "kernel32.dll", "GlobalUnlock" );
	FIX_IMPORT( RVA, 0x40, "kernel32.dll", "QueryPerformanceFrequency" );
	FIX_IMPORT( RVA, 0x44, "kernel32.dll", "QueryPerformanceCounter" );
	FIX_IMPORT( RVA, 0x48, "kernel32.dll", "GetPrivateProfileSectionA" );
	FIX_IMPORT( RVA, 0x4C, "kernel32.dll", "WritePrivateProfileStringA" );
	FIX_IMPORT( RVA, 0x50, "kernel32.dll", "CreateDirectoryA" );
	FIX_IMPORT( RVA, 0x54, "kernel32.dll", "EnterCriticalSection" );
	FIX_IMPORT( RVA, 0x58, "kernel32.dll", "LeaveCriticalSection" );
	FIX_IMPORT( RVA, 0x5C, "kernel32.dll", "InitializeCriticalSectionAndSpinCount" );
	FIX_IMPORT( RVA, 0x60, "kernel32.dll", "DeleteCriticalSection" );
	FIX_IMPORT( RVA, 0x64, "kernel32.dll", "SetEvent" );
	FIX_IMPORT( RVA, 0x68, "kernel32.dll", "ResetEvent" );
	FIX_IMPORT( RVA, 0x6C, "kernel32.dll", "WaitForSingleObjectEx" );
	FIX_IMPORT( RVA, 0x70, "kernel32.dll", "CreateEventW" );
	FIX_IMPORT( RVA, 0x74, "kernel32.dll", "GetModuleHandleW" );
	FIX_IMPORT( RVA, 0x78, "kernel32.dll", "GetProcAddress" );
	FIX_IMPORT( RVA, 0x7C, "kernel32.dll", "UnhandledExceptionFilter" );
	FIX_IMPORT( RVA, 0x80, "kernel32.dll", "SetUnhandledExceptionFilter" );
	FIX_IMPORT( RVA, 0x84, "kernel32.dll", "GetCurrentProcess" );
	FIX_IMPORT( RVA, 0x88, "kernel32.dll", "TerminateProcess" );
	FIX_IMPORT( RVA, 0x8C, "kernel32.dll", "IsProcessorFeaturePresent" );
	FIX_IMPORT( RVA, 0x90, "kernel32.dll", "IsDebuggerPresent" );
	FIX_IMPORT( RVA, 0x94, "kernel32.dll", "FreeLibraryAndExitThread" );
	FIX_IMPORT( RVA, 0x98, "kernel32.dll", "GetCurrentProcessId" );
	FIX_IMPORT( RVA, 0x9C, "kernel32.dll", "GetCurrentThreadId" );
	FIX_IMPORT( RVA, 0xA0, "kernel32.dll", "GetSystemTimeAsFileTime" );
	FIX_IMPORT( RVA, 0xA4, "kernel32.dll", "InitializeSListHead" );
	FIX_IMPORT( RVA, 0xA8, "kernel32.dll", "InitializeCriticalSectionEx" );
	FIX_IMPORT( RVA, 0xAC, "kernel32.dll", "CreateThread" );
	FIX_IMPORT( RVA, 0xB0, "kernel32.dll", "CloseHandle" );
	FIX_IMPORT( RVA, 0xB4, "kernel32.dll", "DisableThreadLibraryCalls" );
	FIX_IMPORT( RVA, 0xB8, "kernel32.dll", "SetLastError" );
	FIX_IMPORT( RVA, 0xBC, "kernel32.dll", "VirtualProtect" );
	FIX_IMPORT( RVA, 0xC0, "kernel32.dll", "EncodePointer" );
	FIX_IMPORT( RVA, 0xC4, "kernel32.dll", "DecodePointer" );
	FIX_IMPORT( RVA, 0xC8, "kernel32.dll", "GetLocaleInfoEx" );
	FIX_IMPORT( RVA, 0xCC, "kernel32.dll", "LCMapStringEx" );
	FIX_IMPORT( RVA, 0xD0, "kernel32.dll", "GetStringTypeW" );
	FIX_IMPORT( RVA, 0xD4, "kernel32.dll", "CompareStringEx" );
	FIX_IMPORT( RVA, 0xD8, "kernel32.dll", "GetCPInfo" );
	FIX_IMPORT( RVA, 0xDC, "kernel32.dll", "RaiseException" );
	FIX_IMPORT( RVA, 0xE0, "kernel32.dll", "GetLastError" );
	FIX_IMPORT( RVA, 0xE4, "kernel32.dll", "RtlUnwindEx" );
	FIX_IMPORT( RVA, 0xE8, "shlwapi.dll", "PathFileExistsW" );
	FIX_IMPORT( RVA, 0xEC, "shlwapi.dll", "PathCombineW" );
	FIX_IMPORT( RVA, 0xF0, "user32.dll", "CharNextW" );
	FIX_IMPORT( RVA, 0xF4, "user32.dll", "MessageBeep" );
	FIX_IMPORT( RVA, 0xF8, "user32.dll", "LoadStringW" );
	FIX_IMPORT( RVA, 0xFC, "user32.dll", "MessageBoxW" );
	FIX_IMPORT( RVA, 0x100, "user32.dll", "FindWindowW" );
	FIX_IMPORT( RVA, 0x104, "user32.dll", "GetWindowRect" );
	FIX_IMPORT( RVA, 0x108, "user32.dll", "GetClientRect" );
	FIX_IMPORT( RVA, 0x10C, "user32.dll", "AdjustWindowRectEx" );
	FIX_IMPORT( RVA, 0x110, "user32.dll", "GetDC" );
	FIX_IMPORT( RVA, 0x114, "user32.dll", "ReleaseDC" );
	FIX_IMPORT( RVA, 0x118, "user32.dll", "DefWindowProcW" );
	FIX_IMPORT( RVA, 0x11C, "user32.dll", "PostQuitMessage" );
	FIX_IMPORT( RVA, 0x120, "user32.dll", "RegisterClassExW" );
	FIX_IMPORT( RVA, 0x124, "user32.dll", "CreateWindowExW" );
	FIX_IMPORT( RVA, 0x128, "user32.dll", "DestroyWindow" );
	FIX_IMPORT( RVA, 0x12C, "user32.dll", "ShowWindow" );
	FIX_IMPORT( RVA, 0x130, "user32.dll", "UpdateWindow" );
	FIX_IMPORT( RVA, 0x134, "user32.dll", "GetMessageW" );
	FIX_IMPORT( RVA, 0x138, "user32.dll", "TranslateMessage" );
	FIX_IMPORT( RVA, 0x13C, "user32.dll", "DispatchMessageW" );
	FIX_IMPORT( RVA, 0x140, "user32.dll", "LoadCursorW" );
	FIX_IMPORT( RVA, 0x144, "user32.dll", "LoadIconW" );
	FIX_IMPORT( RVA, 0x148, "user32.dll", "SetCursor" );
	FIX_IMPORT( RVA, 0x14C, "user32.dll", "GetSystemMetrics" );
	FIX_IMPORT( RVA, 0x150, "user32.dll", "GetClientRect" );
	FIX_IMPORT( RVA, 0x154, "user32.dll", "FillRect" );
	FIX_IMPORT( RVA, 0x158, "user32.dll", "DrawTextW" );
	FIX_IMPORT( RVA, 0x15C, "user32.dll", "GetClipboardData" );
	FIX_IMPORT( RVA, 0x1C8, "ucrtbase.dll", "strtol" );
	FIX_IMPORT( RVA, 0x1CC, "ucrtbase.dll", "strtof" );
	FIX_IMPORT( RVA, 0x1D0, "ucrtbase.dll", "strtod" );
	FIX_IMPORT( RVA, 0x1D4, "ucrtbase.dll", "atof" );
	FIX_IMPORT( RVA, 0x1D8, "ucrtbase.dll", "atoi" );
	FIX_IMPORT( RVA, 0x1DC, "ucrtbase.dll", "_wtof" );
	FIX_IMPORT( RVA, 0x1E0, "ucrtbase.dll", "_wtoi" );
	FIX_IMPORT( RVA, 0x1E4, "ucrtbase.dll", "strtoul" );
	FIX_IMPORT( RVA, 0x1E8, "ucrtbase.dll", "_itoa" );
	FIX_IMPORT( RVA, 0x1F0, "ucrtbase.dll", "getenv" );
	FIX_IMPORT( RVA, 0x1F8, "ucrtbase.dll", "free" );
	FIX_IMPORT( RVA, 0x1FC, "ucrtbase.dll", "calloc" );
	FIX_IMPORT( RVA, 0x200, "ucrtbase.dll", "realloc" );
	FIX_IMPORT( RVA, 0x204, "ucrtbase.dll", "malloc" );
	FIX_IMPORT( RVA, 0x208, "ucrtbase.dll", "_callnewh" );
	FIX_IMPORT( RVA, 0x210, "ucrtbase.dll", "_lock_locales" );
	FIX_IMPORT( RVA, 0x214, "ucrtbase.dll", "___lc_codepage_func" );
	FIX_IMPORT( RVA, 0x218, "ucrtbase.dll", "___lc_locale_name_func" );
	FIX_IMPORT( RVA, 0x21C, "ucrtbase.dll", "_unlock_locales" );
	FIX_IMPORT( RVA, 0x220, "ucrtbase.dll", "___lc_collate_cp_func" );
	FIX_IMPORT( RVA, 0x224, "ucrtbase.dll", "__pctype_func" );
	FIX_IMPORT( RVA, 0x228, "ucrtbase.dll", "setlocale" );
	FIX_IMPORT( RVA, 0x22C, "ucrtbase.dll", "localeconv" );
	FIX_IMPORT( RVA, 0x230, "ucrtbase.dll", "___mb_cur_max_func" );
	FIX_IMPORT( RVA, 0x238, "ucrtbase.dll", "frexp" );
	FIX_IMPORT( RVA, 0x23C, "ucrtbase.dll", "_libm_sse2_sqrt_precise" );
	FIX_IMPORT( RVA, 0x240, "ucrtbase.dll", "_libm_sse2_sin_precise" );
	FIX_IMPORT( RVA, 0x244, "ucrtbase.dll", "_libm_sse2_pow_precise" );
	FIX_IMPORT( RVA, 0x248, "ucrtbase.dll", "_libm_sse2_log_precise" );
	FIX_IMPORT( RVA, 0x24C, "ucrtbase.dll", "_libm_sse2_exp_precise" );
	FIX_IMPORT( RVA, 0x250, "ucrtbase.dll", "_libm_sse2_cos_precise" );
	FIX_IMPORT( RVA, 0x254, "ucrtbase.dll", "fminf" );
	FIX_IMPORT( RVA, 0x258, "ucrtbase.dll", "_CIpow" );
	FIX_IMPORT( RVA, 0x25C, "ucrtbase.dll", "round" );
	FIX_IMPORT( RVA, 0x260, "ucrtbase.dll", "fmaxf" );
	FIX_IMPORT( RVA, 0x264, "ucrtbase.dll", "floor" );
	FIX_IMPORT( RVA, 0x268, "ucrtbase.dll", "remainderf" );
	FIX_IMPORT( RVA, 0x26C, "ucrtbase.dll", "ldexp" );
	FIX_IMPORT( RVA, 0x270, "ucrtbase.dll", "_CIatan2" );
	FIX_IMPORT( RVA, 0x274, "ucrtbase.dll", "_libm_sse2_atan_precise" );
	FIX_IMPORT( RVA, 0x278, "ucrtbase.dll", "_CIfmod" );
	FIX_IMPORT( RVA, 0x27C, "ucrtbase.dll", "ceil" );
	FIX_IMPORT( RVA, 0x280, "ucrtbase.dll", "_libm_sse2_asin_precise" );
	FIX_IMPORT( RVA, 0x284, "ucrtbase.dll", "_libm_sse2_tan_precise" );
	FIX_IMPORT( RVA, 0x288, "ucrtbase.dll", "_fdclass" );
	FIX_IMPORT( RVA, 0x28C, "ucrtbase.dll", "roundf" );
	FIX_IMPORT( RVA, 0x294, "ucrtbase.dll", "_initialize_onexit_table" );
	FIX_IMPORT( RVA, 0x298, "ucrtbase.dll", "_seh_filter_dll" );
	FIX_IMPORT( RVA, 0x29C, "ucrtbase.dll", "_invalid_parameter_noinfo_noreturn" );
	FIX_IMPORT( RVA, 0x2A0, "ucrtbase.dll", "_configure_narrow_argv" );
	FIX_IMPORT( RVA, 0x2A4, "ucrtbase.dll", "_initialize_narrow_environment" );
	FIX_IMPORT( RVA, 0x2A8, "ucrtbase.dll", "_errno" );
	FIX_IMPORT( RVA, 0x2AC, "ucrtbase.dll", "_register_onexit_function" );
	FIX_IMPORT( RVA, 0x2B0, "ucrtbase.dll", "_execute_onexit_table" );
	FIX_IMPORT( RVA, 0x2B4, "ucrtbase.dll", "abort" );
	FIX_IMPORT( RVA, 0x2B8, "ucrtbase.dll", "_crt_atexit" );
	FIX_IMPORT( RVA, 0x2BC, "ucrtbase.dll", "_cexit" );
	FIX_IMPORT( RVA, 0x2C0, "ucrtbase.dll", "_initterm" );
	FIX_IMPORT( RVA, 0x2C4, "ucrtbase.dll", "terminate" );
	FIX_IMPORT( RVA, 0x2C8, "ucrtbase.dll", "_initterm_e" );
	FIX_IMPORT( RVA, 0x2D0, "ucrtbase.dll", "_wfopen" );
	FIX_IMPORT( RVA, 0x2D4, "ucrtbase.dll", "fread" );
	FIX_IMPORT( RVA, 0x2D8, "ucrtbase.dll", "__stdio_common_vfprintf" );
	FIX_IMPORT( RVA, 0x2DC, "ucrtbase.dll", "fopen" );
	FIX_IMPORT( RVA, 0x2E0, "ucrtbase.dll", "fclose" );
	FIX_IMPORT( RVA, 0x2E4, "ucrtbase.dll", "fflush" );
	FIX_IMPORT( RVA, 0x2E8, "ucrtbase.dll", "ftell" );
	FIX_IMPORT( RVA, 0x2EC, "ucrtbase.dll", "__stdio_common_vsprintf" );
	FIX_IMPORT( RVA, 0x2F0, "ucrtbase.dll", "__stdio_common_vsprintf_s" );
	FIX_IMPORT( RVA, 0x2F4, "ucrtbase.dll", "fwrite" );
	FIX_IMPORT( RVA, 0x2F8, "ucrtbase.dll", "fseek" );
	FIX_IMPORT( RVA, 0x2FC, "ucrtbase.dll", "__stdio_common_vsscanf" );
	FIX_IMPORT( RVA, 0x304, "ucrtbase.dll", "strncpy_s" );
	FIX_IMPORT( RVA, 0x308, "ucrtbase.dll", "strtok" );
	FIX_IMPORT( RVA, 0x30C, "ucrtbase.dll", "strcpy_s" );
	FIX_IMPORT( RVA, 0x310, "ucrtbase.dll", "_mbsdup" );
	FIX_IMPORT( RVA, 0x314, "ucrtbase.dll", "strcat" );
	FIX_IMPORT( RVA, 0x318, "ucrtbase.dll", "strcmp" );
	FIX_IMPORT( RVA, 0x31C, "ucrtbase.dll", "strlen" );
	FIX_IMPORT( RVA, 0x320, "ucrtbase.dll", "strncmp" );
	FIX_IMPORT( RVA, 0x324, "ucrtbase.dll", "strcpy" );
	FIX_IMPORT( RVA, 0x328, "ucrtbase.dll", "isupper" );
	FIX_IMPORT( RVA, 0x32C, "ucrtbase.dll", "_wcsdup" );
	FIX_IMPORT( RVA, 0x330, "ucrtbase.dll", "islower" );
	FIX_IMPORT( RVA, 0x334, "ucrtbase.dll", "__strncnt" );
	FIX_IMPORT( RVA, 0x338, "ucrtbase.dll", "toupper" );
	FIX_IMPORT( RVA, 0x33C, "ucrtbase.dll", "strcspn" );
	FIX_IMPORT( RVA, 0x340, "ucrtbase.dll", "_stricmp" );
	FIX_IMPORT( RVA, 0x344, "ucrtbase.dll", "tolower" );
	FIX_IMPORT( RVA, 0x348, "ucrtbase.dll", "isspace" );
	FIX_IMPORT( RVA, 0x34C, "ucrtbase.dll", "isxdigit" );
	FIX_IMPORT( RVA, 0x350, "ucrtbase.dll", "wcsnlen" );
	FIX_IMPORT( RVA, 0x354, "ucrtbase.dll", "strcat_s" );
	FIX_IMPORT( RVA, 0x358, "ucrtbase.dll", "strncpy" );
	FIX_IMPORT( RVA, 0x360, "ucrtbase.dll", "_Getdays" );
	FIX_IMPORT( RVA, 0x364, "ucrtbase.dll", "_W_Getmonths" );
	FIX_IMPORT( RVA, 0x368, "ucrtbase.dll", "_Strftime" );
	FIX_IMPORT( RVA, 0x36C, "ucrtbase.dll", "_Gettnames" );
	FIX_IMPORT( RVA, 0x370, "ucrtbase.dll", "_Wcsftime" );
	FIX_IMPORT( RVA, 0x374, "ucrtbase.dll", "_W_Gettnames" );
	FIX_IMPORT( RVA, 0x378, "ucrtbase.dll", "_W_Getdays" );
	FIX_IMPORT( RVA, 0x37C, "ucrtbase.dll", "_Getmonths" );
	FIX_IMPORT( RVA, 0x384, "ucrtbase.dll", "qsort" );
	FIX_IMPORT( RVA, 0x388, "ucrtbase.dll", "rand" );
	FIX_IMPORT( RVA, 0x390, "urlmon.dll", "URLDownloadToFileA" );
}

void CContext::FixRelocations( std::uintptr_t Cheat, std::uintptr_t Stub ) {
	std::uint32_t Delta = Cheat - 0x36DA0000;

	for ( uint32_t i = 0; i < CheatRelocations.size( ); i++ ) // Relocate the cheat.
		*reinterpret_cast< uint32_t* >( Cheat + CheatRelocations[ i ] ) += Delta;

	Delta = Stub - 0x370D0000;

	for ( uint32_t i = 0; i < StubRelocations.size( ); i++ ) //relocate the stub
		*reinterpret_cast< uint32_t* >( Stub + StubRelocations[ i ] ) += Delta;
}

void CContext::AllocateConsole( ) {
	if ( !AllocConsole( ) )
		return;

	SetConsoleTitle( NAME );
	freopen( "CONOUT$", "w", stdout );
}