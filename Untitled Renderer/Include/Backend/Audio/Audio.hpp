#pragma once
#ifndef AUDIO_H
#define AUDIO_H

#include "../../../Common.hpp"

class cAudio {
public:
    void Init( );
    void Release( );

    int LoadSound( const std::string& file_name );
    void PlaySound_( const int sound_id );
    void StopSound( const int sound_id );

    void StopAllSounds( );

private:
    std::vector<std::vector<float>> sounds;

};

inline const auto gAudio = std::make_unique<cAudio>( );

#include "Audio.inl"
#endif