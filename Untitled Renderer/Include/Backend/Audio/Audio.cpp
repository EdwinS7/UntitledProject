#include "Audio.hpp"

void cAudio::OpenSound( const std::string& file_name ) {
    std::ifstream file( file_name, std::ios::binary );
    if ( !file.is_open( ) )
        return;

    std::vector<unsigned char> sound_data( ( std::istreambuf_iterator<char>( file ) ), std::istreambuf_iterator<char>( ) );
    m_Sounds[ file_name ] = sound_data;

    file.close( );
}

void cAudio::PlaySound_( const std::string& name ) {
    auto Sound = m_Sounds.find( name );
    if ( Sound == m_Sounds.end( ) )
        return;


}

void cAudio::StopSound( const std::string& name ) {

}

void cAudio::StopAllSounds( ) {

}