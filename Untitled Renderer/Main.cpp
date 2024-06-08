#include "Common.hpp"

std::string OpenFileDialogAndReadContents( ) {
    OPENFILENAME ofn;       // common dialog box structure
    char szFile[ 260 ];       // buffer for file name

    // Initialize OPENFILENAME
    ZeroMemory( &ofn, sizeof( ofn ) );
    ofn.lStructSize = sizeof( ofn );
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[ 0 ] = '\0';
    ofn.nMaxFile = sizeof( szFile );
    ofn.lpstrFilter = "Lua Scripts\0*.lua;*.luac\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // Display the Open dialog box
    if ( GetOpenFileName( &ofn ) == TRUE ) {
        std::cout << "Selected file: " << ofn.lpstrFile << std::endl; // Debug print

        // Open the selected file
        std::ifstream file( ofn.lpstrFile );
        if ( file.is_open( ) ) {
            std::stringstream buffer;
            buffer << file.rdbuf( ); // Read the file contents into a stringstream
            std::string contents = buffer.str( ); // Convert the stringstream to a string

            std::cout << "File contents read successfully." << std::endl; // Debug print
            std::cout << "Contents: " << contents << std::endl; // Debug print

            return contents; // Return the file contents
        }
        else {
            std::cerr << "Failed to open the selected file.\n";
            return "";
        }
    }
    else {
        std::cerr << "No file selected or dialog canceled.\n"; // Debug print
        return "";
    }
}

int WINAPI WinMain( HINSTANCE instance, HINSTANCE prev_instance, PSTR cmd, int show_cmd ) {
    auto Script = OpenFileDialogAndReadContents( );
    gWindow->Init( "Untitled Renderer", Vec2<int16_t>( 1280, 720 ), true );
    gGraphics->Init( gWindow->GetHandle( ) );
    gWrapper->Init( );
    gBuffer->Init( );

    gWrapper->LoadScript( Script );

    while ( gWindow->DispatchMessages( ) ) {
        gContext->Update( );
        gCamera->Update( );

        if ( gWindow->IsFocused( ) ) {
            gInterface->Draw( );
            gWorld->Draw( );
        }

        gGraphics->DrawScene( );
    }

    gGraphics->Release( );
    gBuffer->Release( );
}