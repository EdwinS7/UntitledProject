#include "Animation.hpp"
#include <math.h>
#define _USE_MATH_DEFINES

float cAnimations::Lerp( float from, float to, float delta_time ) {
    return from + ( to - from ) * delta_time;
}

float cAnimations::Coserp( float from, float to, float delta_time ) {
    float ft = delta_time * 3.14159265358979323846f;
    float f = ( 1 - std::cos( ft ) ) * 0.5f;
    return from * ( 1 - f ) + to * f;
}

float cAnimations::Smoothstep( float from, float to, float delta_time ) {
    delta_time = delta_time * delta_time * ( 3 - 2 * delta_time );
    return from + ( to - from ) * delta_time;
}