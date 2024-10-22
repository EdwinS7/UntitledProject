#pragma once
#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <Common.hpp>
#include "../Logger/Logger.hpp"
#include "../Networking/Networking.hpp"

#define FS_LUA_FOLDER "Lua/"

#define FS_SCRIPTS_FOLDER FS_LUA_FOLDER "Scripts/"
#define FS_LIBRARY_SCRIPTS_FOLDER FS_LUA_FOLDER "Scripts/Libraries/"
#define FS_DEFAULT_SCRIPTS_FOLDER FS_LUA_FOLDER "DefaultScripts/"

#define FS_GAME_FOLDER "Game/"
#define FS_SOUNDS_FOLDER FS_GAME_FOLDER "Sounds/"
#define FS_TEXTURES_FOLDER FS_GAME_FOLDER "Textures/"

#define WRAPPER_ENUMERATIONS_SCRIPT_URL "https://raw.githubusercontent.com/EdwinS7/UntitledProject/main/Untitled%20Renderer%20Lua/DefaultScripts/Enumerations.lua"
#define WRAPPER_DEFAULT_SCRIPT_URL "https://raw.githubusercontent.com/EdwinS7/UntitledProject/main/Untitled%20Renderer%20Lua/DefaultScripts/Startup.lua"

#define FS_STARTUP_LUA_NAME "Startup.lua"

class cFileSystem {
public:
    void Init( );

	void CreateFolder( const std::string& folder_path );

	bool DoesFolderExist( const std::string& folder_path ) const;
    bool DoesFileExist( const std::string& folder_path, const std::string& file_name ) const;
	
    void WriteToFile( const std::string& folder_path, const std::string& file_name, const std::string& content );
    void AppendToFile( const std::string& folder_path, const std::string& file_name, const std::string& content );
    void DeleteFile( const std::string& folder_path, const std::string& file_name );
    void DeleteFolder( const std::string& folder_path );

    std::vector<std::string> GetUsableFonts( ) const;
    std::string GetFontPath( const std::string& font_name );

    std::vector<std::string> GetFilesInFolder( const std::string& folder_path );
    std::vector<std::string> GetFoldersInFolder( const std::string& folder_path );

    std::string GetFileContent( const std::string& folder_path, const std::string& file_name ) const;
    std::string GetFileCreationDate( const std::string& folder_path, const std::string& file_name ) const;
    std::string GetFileLastModifiedDate( const std::string& folder_path, const std::string& file_name ) const;
    std::size_t GetFileSize( const std::string& folder_path, const std::string& file_name ) const;

    std::string GetExecutableDirectory( ) const;

private:
    std::vector<std::string> m_UsableFonts;
};

inline const auto gFileSystem = std::make_unique<cFileSystem>( );

#include "FileSystem.inl"
#endif