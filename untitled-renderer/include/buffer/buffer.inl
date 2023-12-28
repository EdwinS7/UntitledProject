#include "buffer.hpp"

inline void cBuffer::ClearCommands( ) {
    m_draw_commands.clear( );
    m_draw_command.reset( );

    m_VerticesCount = 0;
    m_IndicesCount = 0;
}

inline Command cBuffer::GetCommand( ) {
    return m_command;
}

inline CompiledDrawCommand cBuffer::GetDrawCommand( ) {
    return m_draw_command;
}

inline std::vector<DrawCommand> cBuffer::GetDrawCommands( ) {
    return m_draw_commands;
}

inline int cBuffer::GetCommandsCount( ) {
    return m_draw_commands.size();
}

inline int cBuffer::GetVerticesCount( ) {
    return m_VerticesCount;
}

inline int cBuffer::GetIndicesCount( ) {
    return m_IndicesCount;
}

inline void cBuffer::PushClip( RECT rect ) {
    m_command.clips.push_back( rect );
}

inline void cBuffer::PopClip( ) {
    m_command.clips.pop_back( );
}

inline void cBuffer::PushTexture( IDirect3DTexture9* resource ) {
    m_command.textures.push_back( resource );
}

inline void cBuffer::PopTexture( ) {
    m_command.textures.pop_back( );
}

inline std::string cBuffer::GetFontPath( const char* FontName ) {
	HKEY key;

	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts", 0, KEY_READ, &key) != ERROR_SUCCESS)
		std::printf("[ Buffer ] get_path failed ( failed to open registry )");

	std::string str_path;
	char str_buffer[MAX_PATH];

	for (auto i = 0;; i++) {
		DWORD buf_size = MAX_PATH;
		memset(str_buffer, 0, MAX_PATH);

		if (RegEnumValueA(key, i, str_buffer, &buf_size, nullptr, nullptr, nullptr, nullptr) != ERROR_SUCCESS)
			std::printf("[ Buffer ] get_path failed ( invalid font )");

		if (std::string(str_buffer).find( FontName ) != std::string::npos) {
			buf_size = MAX_PATH;
			RegQueryValueExA(key, str_buffer, nullptr, nullptr, reinterpret_cast<LPBYTE>(str_buffer), &buf_size);
			str_path = str_buffer;
			break;
		}
	}

	memset(str_buffer, 0, MAX_PATH);
	SHGetFolderPathA(nullptr, CSIDL_FONTS, nullptr, 0, str_buffer);

	return std::string(str_buffer) + '\\' + str_path;
}