#include "utilities.hpp"

typedef struct {
    PBYTE imageBase;
    HMODULE( WINAPI* loadLibraryA )( PCSTR );
    FARPROC( WINAPI* getProcAddress )( HMODULE, PCSTR );
    VOID( WINAPI* rtlZeroMemory )( PVOID, SIZE_T );
} LoaderData;

DWORD WINAPI loadLibrary( LoaderData* loaderData )
{
    PIMAGE_NT_HEADERS ntHeaders = ( PIMAGE_NT_HEADERS ) ( loaderData->imageBase + ( ( PIMAGE_DOS_HEADER ) loaderData->imageBase )->e_lfanew );
    PIMAGE_BASE_RELOCATION relocation = ( PIMAGE_BASE_RELOCATION ) ( loaderData->imageBase
        + ntHeaders->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_BASERELOC ].VirtualAddress );
    DWORD delta = ( DWORD ) ( loaderData->imageBase - ntHeaders->OptionalHeader.ImageBase );
    while ( relocation->VirtualAddress ) {
        PWORD relocationInfo = ( PWORD ) ( relocation + 1 );
        for ( int i = 0, count = ( relocation->SizeOfBlock - sizeof( IMAGE_BASE_RELOCATION ) ) / sizeof( WORD ); i < count; i++ )
            if ( relocationInfo[ i ] >> 12 == IMAGE_REL_BASED_HIGHLOW )
                *( PDWORD ) ( loaderData->imageBase + ( relocation->VirtualAddress + ( relocationInfo[ i ] & 0xFFF ) ) ) += delta;

        relocation = ( PIMAGE_BASE_RELOCATION ) ( ( LPBYTE ) relocation + relocation->SizeOfBlock );
    }

    PIMAGE_IMPORT_DESCRIPTOR importDirectory = ( PIMAGE_IMPORT_DESCRIPTOR ) ( loaderData->imageBase
        + ntHeaders->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_IMPORT ].VirtualAddress );

    while ( importDirectory->Characteristics ) {
        PIMAGE_THUNK_DATA originalFirstThunk = ( PIMAGE_THUNK_DATA ) ( loaderData->imageBase + importDirectory->OriginalFirstThunk );
        PIMAGE_THUNK_DATA firstThunk = ( PIMAGE_THUNK_DATA ) ( loaderData->imageBase + importDirectory->FirstThunk );

        HMODULE module = loaderData->loadLibraryA( ( LPCSTR ) loaderData->imageBase + importDirectory->Name );

        if ( !module )
            return FALSE;

        while ( originalFirstThunk->u1.AddressOfData ) {
            DWORD Function = ( DWORD ) loaderData->getProcAddress( module, originalFirstThunk->u1.Ordinal & IMAGE_ORDINAL_FLAG ? ( LPCSTR ) ( originalFirstThunk->u1.Ordinal & 0xFFFF ) : ( ( PIMAGE_IMPORT_BY_NAME ) ( ( LPBYTE ) loaderData->imageBase + originalFirstThunk->u1.AddressOfData ) )->Name );

            if ( !Function )
                return FALSE;

            firstThunk->u1.Function = Function;
            originalFirstThunk++;
            firstThunk++;
        }
        importDirectory++;
    }

    if ( ntHeaders->OptionalHeader.AddressOfEntryPoint ) {
        DWORD result = ( ( DWORD( __stdcall* )( HMODULE, DWORD, LPVOID ) )
            ( loaderData->imageBase + ntHeaders->OptionalHeader.AddressOfEntryPoint ) )
            ( ( HMODULE ) loaderData->imageBase, DLL_PROCESS_ATTACH, NULL );

        loaderData->rtlZeroMemory( loaderData->imageBase + ntHeaders->OptionalHeader.AddressOfEntryPoint, 32 );
        loaderData->rtlZeroMemory( loaderData->imageBase, ntHeaders->OptionalHeader.SizeOfHeaders );

        return result;
    }
    return TRUE;
}

VOID stub( VOID ) { }

int CUtilities::InjectDLL( const char* process_name, uint8_t binary[] ) {
    int ProcessID = GetProcessID( process_name );
    if ( ProcessID == 0 ) {
        MessageBoxA( Win32->GetHwnd( ), "Specified process not found!", "Failed!", 0 );
        return 0;
    }

    EscalateLocalPrivileges( );

    LOG( "Process Name: %s", process_name );
    LOG( "Process ID: %d", ProcessID );

    HANDLE TargetProcess = OpenProcess( PROCESS_VM_WRITE | PROCESS_VM_OPERATION | PROCESS_CREATE_THREAD, FALSE, ProcessID );
    if ( TargetProcess == INVALID_HANDLE_VALUE )
        return 0;

    PIMAGE_NT_HEADERS ntHeaders = ( PIMAGE_NT_HEADERS ) ( binary + ( ( PIMAGE_DOS_HEADER ) binary )->e_lfanew );

    PBYTE executableImage = static_cast< PBYTE >( VirtualAllocEx( TargetProcess, NULL, ntHeaders->OptionalHeader.SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE ) );

    WriteProcessMemory( TargetProcess, executableImage, binary, ntHeaders->OptionalHeader.SizeOfHeaders, NULL );

    PIMAGE_SECTION_HEADER sectionHeaders = ( PIMAGE_SECTION_HEADER ) ( ntHeaders + 1 );

    for ( int i = 0; i < ntHeaders->FileHeader.NumberOfSections; i++ )
        WriteProcessMemory( TargetProcess, executableImage + sectionHeaders[ i ].VirtualAddress, binary + sectionHeaders[ i ].PointerToRawData, sectionHeaders[ i ].SizeOfRawData, NULL );

    LoaderData* loaderMemory = static_cast< LoaderData* >( VirtualAllocEx( TargetProcess, NULL, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READ ) );

    LoaderData loaderParams;
    loaderParams.imageBase = executableImage;
    loaderParams.loadLibraryA = LoadLibraryA;
    loaderParams.getProcAddress = GetProcAddress;
    loaderParams.rtlZeroMemory = ( VOID( NTAPI* )( PVOID, SIZE_T ) )GetProcAddress( LoadLibraryW( L"ntdll" ), "RtlZeroMemory" );

    WriteProcessMemory( TargetProcess, loaderMemory, &loaderParams, sizeof( LoaderData ), NULL );
    WriteProcessMemory( TargetProcess, loaderMemory + 1, loadLibrary, ( DWORD ) stub - ( DWORD ) loadLibrary, NULL );
    CreateRemoteThread( TargetProcess, NULL, 0, ( LPTHREAD_START_ROUTINE ) ( loaderMemory + 1 ), loaderMemory, 0, NULL );
    VirtualFreeEx( TargetProcess, loaderMemory, 0, MEM_RELEASE );

    LOG( "Injected!" );

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