#include "Interface.hpp"

// Draw 2D Screen Interfaces & Effects
void cInterface::Init( ) {
	
}

void cInterface::Draw( ) {
    int centerX = 1280 / 2;
    int centerY = 720 / 2;
    float time = gContext->GetRealTime( );

    // Draw rotating star pattern
    int numPoints = 12;
    for ( int i = 0; i < numPoints; ++i ) {
        float angle = i * ( 360.0f / numPoints ) + time * 100; // Rotate over time
        float startX = centerX + 100 * cos( angle * 3.14 / 180 );
        float startY = centerY + 100 * sin( angle * 3.14 / 180 );
        float endX = centerX + 200 * cos( ( angle + 144 ) * 3.14 / 180 );
        float endY = centerY + 200 * sin( ( angle + 144 ) * 3.14 / 180 );

        Color color( static_cast< uint8_t >( sin( time ) * 255 ), static_cast< uint8_t >( cos( time ) * 255 ), static_cast< uint8_t >( ( sin( time ) + cos( time ) ) * 127 ) );

        gBuffer->Line( Vec2<int16_t>( startX, startY ), Vec2<int16_t>( endX, endY ), color );
    }

    // Draw central circle pulsating
    int pulsateRadius = 50 + static_cast< int >( sin( time ) * 10 ); // Pulsate over time
    Color pulsateColor( 255, static_cast< uint8_t >( sin( time ) * 255 ), 0 );
    gBuffer->FilledCircle( Vec2<int16_t>( centerX, centerY ), pulsateRadius, pulsateColor );

    // Draw rectangles
    gBuffer->Rectangle( Vec2<int16_t>( centerX - 100, centerY - 100 ), Vec2<int16_t>( 200, 200 ), Color( 0, 255, 255 ) );
    gBuffer->FilledRectangle( Vec2<int16_t>( centerX + 150, centerY - 75 ), Vec2<int16_t>( 100, 150 ), Color( 255, 0, 255 ), 20, CornerFlags::CornerTopLeft );

    // Draw triangle
    Vec2<int16_t> points[] = {
        Vec2<int16_t>( centerX + 50, centerY - 150 ),
        Vec2<int16_t>( centerX + 150, centerY - 50 ),
        Vec2<int16_t>( centerX - 50, centerY - 50 )
    };
    gBuffer->FilledTriangle( points[ 0 ], points[ 1 ], points[ 2 ], Color( 255, 255, 0 ) );

    // Draw text
    gBuffer->Text( gBuffer->GetDefaultFont( ), "Cool Shapes!", Vec2<int16_t>( centerX - 50, centerY + 100 ), Color( 255, 255, 255 ) );
}

void cInterface::DrawAllShapes( ) {
    // Draw Line
    gBuffer->Line( Vec2<int16_t>( 50, 50 ), Vec2<int16_t>( 100, 50 ), Color( 255, 0, 0 ) );

    // Draw Polyline
    std::vector<Vec2<int16_t>> pointsPolyline = { {120, 50}, {150, 90}, {180, 50}, {210, 90} };
    gBuffer->Polyline( pointsPolyline, Color( 0, 255, 0 ) );

    // Draw Polygon
    std::vector<Vec2<int16_t>> pointsPolygon = { {230, 50}, {280, 90}, {330, 50} };
    gBuffer->Polygon( pointsPolygon, Color( 0, 0, 255 ) );

    // Draw Rectangle
    gBuffer->Rectangle( Vec2<int16_t>( 50, 120 ), Vec2<int16_t>( 100, 80 ), Color( 255, 255, 0 ) );

    // Draw Filled Rectangle
    gBuffer->FilledRectangle( Vec2<int16_t>( 170, 120 ), Vec2<int16_t>( 80, 60 ), Color( 255, 0, 255 ) );

    // Draw Textured Rectangle
    // LPDIRECT3DTEXTURE9* texture = /* provide texture */;
    // gBuffer->TexturedRectangle(texture, Vec2<int16_t>(280, 120), Vec2<int16_t>(120, 60), Color(255, 255, 255));

    // Draw Gradient
    gBuffer->Gradient( Vec2<int16_t>( 50, 220 ), Vec2<int16_t>( 100, 60 ), Color( 255, 0, 0 ), Color( 0, 255, 0 ), false );

    // Draw Filled Gradient
    gBuffer->FilledGradient( Vec2<int16_t>( 170, 220 ), Vec2<int16_t>( 100, 60 ), Color( 0, 0, 255 ), Color( 255, 255, 0 ), true );

    // Draw Triangle
    gBuffer->Triangle( Vec2<int16_t>( 290, 220 ), Vec2<int16_t>( 340, 260 ), Vec2<int16_t>( 390, 220 ), Color( 255, 0, 255 ) );

    // Draw Filled Triangle
    gBuffer->FilledTriangle( Vec2<int16_t>( 50, 320 ), Vec2<int16_t>( 100, 380 ), Vec2<int16_t>( 150, 320 ), Color( 0, 255, 255 ) );

    // Draw Circle
    gBuffer->Circle( Vec2<int16_t>( 200, 350 ), 30, Color( 255, 128, 0 ) );

    // Draw Filled Circle
    gBuffer->FilledCircle( Vec2<int16_t>( 280, 350 ), 25, Color( 128, 255, 0 ) );

    // Draw Text
    gBuffer->Text( gBuffer->GetDefaultFont( ), "Hello, Shapes!", Vec2<int16_t>( 340, 320 ), Color( 255, 255, 255 ) );
}
