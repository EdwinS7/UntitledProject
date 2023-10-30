#include "context.hpp"

int Untitled::Context::IsFirstLaunch( ) {
    HKEY HKey;

    if ( RegOpenKeyEx( HKEY_CURRENT_USER, "Software//UntitledAPI", 0, KEY_READ, &HKey ) != ERROR_SUCCESS ) {
        if ( RegCreateKey( HKEY_CURRENT_USER, "Software//UntitledAPI", &HKey ) == ERROR_SUCCESS ) {
            DWORD value = 1;
            RegSetValueEx( HKey, "IsFirstLaunch", 0, REG_DWORD, ( BYTE* ) &value, sizeof( value ) );
            RegCloseKey( HKey );
        }

        return 1;
    }
    else {
        RegCloseKey( HKey );
        return 0;
    }
}