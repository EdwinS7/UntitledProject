#pragma once

#include "../../common.hpp"

/* https://github.com/TheCruZ/Simple-Manual-Map-Injector/ */
using f_LoadLibraryA = HINSTANCE( WINAPI* )( const char* lpLibFilename );
using f_GetProcAddress = FARPROC( WINAPI* )( HMODULE hModule, LPCSTR lpProcName );
using f_DLL_ENTRY_POINT = BOOL( WINAPI* )( void* hDll, DWORD dwReason, void* pReserved );

#ifdef _WIN64
using f_RtlAddFunctionTable = BOOL( WINAPIV* )( PRUNTIME_FUNCTION FunctionTable, DWORD EntryCount, DWORD64 BaseAddress );
#endif

struct MANUAL_MAPPING_DATA
{
	f_LoadLibraryA pLoadLibraryA;
	f_GetProcAddress pGetProcAddress;
#ifdef _WIN64
	f_RtlAddFunctionTable pRtlAddFunctionTable;
#endif
	BYTE* pbase;
	HINSTANCE hMod;
	DWORD fdwReasonParam;
	LPVOID reservedParam;
	BOOL SEHSupport;
};

class CUtilities {
public:
	int InjectDLL( const char* process_name, uint8_t binary[] );
	int IsCorrectTargetArchitecture( HANDLE process );
	int GetProcessID( const char* process_name );
	int EscalateLocalPrivileges( );

private:
	const char* m_Software{ "MX Simulator" };

	int m_ActiveRegion, m_CurrentSelection;
	std::string m_Username, m_Password;
	bool m_SEHExceptionSupport;
};

inline const auto Utilities = std::make_unique<CUtilities>( );