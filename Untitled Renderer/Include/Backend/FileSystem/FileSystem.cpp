#include "FileSystem.hpp"

void cFileSystem::Init( ) {
    CreateFolder( "DefaultScripts/" );
    CreateFolder( "Scripts/" );

    CreateFolder( "Sounds/" );
    CreateFolder( "Textures/" );
}

void cFileSystem::CreateFolder( const std::string& folder_path ) {
    if ( DoesFolderExist( folder_path ) )
        return;

    if ( !std::filesystem::create_directory( folder_path ) ) {
        gLogger->Print( LogLevel::Error, "Failed to create folder!" );
    }
}

bool cFileSystem::DoesFolderExist( const std::string& folder_path ) {
    return std::filesystem::exists( folder_path ) && std::filesystem::is_directory( folder_path );
}

std::string cFileSystem::GetFileContent( const std::string& folder_path, const std::string& file_name ) const {
    std::ifstream file( folder_path + "/" + file_name );
    if ( file ) {
        std::string content( ( std::istreambuf_iterator<char>( file ) ), std::istreambuf_iterator<char>( ) );
        return content;
    }
    else {
        gLogger->Print( LogLevel::Error, "Failed to open file: " + folder_path + "/" + file_name );
        return "";
    }
}

void cFileSystem::WriteToFile( const std::string& folder_path, const std::string& file_name, const std::string& content ) {
    std::ofstream file( folder_path + "/" + file_name );
   
    if ( file )
        file << content;
    else
        gLogger->Print( LogLevel::Error, "Failed to write to file:" + folder_path + "/" + file_name );
}

void cFileSystem::AppendToFile( const std::string& folder_path, const std::string& file_name, const std::string& content ) {
    std::ofstream file( folder_path + "/" + file_name, std::ios::app );
    
    if ( file )
        file << content;
    else
        gLogger->Print( LogLevel::Error, "Failed to write to file:" + folder_path + "/" + file_name );
}

void cFileSystem::DeleteFile( const std::string& folder_path, const std::string& file_name ) {
    if ( !std::filesystem::remove( folder_path + "/" + file_name ) )
        gLogger->Print( LogLevel::Error, "Failed to delete file:" + folder_path + "/" + file_name );
}

void cFileSystem::DeleteFolder( const std::string& folder_path ) {
    if ( std::filesystem::remove_all( folder_path ) < 0 )
        gLogger->Print( LogLevel::Error, "Failed to delete folder:" + folder_path );
}

std::vector<std::string> cFileSystem::GetFilesInFolder( const std::string& folder_path ) {
    std::vector<std::string> files;

    for ( const auto& entry : std::filesystem::directory_iterator( folder_path ) ) {
        if ( entry.is_regular_file( ) ) {
            files.push_back( entry.path( ).filename( ).string( ) );
        }
    }

    return files;
}

std::vector<std::string> cFileSystem::GetFoldersInFolder( const std::string& folder_path ) {
    std::vector<std::string> folders;

    for ( const auto& entry : std::filesystem::directory_iterator( folder_path ) ) {
        if ( entry.is_directory( ) ) {
            folders.push_back( entry.path( ).filename( ).string( ) );
        }
    }

    return folders;
}

std::string cFileSystem::GetFileCreationDate( const std::string& folder_path, const std::string& file_name ) const {
    auto creation_time = std::filesystem::last_write_time( folder_path + "/" + file_name );
    auto creation_time_point = std::chrono::time_point_cast< std::chrono::system_clock::duration >( creation_time - decltype( creation_time )::clock::now( ) + std::chrono::system_clock::now( ) );
    std::time_t creation_time_t = std::chrono::system_clock::to_time_t( creation_time_point );

    std::tm timeinfo;
    localtime_s( &timeinfo, &creation_time_t );
    char timeStr[ 26 ];
    asctime_s( timeStr, sizeof( timeStr ), &timeinfo );
    return timeStr;
}

std::string cFileSystem::GetFileLastModifiedDate( const std::string& folder_path, const std::string& file_name ) const {
    auto last_mod_time = std::filesystem::last_write_time( folder_path + "/" + file_name );
    auto last_mod_time_point = std::chrono::time_point_cast< std::chrono::system_clock::duration >( last_mod_time - decltype( last_mod_time )::clock::now( ) + std::chrono::system_clock::now( ) );
    std::time_t last_mod_time_t = std::chrono::system_clock::to_time_t( last_mod_time_point );

    std::tm timeinfo;
    localtime_s( &timeinfo, &last_mod_time_t );
    char timeStr[ 26 ];
    asctime_s( timeStr, sizeof( timeStr ), &timeinfo );
    return timeStr;
}

std::size_t cFileSystem::GetFileSize( const std::string& folder_path, const std::string& file_name ) const {
    return std::filesystem::file_size( folder_path + "/" + file_name );
}