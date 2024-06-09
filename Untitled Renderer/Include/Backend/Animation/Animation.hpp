#pragma once
#ifndef ANIMATION_H
#define ANIMATION_H

#include "../../../Common.hpp"

class cAnimations {
public:
    float Lerp( float from, float to, float delta_time );
    float Coserp( float from, float to, float delta_time );
    float Smoothstep( float from, float to, float delta_time );

private:

};

inline const auto gAnimations = std::make_unique<cAnimations>( );

#include "Animation.inl"
#endif