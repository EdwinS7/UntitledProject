#include "Wrapper.hpp"
#include "Features.hpp"

//Need more money for my drugs so I added all this stuff to start ig

void cWrapper::Init( ) {
    Lua.open_libraries( 
        sol::lib::base, sol::lib::package, sol::lib::coroutine,
        sol::lib::string, sol::lib::math, sol::lib::table,
        sol::lib::debug, sol::lib::bit32, sol::lib::utf8
        /*sol::lib::ffi, sol::lib::jit*/
    );

    // Disabled for user safety.
    Lua[ "collectgarbage" ] = sol::nil;
    Lua[ "dofilsse" ] = sol::nil;
    Lua[ "load" ] = sol::nil;
    Lua[ "loadfile" ] = sol::nil;
    Lua[ "pcall" ] = sol::nil;
    Lua[ "xpcall" ] = sol::nil;
    Lua[ "getmetatable" ] = sol::nil;
    Lua[ "setmetatable" ] = sol::nil;
    Lua[ "__nil_callback" ] = [ ] ( ) {};

    Lua.new_usertype<Vec2<int16_t>>( 
        "Vector2", sol::constructors<Vec2<int16_t>( ), Vec2<int16_t>( int16_t, int16_t )>( ),
        "x", &Vec2<int16_t>::x,
        "y", &Vec2<int16_t>::y,
        "Min", &Vec2<int16_t>::Min,
        "Max", &Vec2<int16_t>::Max,
        "Lerp", &Vec2<int16_t>::Lerp,
        "DistanceTo", &Vec2<int16_t>::DistanceTo
    );

    Lua.new_usertype <Vec3<int>>( 
        "Vector3", sol::constructors<Vec3<int>( ), Vec3<int>( int, int, int )>( ),
        "x", &Vec3<int>::x,
        "y", &Vec3<int>::y,
        "z", &Vec3<int>::z,
        "Lerp", &Vec3<int>::Lerp
    );

    Lua.new_usertype <Vec4<int>>( 
        "Vector4", sol::constructors<Vec4<int>( ), Vec4<int>( int, int, int, int )>( ),
        "x", &Vec4<int>::x,
        "y", &Vec4<int>::y,
        "z", &Vec4<int>::z,
        "w", &Vec4<int>::w,
        "Lerp", &Vec4<int>::Lerp
    );

    Lua.new_usertype <Rect<int>>( 
        "Rect", sol::constructors<Rect<int>( ), Rect<int16_t>( int16_t, int16_t, int16_t, int16_t )>( ),
        "x", &Rect<int>::x,
        "y", &Rect<int>::y,
        "w", &Rect<int>::w,
        "h", &Rect<int>::h,
        "Lerp", &Rect<int>::Lerp
    );

    Lua.new_usertype<Color>( 
        "Color", sol::constructors<Color( ), Color( uint8_t, uint8_t, uint8_t, uint8_t )>( ),
        "Hex", &Color::Hex,
        "r", sol::property( &Color::getR ),
        "g", sol::property( &Color::getG ),
        "b", sol::property( &Color::getB ),
        "a", sol::property( &Color::getA )
    );

    // Globals
    Lua[ "AddCallback" ] = AddCallback;

    auto Client = Lua.create_table( );
    Client[ "GetUsername" ] = Client::GetUsername;
    Client[ "GetFramerate" ] = Client::GetFPS;
    Client[ "GetRealtime" ] = Client::GetRealTime;
    Client[ "GetDeltaTime" ] = Client::GetDeltaTime;
    Client[ "GetFontList" ] = Client::GetFontList;

    auto Input = Lua.create_table( );
    Input[ "IsMouseHoveringRect" ] = Input::IsMouseHoveringRect;
    Input[ "GetMousePosition" ] = Input::GetMousePosition;
    Input[ "SetMousePosition" ] = Input::SetMousePosition;
    Input[ "IsAnyKeyPressed" ] = Input::IsAnyKeyPressed;
    Input[ "IsKeyPressed" ] = Input::IsKeyPressed;
    Input[ "IsKeyHeld" ] = Input::IsKeyPressed;
    Input[ "SetCursorStyle" ] = Input::SetCursorStyle;
    Input[ "GetCursorStyle" ] = Input::GetCursorStyle;

    auto Graphics = Lua.create_table( );
    Graphics[ "SetClearColor" ] = Graphics::SetClearColor;
    Graphics[ "GetClearColor" ] = Graphics::GetClearColor;

    auto Renderer = Lua.create_table( );
    Renderer[ "Line" ] = Renderer::Line;
    Renderer[ "Polyline" ] = Renderer::Polyline;
    Renderer[ "Polygon" ] = Renderer::Polygon;
    Renderer[ "Rectangle" ] = Renderer::FilledRectangle;
    Renderer[ "FilledRectangle" ] = Renderer::FilledRectangle;
    Renderer[ "Gradient" ] = Renderer::Gradient;
    Renderer[ "FilledGradient" ] = Renderer::FilledGradient;
    Renderer[ "Gradient4" ] = Renderer::Gradient4;
    Renderer[ "FilledGradient4" ] = Renderer::FilledGradient4;
    Renderer[ "Triangle" ] = Renderer::Triangle;
    Renderer[ "FilledTriangle" ] = Renderer::FilledTriangle;
    Renderer[ "Circle" ] = Renderer::Circle;
    Renderer[ "FilledCircle" ] = Renderer::FilledCircle;
    Renderer[ "Text" ] = Renderer::Text;
    Renderer[ "GetTextSize" ] = Renderer::GetTextSize;
    Renderer[ "GetVerticesCount" ] = Renderer::GetVerticesCount;
    Renderer[ "GetIndicesCount" ] = Renderer::GetIndicesCount;
    Renderer[ "GetScreenSize" ] = Renderer::GetScreenSize;
    Renderer[ "GetDefaultFont" ] = Renderer::GetDefaultFont;
    Renderer[ "CreateFont" ] = Renderer::CreateFont_;

    auto Math = Lua.create_table( );
    Math[ "RandomFloat" ] = Math::RandomFloat;
    Math[ "RandomInt" ] = Math::RandomInt;

    auto Http = Lua.create_table( );
    Http[ "Get" ] = Http::Get;
    Http[ "Post" ] = Http::Post;
    Http[ "Put" ] = Http::Put;
    Http[ "Delete" ] = Http::Delete;

    auto Utils = Lua.create_table( );
    Http[ "Sha256Encode" ] = Utils::SHA256;
    Http[ "Base64Encode" ] = Utils::Base64Encode;
    Http[ "Base64Decode" ] = Utils::Base64Decode;

    Lua[ "Client" ] = Client;
    Lua[ "Input" ] = Input;
    Lua[ "Graphics" ] = Graphics;
    Lua[ "Renderer" ] = Renderer;
    Lua[ "Math" ] = Math;
    Lua[ "Http" ] = Http;
    Lua[ "Utils" ] = Utils;
}

int cWrapper::LoadScript( const std::string& source ) {
    if ( source.empty( ) )
        return 0;

    bool Error = false;

    Lua.safe_script( source, [ &Error ] ( lua_State*, sol::protected_function_result Result ) {
        if ( !Result.valid( ) ) {
            std::cout << static_cast< std::string >( Result.get<sol::error>( ).what( ) ) + "\n";
            Error = true;
        }
        return Result;
    } );

    if ( Error )
        return 0;

    return 1;
}

int cWrapper::LoadScriptFromFile( const std::string& file_name ) {
    std::string full_path = "Scripts/" + file_name;

    std::ifstream inFile( full_path );
    if ( !inFile.is_open( ) ) {
        MessageBox( NULL, "Failed to open Lua script file?", "FileSystem Error!", MB_OK | MB_ICONERROR );
        return false;
    }

    std::string scriptContent( ( std::istreambuf_iterator<char>( inFile ) ), std::istreambuf_iterator<char>( ) );
    inFile.close( );

    return LoadScript( scriptContent );
}

void cWrapper::RegisterCallback( std::string event_name, sol::protected_function Callback ) {
    this->m_Callbacks[ event_name ].push_back( cLuaCallback( Callback ) );
}

void cWrapper::UnregisterCallbacks( ) {
    if (!m_Callbacks.empty() )
        m_Callbacks.clear( );
}

std::vector<cLuaCallback> cWrapper::GetCallbacks( std::string event_name ) {
    return this->m_Callbacks[ event_name ];
}