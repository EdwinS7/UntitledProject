#include "utilities.hpp"

#ifdef _WIN64
#define CURRENT_ARCH IMAGE_FILE_MACHINE_AMD64
#else
#define CURRENT_ARCH IMAGE_FILE_MACHINE_I386
#endif

VOID stub( VOID ) { }

#define RELOC_FLAG32(RelInfo) ((RelInfo >> 0x0C) == IMAGE_REL_BASED_HIGHLOW)
#define RELOC_FLAG64(RelInfo) ((RelInfo >> 0x0C) == IMAGE_REL_BASED_DIR64)

#ifdef _WIN64
#define RELOC_FLAG RELOC_FLAG64
#else
#define RELOC_FLAG RELOC_FLAG32
#endif

#pragma runtime_checks( "", off )
#pragma optimize( "", off )
void __stdcall Shellcode( MANUAL_MAPPING_DATA* pData ) {
    if ( !pData ) {
        pData->hMod = ( HINSTANCE ) 0x404040;
        return;
    }

    BYTE* pBase = pData->pbase;
    auto* pOpt = &reinterpret_cast< IMAGE_NT_HEADERS* >( pBase + reinterpret_cast< IMAGE_DOS_HEADER* >( ( uintptr_t ) pBase )->e_lfanew )->OptionalHeader;

    auto _LoadLibraryA = pData->pLoadLibraryA;
    auto _GetProcAddress = pData->pGetProcAddress;
#ifdef _WIN64
    auto _RtlAddFunctionTable = pData->pRtlAddFunctionTable;
#endif
    auto _DllMain = reinterpret_cast< f_DLL_ENTRY_POINT >( pBase + pOpt->AddressOfEntryPoint );

    BYTE* LocationDelta = pBase - pOpt->ImageBase;
    if ( LocationDelta ) {
        if ( pOpt->DataDirectory[ IMAGE_DIRECTORY_ENTRY_BASERELOC ].Size ) {
            auto* pRelocData = reinterpret_cast< IMAGE_BASE_RELOCATION* >( pBase + pOpt->DataDirectory[ IMAGE_DIRECTORY_ENTRY_BASERELOC ].VirtualAddress );
            const auto* pRelocEnd = reinterpret_cast< IMAGE_BASE_RELOCATION* >( reinterpret_cast< uintptr_t >( pRelocData ) + pOpt->DataDirectory[ IMAGE_DIRECTORY_ENTRY_BASERELOC ].Size );
            while ( pRelocData < pRelocEnd && pRelocData->SizeOfBlock ) {
                UINT AmountOfEntries = ( pRelocData->SizeOfBlock - sizeof( IMAGE_BASE_RELOCATION ) ) / sizeof( WORD );
                WORD* pRelativeInfo = reinterpret_cast< WORD* >( pRelocData + 1 );

                for ( UINT i = 0; i != AmountOfEntries; ++i, ++pRelativeInfo ) {
                    if ( RELOC_FLAG( *pRelativeInfo ) ) {
                        UINT_PTR* pPatch = reinterpret_cast< UINT_PTR* >( pBase + pRelocData->VirtualAddress + ( ( *pRelativeInfo ) & 0xFFF ) );
                        *pPatch += reinterpret_cast< UINT_PTR >( LocationDelta );
                    }
                }
                pRelocData = reinterpret_cast< IMAGE_BASE_RELOCATION* >( reinterpret_cast< BYTE* >( pRelocData ) + pRelocData->SizeOfBlock );
            }
        }
    }

    if ( pOpt->DataDirectory[ IMAGE_DIRECTORY_ENTRY_IMPORT ].Size ) {
        auto* pImportDescr = reinterpret_cast< IMAGE_IMPORT_DESCRIPTOR* >( pBase + pOpt->DataDirectory[ IMAGE_DIRECTORY_ENTRY_IMPORT ].VirtualAddress );
        while ( pImportDescr->Name ) {
            char* szMod = reinterpret_cast< char* >( pBase + pImportDescr->Name );
            HINSTANCE hDll = _LoadLibraryA( szMod );

            ULONG_PTR* pThunkRef = reinterpret_cast< ULONG_PTR* >( pBase + pImportDescr->OriginalFirstThunk );
            ULONG_PTR* pFuncRef = reinterpret_cast< ULONG_PTR* >( pBase + pImportDescr->FirstThunk );

            if ( !pThunkRef )
                pThunkRef = pFuncRef;

            for ( ; *pThunkRef; ++pThunkRef, ++pFuncRef ) {
                if ( IMAGE_SNAP_BY_ORDINAL( *pThunkRef ) ) {
                    *pFuncRef = ( ULONG_PTR ) _GetProcAddress( hDll, reinterpret_cast< char* >( *pThunkRef & 0xFFFF ) );
                }
                else {
                    auto* pImport = reinterpret_cast< IMAGE_IMPORT_BY_NAME* >( pBase + ( *pThunkRef ) );
                    *pFuncRef = ( ULONG_PTR ) _GetProcAddress( hDll, pImport->Name );
                }
            }
            ++pImportDescr;
        }
    }

    if ( pOpt->DataDirectory[ IMAGE_DIRECTORY_ENTRY_TLS ].Size ) {
        auto* pTLS = reinterpret_cast< IMAGE_TLS_DIRECTORY* >( pBase + pOpt->DataDirectory[ IMAGE_DIRECTORY_ENTRY_TLS ].VirtualAddress );
        auto* pCallback = reinterpret_cast< PIMAGE_TLS_CALLBACK* >( pTLS->AddressOfCallBacks );
        for ( ; pCallback && *pCallback; ++pCallback )
            ( *pCallback )( pBase, DLL_PROCESS_ATTACH, nullptr );
    }

    bool ExceptionSupportFailed = false;

#ifdef _WIN64

    if ( pData->SEHSupport ) {
        auto excep = pOpt->DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXCEPTION ];
        if ( excep.Size ) {
            if ( !_RtlAddFunctionTable(
                reinterpret_cast< IMAGE_RUNTIME_FUNCTION_ENTRY* >( pBase + excep.VirtualAddress ),
                excep.Size / sizeof( IMAGE_RUNTIME_FUNCTION_ENTRY ), ( DWORD64 ) pBase ) ) {
                ExceptionSupportFailed = true;
            }
        }
    }

#endif

    _DllMain( pBase, pData->fdwReasonParam, pData->reservedParam );

    if ( ExceptionSupportFailed )
        pData->hMod = reinterpret_cast< HINSTANCE >( 0x505050 );
    else
        pData->hMod = reinterpret_cast< HINSTANCE >( pBase );
}

int CUtilities::InjectDLL( const char* process_name, uint8_t binary[] ) {
    int ProcessID = GetProcessID( process_name );
    if ( ProcessID == 0 ) {
        MessageBoxA( Win32->GetHwnd( ), "Specified process not found!", "Failed!", 0 );
        return 0;
    }

    EscalateLocalPrivileges( );

    HANDLE TargetProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, ProcessID );
    if ( TargetProcess == INVALID_HANDLE_VALUE )
        return 0;

    if ( !IsCorrectTargetArchitecture( TargetProcess ) ) {
        MessageBoxA( Win32->GetHwnd( ), "Architecture mismatch, cannot inject into x64 bit processes!", "Failed!", 0 );
        return 0;
    }

    IMAGE_NT_HEADERS* pOldNtHeader = nullptr;
    IMAGE_OPTIONAL_HEADER* pOldOptHeader = nullptr;
    IMAGE_FILE_HEADER* pOldFileHeader = nullptr;
    BYTE* pTargetBase = nullptr;

    if ( reinterpret_cast< IMAGE_DOS_HEADER* >( binary )->e_magic != 0x5A4D ) {
        MessageBoxA( Win32->GetHwnd( ), "File seems to be corrupt!", "Failed!", 0 );
        return false;
    }

    pOldNtHeader = reinterpret_cast< IMAGE_NT_HEADERS* >( binary + reinterpret_cast< IMAGE_DOS_HEADER* >( binary )->e_lfanew );
    pOldOptHeader = &pOldNtHeader->OptionalHeader;
    pOldFileHeader = &pOldNtHeader->FileHeader;

    if ( pOldFileHeader->Machine != CURRENT_ARCH ) {
        MessageBoxA( Win32->GetHwnd( ), "Your operating system does not support this file!", "Failed!", 0 );
        return false;
    }

    pTargetBase = reinterpret_cast< BYTE* >( VirtualAllocEx( TargetProcess, nullptr, pOldOptHeader->SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE ) );
    if ( !pTargetBase ) {
        LOG( "Target process memory allocation failed (ex) 0x%X", GetLastError( ) );
        return false;
    }

    DWORD oldp = 0;
    VirtualProtectEx( TargetProcess, pTargetBase, pOldOptHeader->SizeOfImage, PAGE_EXECUTE_READWRITE, &oldp );

    MANUAL_MAPPING_DATA data{ 0 };
    data.pLoadLibraryA = LoadLibraryA;
    data.pGetProcAddress = GetProcAddress;
#ifdef _WIN64
    data.pRtlAddFunctionTable = ( f_RtlAddFunctionTable ) RtlAddFunctionTable;
#else 
    m_SEHExceptionSupport = false;
#endif
    data.pbase = pTargetBase;
    data.fdwReasonParam = DLL_PROCESS_ATTACH;
    data.reservedParam = 0;
    data.SEHSupport = m_SEHExceptionSupport;


    //File header
    if ( !WriteProcessMemory( TargetProcess, pTargetBase, binary, 0x1000, nullptr ) ) { //only first 0x1000 bytes for the header
        LOG( "Can't write file header 0x%X", GetLastError( ) );
        VirtualFreeEx( TargetProcess, pTargetBase, 0, MEM_RELEASE );
        return false;
    }

    IMAGE_SECTION_HEADER* pSectionHeader = IMAGE_FIRST_SECTION( pOldNtHeader );
    for ( UINT i = 0; i != pOldFileHeader->NumberOfSections; ++i, ++pSectionHeader ) {
        if ( pSectionHeader->SizeOfRawData ) {
            if ( !WriteProcessMemory( TargetProcess, pTargetBase + pSectionHeader->VirtualAddress, binary + pSectionHeader->PointerToRawData, pSectionHeader->SizeOfRawData, nullptr ) ) {
                LOG( "Can't map sections: 0x%x", GetLastError( ) );
                VirtualFreeEx( TargetProcess, pTargetBase, 0, MEM_RELEASE );
                return false;
            }
        }
    }

    BYTE* MappingDataAlloc = reinterpret_cast< BYTE* >( VirtualAllocEx( TargetProcess, nullptr, sizeof( MANUAL_MAPPING_DATA ), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE ) );
    if ( !MappingDataAlloc ) {
        LOG( "Target process mapping allocation failed (ex) 0x%X", GetLastError( ) );
        VirtualFreeEx( TargetProcess, pTargetBase, 0, MEM_RELEASE );
        return false;
    }

    if ( !WriteProcessMemory( TargetProcess, MappingDataAlloc, &data, sizeof( MANUAL_MAPPING_DATA ), nullptr ) ) {
        LOG( "Can't write mapping 0x%X", GetLastError( ) );
        VirtualFreeEx( TargetProcess, pTargetBase, 0, MEM_RELEASE );
        VirtualFreeEx( TargetProcess, MappingDataAlloc, 0, MEM_RELEASE );
        return false;
    }

    void* pShellcode = VirtualAllocEx( TargetProcess, nullptr, 0x1000, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE );
    if ( !pShellcode ) {
        LOG( "Memory shellcode allocation failed (ex) 0x%X", GetLastError( ) );
        VirtualFreeEx( TargetProcess, pTargetBase, 0, MEM_RELEASE );
        VirtualFreeEx( TargetProcess, MappingDataAlloc, 0, MEM_RELEASE );
        return false;
    }

    if ( !WriteProcessMemory( TargetProcess, pShellcode, Shellcode, 0x1000, nullptr ) ) {
        LOG( "Can't write shellcode 0x%X", GetLastError( ) );
        VirtualFreeEx( TargetProcess, pTargetBase, 0, MEM_RELEASE );
        VirtualFreeEx( TargetProcess, MappingDataAlloc, 0, MEM_RELEASE );
        VirtualFreeEx( TargetProcess, pShellcode, 0, MEM_RELEASE );
        return false;
    }

    LOG( "Dynamic Link Library mapped at: 0x%p", pTargetBase );
    LOG( "Mapping info at: 0x%p", MappingDataAlloc );
    LOG( "Shell code mapped at: 0x%p", pShellcode );
    LOG( "Shellcode pointer at: 0x%p", Shellcode );
    LOG( "Target point at: 0x%p", pShellcode );

    HANDLE hThread = CreateRemoteThread( TargetProcess, nullptr, 0, reinterpret_cast< LPTHREAD_START_ROUTINE >( pShellcode ), MappingDataAlloc, 0, nullptr );
    if ( !hThread ) {
        LOG( "Thread creation error! error code: %X", GetLastError( ) );
        VirtualFreeEx( TargetProcess, pTargetBase, 0, MEM_RELEASE );
        VirtualFreeEx( TargetProcess, MappingDataAlloc, 0, MEM_RELEASE );
        VirtualFreeEx( TargetProcess, pShellcode, 0, MEM_RELEASE );
        return false;
    }
    CloseHandle( hThread );

    LOG( "Thread created at: 0x%p, waiting for return...", pShellcode );

    HINSTANCE hCheck = NULL;
    while ( !hCheck ) {
        DWORD exitcode = 0;
        GetExitCodeProcess( TargetProcess, &exitcode );
        if ( exitcode != STILL_ACTIVE ) {
            LOG( "Process crashed, exit code: %d", exitcode );
            return false;
        }

        MANUAL_MAPPING_DATA data_checked{ 0 };
        ReadProcessMemory( TargetProcess, MappingDataAlloc, &data_checked, sizeof( data_checked ), nullptr );
        hCheck = data_checked.hMod;

        if ( hCheck == ( HINSTANCE ) 0x404040 ) {
            LOG( "Wrong mapping ptr" );
            VirtualFreeEx( TargetProcess, pTargetBase, 0, MEM_RELEASE );
            VirtualFreeEx( TargetProcess, MappingDataAlloc, 0, MEM_RELEASE );
            VirtualFreeEx( TargetProcess, pShellcode, 0, MEM_RELEASE );
            return false;
        }
        else if ( hCheck == ( HINSTANCE ) 0x505050 ) {
            LOG( "WARNING: Exception support failed!" );
        }

        Sleep( 10 );
    }

    BYTE* emptyBuffer = ( BYTE* ) malloc( 1024 * 1024 * 20 );
    if ( emptyBuffer == nullptr ) {
        LOG( "Unable to allocate memory" );
        return false;
    }
    memset( emptyBuffer, 0, 1024 * 1024 * 20 );

    if ( !WriteProcessMemory( TargetProcess, pTargetBase, emptyBuffer, 0x1000, nullptr ) ) {
        LOG( "WARNING!: Can't clear HEADER" );
    }


    pSectionHeader = IMAGE_FIRST_SECTION( pOldNtHeader );
    for ( UINT i = 0; i != pOldFileHeader->NumberOfSections; ++i, ++pSectionHeader ) {
        if ( pSectionHeader->Misc.VirtualSize ) {
            if ( ( m_SEHExceptionSupport ? 0 : strcmp( ( char* ) pSectionHeader->Name, ".pdata" ) == 0 ) ||
                strcmp( ( char* ) pSectionHeader->Name, ".rsrc" ) == 0 ||
                strcmp( ( char* ) pSectionHeader->Name, ".reloc" ) == 0 ) {
                LOG( "Processing %s removal", pSectionHeader->Name );
                if ( !WriteProcessMemory( TargetProcess, pTargetBase + pSectionHeader->VirtualAddress, emptyBuffer, pSectionHeader->Misc.VirtualSize, nullptr ) ) {
                    LOG( "Can't clear section %s: 0x%x", pSectionHeader->Name, GetLastError( ) );
                }
            }
        }
    }

    /*if ( AdjustProtections ) {
        pSectionHeader = IMAGE_FIRST_SECTION( pOldNtHeader );
        for ( UINT i = 0; i != pOldFileHeader->NumberOfSections; ++i, ++pSectionHeader ) {
            if ( pSectionHeader->Misc.VirtualSize ) {
                DWORD old = 0;
                DWORD newP = PAGE_READONLY;

                if ( ( pSectionHeader->Characteristics & IMAGE_SCN_MEM_WRITE ) > 0 ) {
                    newP = PAGE_READWRITE;
                }
                else if ( ( pSectionHeader->Characteristics & IMAGE_SCN_MEM_EXECUTE ) > 0 ) {
                    newP = PAGE_EXECUTE_READ;
                }
                if ( VirtualProtectEx( TargetProcess, pTargetBase + pSectionHeader->VirtualAddress, pSectionHeader->Misc.VirtualSize, newP, &old ) ) {
                    LOG( "section %s set as %lX", ( char* ) pSectionHeader->Name, newP );
                }
                else {
                    LOG( "FAIL: section %s not set as %lX", ( char* ) pSectionHeader->Name, newP );
                }
            }
        }
        DWORD old = 0;
        VirtualProtectEx( TargetProcess, pTargetBase, IMAGE_FIRST_SECTION( pOldNtHeader )->VirtualAddress, PAGE_READONLY, &old );
    }*/

    if ( !WriteProcessMemory( TargetProcess, pShellcode, emptyBuffer, 0x1000, nullptr ) ) {
        LOG( "WARNING: Can't clear shellcode" );
    }
    if ( !VirtualFreeEx( TargetProcess, pShellcode, 0, MEM_RELEASE ) ) {
        LOG( "WARNING: can't release shell code memory" );
    }
    if ( !VirtualFreeEx( TargetProcess, MappingDataAlloc, 0, MEM_RELEASE ) ) {
        LOG( "WARNING: can't release mapping data memory" );
    }

    LOG( "Successfully injected!!" );
    MessageBoxA( Win32->GetHwnd( ), "Successfully injected!", "Success!", 0 );

    return 1;
}

int CUtilities::OverwritePrivileges( const char* process_name ) {
    HANDLE processSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
    if ( processSnapshot == INVALID_HANDLE_VALUE )
        return 1;

    HANDLE process = NULL;
    PROCESSENTRY32W processInfo;
    processInfo.dwSize = sizeof( processInfo );

    int bufferSize = MultiByteToWideChar( CP_UTF8, 0, process_name, -1, NULL, 0 );
    WCHAR* wideString = new WCHAR[ bufferSize ];

    MultiByteToWideChar( CP_UTF8, 0, process_name, -1, wideString, bufferSize );

    if ( Process32FirstW( processSnapshot, &processInfo ) ) {
        do {
            if ( !lstrcmpW( processInfo.szExeFile, wideString ) ) {
                process = OpenProcess( PROCESS_VM_WRITE | PROCESS_VM_OPERATION | PROCESS_CREATE_THREAD, FALSE, processInfo.th32ProcessID );
                break;
            }

        } while ( Process32NextW( processSnapshot, &processInfo ) );
    }
    CloseHandle( processSnapshot );

    if ( !process )
        return 1;
}

int CUtilities::IsCorrectTargetArchitecture( HANDLE process ) {
    BOOL bTarget = FALSE;
    
    if ( !IsWow64Process( process, &bTarget ) ) {
        printf( "Can't confirm target process architecture: 0x%X", GetLastError( ) );
        return false;
    }

    BOOL bHost = FALSE;
    IsWow64Process( GetCurrentProcess( ), &bHost );

    return ( bTarget == bHost );
}

int CUtilities::GetProcessID( const char* process_name ) {
    HANDLE hSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
    int PID = 0;

    if ( hSnap != INVALID_HANDLE_VALUE ) {
        PROCESSENTRY32 pE;
        pE.dwSize = sizeof( pE );

        if ( Process32First( hSnap, &pE ) ) {
            if ( !pE.th32ProcessID )
                Process32Next( hSnap, &pE );

            while ( Process32Next( hSnap, &pE ) ) {
                if ( !_stricmp( pE.szExeFile, process_name ) ) {
                    PID = pE.th32ProcessID;
                    break;
                }
            }
        }
    }

    CloseHandle( hSnap );

    return PID;
}

int CUtilities::EscalateLocalPrivileges( ) {
    TOKEN_PRIVILEGES priv = { 0 };
    HANDLE hToken = NULL;

    if ( !OpenProcessToken( GetCurrentProcess( ), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken ) )
        return 0;
    
    priv.PrivilegeCount = 1;
    priv.Privileges[ 0 ].Attributes = SE_PRIVILEGE_ENABLED;

    if ( LookupPrivilegeValue( NULL, SE_DEBUG_NAME, &priv.Privileges[ 0 ].Luid ) )
        AdjustTokenPrivileges( hToken, FALSE, &priv, 0, NULL, NULL );

    CloseHandle( hToken );

    return 1;
}