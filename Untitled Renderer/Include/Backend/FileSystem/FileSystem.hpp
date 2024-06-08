#pragma once
#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include "../../../Common.hpp"

class cFileSystem {
public:
    void Init( );

	void CreateFolder( const std::string& folder_path );
	bool DoesFolderExist( const std::string& folder_path );
	
    void WriteToFile( const std::string& folder_path, const std::string& file_name, const std::string& content );
    void AppendToFile( const std::string& folder_path, const std::string& file_name, const std::string& content );
    void DeleteFile( const std::string& folder_path, const std::string& file_name );
    void DeleteFolder( const std::string& folder_path );

    std::vector<std::string> GetFilesInFolder( const std::string& folder_path );
    std::vector<std::string> GetFoldersInFolder( const std::string& folder_path );

    std::string GetFileContent( const std::string& folder_path, const std::string& file_name ) const;
    std::string GetFileCreationDate( const std::string& folder_path, const std::string& file_name ) const;
    std::string GetFileLastModifiedDate( const std::string& folder_path, const std::string& file_name ) const;
    std::size_t GetFileSize( const std::string& folder_path, const std::string& file_name ) const;

private:
	
};

inline const auto gFileSystem = std::make_unique<cFileSystem>( );

#include "FileSystem.inl"
#endif