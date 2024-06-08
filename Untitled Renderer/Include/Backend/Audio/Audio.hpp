#pragma once
#ifndef AUDIO_H
#define AUDIO_H

#include "../../../Common.hpp"

class cAudio {
public:
    void OpenSound( const std::string& file_name );
    void PlaySound_( const std::string& name );
    void StopSound( const std::string& name );

    void StopAllSounds( );

private:
    std::unordered_map<std::string, std::vector<unsigned char>> m_Sounds;

};

inline const auto gAudio = std::make_unique<cAudio>( );

#include "Audio.inl"
#endif