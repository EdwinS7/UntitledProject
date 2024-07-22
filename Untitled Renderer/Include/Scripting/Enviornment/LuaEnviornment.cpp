#include "../LuaAPI.hpp"
#include "LuaEnviornment.hpp"

// Lua API function definitions.
#include "Definitions/FunctionWrapper.hpp"

int LuaExceptionHandler( lua_State* L, sol::optional<const std::exception&> exception, sol::string_view description ) {
	if ( exception.has_value( ) )
		gLogger->Log( LogLevel::Error, std::format( "(straight from the exception): {}", exception->what( ) ) );
	else
		gLogger->Log( LogLevel::Information, std::format( "(from the description parameter): {}", description ) );

	return sol::stack::push( L, description );
}

inline void LuaPanicHandler( sol::optional<std::string> message ) {
    gLogger->Log( LogLevel::Error, "Lua panic! This is a fatal error in the Lua state and will now abort() the application" );

    if ( message.has_value( ) )
        gLogger->Log( LogLevel::Error, std::format( "Lua Panic: ", message.value( ) ) );
}

void cLuaEnviornment::Init( ) {
    Lua = sol::state( sol::c_call<decltype( &LuaPanicHandler ), &LuaPanicHandler> );
    
    Lua.open_libraries(
        sol::lib::base, sol::lib::package, sol::lib::coroutine,
        sol::lib::string, sol::lib::math, sol::lib::table,
        sol::lib::debug, sol::lib::bit32, sol::lib::utf8,
        sol::lib::ffi, sol::lib::jit
    );

    Lua[ "package" ][ "path" ] = ( gFileSystem->GetExecutableDirectory( ) + FS_LIBRARY_SCRIPTS_FOLDER ) + "/?.lua";

    std::vector<std::string> UnregisterFunctions = {
        "collectgarbage", "dofilsse", "load",
        "loadfile", "pcall", "print", "xpcall",
        "getmetatable", "setmetatable", "__nil_callback"
    };

    for ( auto& function : UnregisterFunctions ) {
        Lua[ function ] = sol::nil;
    }

    Lua.new_usertype<Vec2<int16_t>>(
        "Vector2", sol::constructors<Vec2<int16_t>( ), Vec2<int16_t>( int16_t, int16_t )>( ),
        "x", &Vec2<int16_t>::x, "y", &Vec2<int16_t>::y,
        "Min", &Vec2<int16_t>::Min, "Max", &Vec2<int16_t>::Max,
        "Lerp", &Vec2<int16_t>::Lerp, "DistanceTo", &Vec2<int16_t>::DistanceTo,
        "__add", &Vec2<int16_t>::operator+, "__sub", &Vec2<int16_t>::operator-,
        "__mul", &Vec2<int16_t>::operator*, "__div", &Vec2<int16_t>::operator/,
        "__iadd", &Vec2<int16_t>::operator+=, "__isub", &Vec2<int16_t>::operator-=,
        "__imul", &Vec2<int16_t>::operator*=, "__idiv", &Vec2<int16_t>::operator/=,
        "__lt", &Vec2<int16_t>::operator<, "__le", &Vec2<int16_t>::operator<=,
        "__gt", &Vec2<int16_t>::operator>, "__ge", &Vec2<int16_t>::operator>=,
        "__eq", &Vec2<int16_t>::operator==
    );

    Lua.new_usertype <Vec3<int>>(
        "Vector3", sol::constructors<Vec3<int>( ), Vec3<int>( int, int, int )>( ),
        "x", &Vec3<int>::x, "y", &Vec3<int>::y, "z", &Vec3<int>::z,
        "Min", &Vec3<int>::Min, "Max", &Vec3<int>::Max,
        "Lerp", &Vec3<int>::Lerp, "DistanceTo", &Vec3<int>::DistanceTo,
        "__add", &Vec3<int>::operator+, "__sub", &Vec3<int>::operator-,
        "__mul", &Vec3<int>::operator*, "__div", &Vec3<int>::operator/,
        "__iadd", &Vec3<int>::operator+=, "__isub", &Vec3<int>::operator-=,
        "__imul", &Vec3<int>::operator*=, "__idiv", &Vec3<int>::operator/=,
        "__lt", &Vec3<int>::operator<, "__le", &Vec3<int>::operator<=,
        "__gt", &Vec3<int>::operator>, "__ge", &Vec3<int>::operator>=,
        "__eq", &Vec3<int>::operator==
    );

    Lua.new_usertype <Vec4<int>>( 
        "Vector4", sol::constructors<Vec4<int>( ), Vec4<int>( int, int, int, int )>( ),
        "x", &Vec4<int>::x, "y", &Vec4<int>::y, "z", &Vec4<int>::z, "w", &Vec4<int>::w,
        "Min", &Vec4<int>::Min, "Max", &Vec4<int>::Max,
        "Lerp", &Vec4<int>::Lerp, "DistanceTo", &Vec4<int>::DistanceTo,
        "__add", &Vec4<int>::operator+, "__sub", &Vec4<int>::operator-,
        "__mul", &Vec4<int>::operator*, "__div", &Vec4<int>::operator/,
        "__iadd", &Vec4<int>::operator+=, "__isub", &Vec4<int>::operator-=,
        "__imul", &Vec4<int>::operator*=, "__idiv", &Vec4<int>::operator/=,
        "__lt", &Vec4<int>::operator<, "__le", &Vec4<int>::operator<=,
        "__gt", &Vec4<int>::operator>, "__ge", &Vec4<int>::operator>=,
        "__eq", &Vec4<int>::operator==
    );

    Lua.new_usertype <Rect<int>>( 
        "Rect", sol::constructors<Rect<int>( ), Rect<int16_t>( int16_t, int16_t, int16_t, int16_t )>( ),
        "x", &Rect<int>::x, "y", &Rect<int>::y, "w", &Rect<int>::w, "h", &Rect<int>::h,
        "Min", &Rect<int>::Min, "Max", &Rect<int>::Max, 
        "Lerp", &Rect<int>::Lerp, "DistanceTo", &Rect<int>::DistanceTo,
        "__add", &Rect<int>::operator+, "__sub", &Rect<int>::operator-,
        "__mul", &Rect<int>::operator*, "__div", &Rect<int>::operator/,
        "__iadd", &Rect<int>::operator+=, "__isub", &Rect<int>::operator-=,
        "__imul", &Rect<int>::operator*=, "__idiv", &Rect<int>::operator/=,
        "__lt", &Rect<int>::operator<, "__le", &Rect<int>::operator<=,
        "__gt", &Rect<int>::operator>, "__ge", &Rect<int>::operator>=,
        "__eq", &Rect<int>::operator==
    );

    Lua.new_usertype<Color>( 
        "Color", sol::constructors<Color( ), Color( uint8_t, uint8_t, uint8_t, uint8_t )>( ),
        "Hex", &Color::Hex, "r", sol::property( &Color::GetR ), "g", sol::property( &Color::GetG ),
        "b", sol::property( &Color::GetB ), "a", sol::property( &Color::GetA )
    );

	// Global functions
    Lua[ "Connect" ] = Globals::Connect;
    Lua[ "print" ] = Globals::Print;

    auto Client = Lua.create_table( );
    Client[ "Log" ] = Client::Log;
    Client[ "GetLogs" ] = Client::GetLogs;
    Client[ "ClearLogs" ] = Client::ClearLogs;
    Client[ "GetUsername" ] = Client::GetUsername;
    Client[ "GetFramerate" ] = Client::GetFPS;
    Client[ "GetRealtime" ] = Client::GetRealTime;
    Client[ "GetDeltaTime" ] = Client::GetDeltaTime;
    Client[ "GetUsableFonts" ] = Client::GetUsableFonts;

    auto Input = Lua.create_table( );
    Input[ "IsMouseHoveringRect" ] = Input::IsMouseHoveringRect;
    Input[ "GetMousePosition" ] = Input::GetMousePosition;
    Input[ "SetMousePosition" ] = Input::SetMousePosition;
    Input[ "IsAnyKeyPressed" ] = Input::IsAnyKeyPressed;
    Input[ "IsKeyPressed" ] = Input::IsKeyPressed;
    Input[ "IsKeyHeld" ] = Input::IsKeyHeld;
    Input[ "SetCursorStyle" ] = Input::SetCursorStyle;
    Input[ "GetCursorStyle" ] = Input::GetCursorStyle;

    auto Window = Lua.create_table( );
    Window[ "SetFullscreen" ] = Window::SetFullscreen;
    Window[ "GetFullscreen" ] = Window::GetFullscreen;
    Window[ "SetSize" ] = Window::SetSize;
    Window[ "GetSize" ] = Window::GetSize;
    Window[ "SetPos" ] = Window::SetPos;
    Window[ "GetPos" ] = Window::GetPos;

    auto Graphics = Lua.create_table( );
    Graphics[ "SetVerticalSync" ] = Graphics::SetVerticalSync;
    Graphics[ "GetVerticalSync" ] = Graphics::GetVerticalSync;
    Graphics[ "SetClearColor" ] = Graphics::SetClearColor;
    Graphics[ "GetClearColor" ] = Graphics::GetClearColor;

    auto Renderer = Lua.create_table( );
    Renderer[ "Line" ] = Renderer::Line;
    Renderer[ "Polyline" ] = Renderer::Polyline;
    Renderer[ "Polygon" ] = Renderer::Polygon;
    Renderer[ "Rectangle" ] = Renderer::Rectangle;
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
    Renderer[ "GetDefaultFont" ] = Renderer::GetDefaultFont;
    Renderer[ "CreateFont" ] = Renderer::CreateFont_;
    Renderer[ "GetVerticesCount" ] = Renderer::GetVerticesCount;
    Renderer[ "GetIndicesCount" ] = Renderer::GetIndicesCount;
    Renderer[ "GetScreenSize" ] = Renderer::GetScreenSize;
    Renderer[ "PushClip" ] = Renderer::PushClip;
    Renderer[ "PopClip" ] = Renderer::PopClip;

    auto Animations = Lua.create_table( );
    Animations[ "Lerp" ] = Animations::Lerp;
    Animations[ "Coserp" ] = Animations::Coserp;
    Animations[ "Smoothstep" ] = Animations::Smoothstep;

    auto Math = Lua.create_table( );
    Math[ "RandomFloat" ] = Math::RandomFloat;
    Math[ "RandomInt" ] = Math::RandomInt;

    auto Http = Lua.create_table( );
    Http[ "Get" ] = Http::Get;
    Http[ "Post" ] = Http::Post;
    Http[ "Put" ] = Http::Put;
    Http[ "Delete" ] = Http::Delete;

    auto Json = Lua.create_table( );
    Json[ "Serialize" ] = Json::Serialize;
    Json[ "Deserialize" ] = Json::Deserialize;

    auto Utils = Lua.create_table( );
    Utils[ "Sha256Encode" ] = Utils::SHA256;
    Utils[ "Base64Encode" ] = Utils::Base64Encode;
    Utils[ "Base64Decode" ] = Utils::Base64Decode;

    Lua[ "Client" ] = Client;
    Lua[ "Input" ] = Input;
    Lua[ "Window" ] = Window;
    Lua[ "Graphics" ] = Graphics;
    Lua[ "Renderer" ] = Renderer;
    Lua[ "Animations" ] = Animations;
    Lua[ "Math" ] = Math;
    Lua[ "Http" ] = Http;
    Lua[ "Json" ] = Json;
    Lua[ "Utils" ] = Utils;
}

int cLuaEnviornment::LoadScript( const std::string& source ) {
    if ( source.empty( ) )
        return 0;

    bool error = false;

    Lua.safe_script( source, [ &error ] ( lua_State*, sol::protected_function_result result ) {
        if ( !result.valid( ) ) {
            gLogger->Log( LogLevel::Error, result.get<sol::error>( ).what( ) );
            error = true;
        }
        return result;
    } );

    if ( error )
        return 0;

    return 1;
}

int cLuaEnviornment::LoadScriptFromFile( const std::string& folder_path, const std::string& file_name ) {
    return LoadScript( gFileSystem->GetFileContent( folder_path, file_name ) );
}