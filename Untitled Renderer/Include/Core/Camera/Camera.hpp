#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <Common.hpp>

class cCamera {
public:

};

inline const auto gCamera = std::make_unique<cCamera>( );

#include "Camera.inl"
#endif