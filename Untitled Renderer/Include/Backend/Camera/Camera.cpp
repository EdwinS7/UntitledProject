#include "Camera.hpp"

void cCamera::SetPosition( const Vec3<float>& position ) {
    m_Position = position;
    UpdateViewMatrix( );
}

Vec3<float> cCamera::GetPosition( ) const {
    return m_Position;
}

void cCamera::SetRotation( const Vec3<float>& rotation ) {
    m_Rotation = rotation;
    UpdateViewMatrix( );
}

Vec3<float> cCamera::GetRotation( ) const {
    return m_Rotation;
}

void cCamera::MoveForward( float distance ) {
    m_Position.z += distance;
    UpdateViewMatrix( );
}

void cCamera::MoveBackward( float distance ) {
    m_Position.z -= distance;
    UpdateViewMatrix( );
}

void cCamera::MoveRight( float distance ) {
    m_Position.x += distance;
    UpdateViewMatrix( );
}

void cCamera::MoveLeft( float distance ) {
    m_Position.x -= distance;
    UpdateViewMatrix( );
}

void cCamera::MoveUp( float distance ) {
    m_Position.y += distance;
    UpdateViewMatrix( );
}

void cCamera::MoveDown( float distance ) {
    m_Position.y -= distance;
    UpdateViewMatrix( );
}

void cCamera::Rotate( float pitch, float yaw, float roll ) {
    m_Rotation.x += pitch;
    m_Rotation.y += yaw;
    m_Rotation.z += roll;
    UpdateViewMatrix( );
}

void cCamera::UpdateViewMatrix( ) {
    D3DXMATRIX rotationMatrix;
    D3DXVECTOR3 up, lookAt, position;

    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;

    position.x = m_Position.x;
    position.y = m_Position.y;
    position.z = m_Position.z;

    D3DXMatrixRotationYawPitchRoll( &rotationMatrix, m_Rotation.y, m_Rotation.x, m_Rotation.z );

    lookAt.x = 0.0f;
    lookAt.y = 0.0f;
    lookAt.z = 1.0f;

    D3DXVec3TransformCoord( &lookAt, &lookAt, &rotationMatrix );
    D3DXVec3TransformCoord( &up, &up, &rotationMatrix );

    lookAt = position + lookAt;

    D3DXMatrixLookAtLH( &m_ViewMatrix, &position, &lookAt, &up );
}

void cCamera::Update( ) {
    if ( !gInput->AnyKeyPressed( ) )
        return;

    float DeltaTime = gContext->GetDeltaTime( );

    float SpeedFactor = 10.0f * DeltaTime;
    float RotationFactor = 1.0f * DeltaTime;

    if ( gInput->KeyHeld( KEY_W ) ) {
        MoveForward( SpeedFactor );
    }
    if ( gInput->KeyHeld( KEY_S ) ) {
        MoveBackward( SpeedFactor );
    }
    if ( gInput->KeyHeld( KEY_A ) ) {
        MoveLeft( SpeedFactor );
    }
    if ( gInput->KeyHeld( KEY_D ) ) {
        MoveRight( SpeedFactor );
    }
    if ( gInput->KeyHeld( KEY_SPACE ) ) {
        MoveUp( SpeedFactor );
    }
    if ( gInput->KeyHeld( KEY_CONTROL ) ) {
        MoveDown( SpeedFactor );
    }
    if ( gInput->KeyHeld( KEY_UP_ARROW ) ) {
        Rotate( -RotationFactor, 0.0f, 0.0f );
    }
    if ( gInput->KeyHeld( KEY_DOWN_ARROW ) ) {
        Rotate( RotationFactor, 0.0f, 0.0f );
    }
    if ( gInput->KeyHeld( KEY_LEFT_ARROW ) ) {
        Rotate( 0.0f, -RotationFactor, 0.0f );
    }
    if ( gInput->KeyHeld( KEY_RIGHT_ARROW ) ) {
        Rotate( 0.0f, RotationFactor, 0.0f );
    }
}

D3DXMATRIX cCamera::GetViewMatrix( ) const {
    return m_ViewMatrix;
}

D3DXMATRIX cCamera::GetProjectionMatrix( ) const {
    return m_ProjectionMatrix;
}

void cCamera::SetProjectionParameters( float field_of_view, float aspect_ratio, float near_plane, float far_plane ) {
    m_FieldOfView = field_of_view;
    m_AspectRatio = aspect_ratio;
    m_NearPlane = near_plane;
    m_FarPlane = far_plane;

    D3DXMatrixPerspectiveFovLH( &m_ProjectionMatrix, m_FieldOfView, m_AspectRatio, m_NearPlane, m_FarPlane );
}