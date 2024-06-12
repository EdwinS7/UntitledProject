#pragma once
#ifndef AUDIO_H
#define AUDIO_H

#include "Common.hpp"

class cAudio {
public:

};

inline const auto gAudio = std::make_unique<cAudio>( );

#include "Audio.inl"
#endif