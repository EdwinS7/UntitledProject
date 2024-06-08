#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "../../../Common.hpp"

class cCamera {
public:
    void SetPosition( const Vec3<float>& position );
    Vec3<float> GetPosition( ) const;

    void SetRotation( const Vec3<float>& rotation );
    Vec3<float> GetRotation( ) const;

    void MoveForward( float distance );
    void MoveBackward( float distance );
    void MoveRight( float distance );
    void MoveLeft( float distance );
    void MoveUp( float distance );
    void MoveDown( float distance );
    void Rotate( float pitch, float yaw, float roll );

    void UpdateViewMatrix( );
    void Update( );

    D3DXMATRIX GetViewMatrix( ) const;
    D3DXMATRIX GetProjectionMatrix( ) const;

    void SetProjectionParameters( float field_of_view, float aspect_ratio, float near_plane, float far_plane );

private:
    Vec3<float> m_Position, m_Rotation;

    D3DXMATRIX m_ViewMatrix, m_ProjectionMatrix;

    float m_FieldOfView{ D3DX_PI / 4.0f }, m_AspectRatio{ 1.f },
        m_NearPlane{ 0.1f }, m_FarPlane{ 1000.f };
};

inline const auto gCamera = std::make_unique<cCamera>( );

#include "Camera.inl"
#endif