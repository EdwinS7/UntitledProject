#include "Wrapper.hpp"
#include "Features.hpp"

int lua_exception_handler( lua_State* L,
    sol::optional<const std::exception&> maybe_exception, sol::string_view description )
{
    if ( maybe_exception )
    {
        // const std::exception& ex = *maybe_exception;
        // obe::Debug::Log->error("<LuaError>[Exception] : {}", ex.what());
        std::cout << "(straight from the exception): ";
        const std::exception& ex = *maybe_exception;
        std::cout << ex.what( ) << std::endl;
    }
    else
    {
        //obe::Debug::Log->error("<LuaError>[Error] : {}", description);
        std::cout << "(from the description parameter): ";
        std::cout.write( description.data( ), static_cast< std::streamsize >( description.size( ) ) );
        std::cout << std::endl;
    }
    return sol::stack::push( L, description );
}

inline void lua_panic_handler( sol::optional<std::string> maybe_msg ) {
    std::cerr << "Lua is in a panic state and will now abort() the application" << std::endl;
    if ( maybe_msg ) {
        const std::string& msg = maybe_msg.value( );
        std::cerr << "\terror message: " << msg << std::endl;
    }
    // When this function exits, Lua will exhibit default behavior and abort()
}



void cWrapper::Init( ) {
    Lua = sol::state( sol::c_call<decltype( &lua_panic_handler ), &lua_panic_handler> );

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


    // Mouse VK Keys
    Lua.set( "KEY_LMOUSE", 0x01 );
    Lua.set( "KEY_RMOUSE", 0x02 );
    Lua.set( "KEY_CANCEL", 0x03 );
    Lua.set( "KEY_MMOUSE", 0x04 );
    Lua.set( "KEY_MOUSE_5", 0x05 );
    Lua.set( "KEY_MOUSE_6", 0x06 );

    // Keyboard VK Keys
    Lua.set( "KEY_BACK", 0x08 );
    Lua.set( "KEY_TAB", 0x09 );
    Lua.set( "KEY_CLEAR", 0x0C );
    Lua.set( "KEY_RETURN", 0x0D );
    Lua.set( "KEY_SHIFT", 0x10 );
    Lua.set( "KEY_CONTROL", 0x11 );
    Lua.set( "KEY_ALT", 0x12 );
    Lua.set( "KEY_PAUSE", 0x13 );
    Lua.set( "KEY_CAPSLOCK", 0x14 );
    Lua.set( "KEY_ESCAPE", 0x1B );
    Lua.set( "KEY_SPACE", 0x20 );
    Lua.set( "KEY_PAGEUP", 0x21 );
    Lua.set( "KEY_PAGEDOWN", 0x22 );
    Lua.set( "KEY_END", 0x23 );
    Lua.set( "KEY_HOME", 0x24 );
    Lua.set( "KEY_LEFT_ARROW", 0x25 );
    Lua.set( "KEY_UP_ARROW", 0x26 );
    Lua.set( "KEY_RIGHT_ARROW", 0x27 );
    Lua.set( "KEY_DOWN_ARROW", 0x28 );
    Lua.set( "KEY_SELECT", 0x29 );
    Lua.set( "KEY_PRINT", 0x2A );
    Lua.set( "KEY_EXECUTE", 0x2B );
    Lua.set( "KEY_PRINT_SCREEN", 0x2C );
    Lua.set( "KEY_INSERT", 0x2D );
    Lua.set( "KEY_DELETE", 0x2E );
    Lua.set( "KEY_HELP", 0x2F );
    Lua.set( "KEY_0", 0x30 );
    Lua.set( "KEY_1", 0x31 );
    Lua.set( "KEY_2", 0x32 );
    Lua.set( "KEY_3", 0x33 );
    Lua.set( "KEY_4", 0x34 );
    Lua.set( "KEY_5", 0x35 );
    Lua.set( "KEY_6", 0x36 );
    Lua.set( "KEY_7", 0x37 );
    Lua.set( "KEY_8", 0x38 );
    Lua.set( "KEY_9", 0x39 );
    Lua.set( "KEY_A", 0x41 );
    Lua.set( "KEY_B", 0x42 );
    Lua.set( "KEY_C", 0x43 );
    Lua.set( "KEY_D", 0x44 );
    Lua.set( "KEY_E", 0x45 );
    Lua.set( "KEY_F", 0x46 );
    Lua.set( "KEY_G", 0x47 );
    Lua.set( "KEY_H", 0x48 );
    Lua.set( "KEY_I", 0x49 );
    Lua.set( "KEY_J", 0x4A );
    Lua.set( "KEY_K", 0x4B );
    Lua.set( "KEY_L", 0x4C );
    Lua.set( "KEY_M", 0x4D );
    Lua.set( "KEY_N", 0x4E );
    Lua.set( "KEY_O", 0x4F );
    Lua.set( "KEY_P", 0x50 );
    Lua.set( "KEY_Q", 0x51 );
    Lua.set( "KEY_R", 0x52 );
    Lua.set( "KEY_S", 0x53 );
    Lua.set( "KEY_T", 0x54 );
    Lua.set( "KEY_U", 0x55 );
    Lua.set( "KEY_V", 0x56 );
    Lua.set( "KEY_W", 0x57 );
    Lua.set( "KEY_X", 0x58 );
    Lua.set( "KEY_Y", 0x59 );
    Lua.set( "KEY_Z", 0x5A );
    Lua.set( "KEY_LEFT_WIN", 0x5B );
    Lua.set( "KEY_RIGHT_WIN", 0x5C );
    Lua.set( "KEY_APPS", 0x5D );
    Lua.set( "KEY_SLEEP", 0x5F );
    Lua.set( "KEY_NUMPAD_0", 0x60 );
    Lua.set( "KEY_NUMPAD_1", 0x61 );
    Lua.set( "KEY_NUMPAD_2", 0x62 );
    Lua.set( "KEY_NUMPAD_3", 0x63 );
    Lua.set( "KEY_NUMPAD_4", 0x64 );
    Lua.set( "KEY_NUMPAD_5", 0x65 );
    Lua.set( "KEY_NUMPAD_6", 0x66 );
    Lua.set( "KEY_NUMPAD_7", 0x67 );
    Lua.set( "KEY_NUMPAD_8", 0x68 );
    Lua.set( "KEY_NUMPAD_9", 0x69 );
    Lua.set( "KEY_MULTIPLY", 0x6A );
    Lua.set( "KEY_ADD", 0x6B );
    Lua.set( "KEY_SEPARATOR", 0x6C );
    Lua.set( "KEY_SUBTRACT", 0x6D );
    Lua.set( "KEY_DECIMAL", 0x6E );
    Lua.set( "KEY_DIVIDE", 0x6F );
    Lua.set( "KEY_F1", 0x70 );
    Lua.set( "KEY_F2", 0x71 );
    Lua.set( "KEY_F3", 0x72 );
    Lua.set( "KEY_F4", 0x73 );
    Lua.set( "KEY_F5", 0x74 );
    Lua.set( "KEY_F6", 0x75 );
    Lua.set( "KEY_F7", 0x76 );
    Lua.set( "KEY_F8", 0x77 );
    Lua.set( "KEY_F9", 0x78 );
    Lua.set( "KEY_F10", 0x79 );
    Lua.set( "KEY_F11", 0x7A );
    Lua.set( "KEY_F12", 0x7B );
    Lua.set( "KEY_NUMLOCK", 0x90 );
    Lua.set( "KEY_SCROLL_LOCK", 0x91 );
    Lua.set( "KEY_LSHIFT", 0xA0 );
    Lua.set( "KEY_RSHIFT", 0xA1 );
    Lua.set( "KEY_LCONTROL", 0xA2 );
    Lua.set( "KEY_RCONTROL", 0xA3 );
    Lua.set( "KEY_LMENU", 0xA4 );
    Lua.set( "KEY_RMENU", 0xA5 );
    Lua.set( "KEY_BROWSER_BACK", 0xA6 );
    Lua.set( "KEY_BROWSER_FORWARD", 0xA7 );
    Lua.set( "KEY_BROWSER_REFRESH", 0xA8 );
    Lua.set( "KEY_BROWSER_STOP", 0xA9 );
    Lua.set( "KEY_BROWSER_SEARCH", 0xAA );
    Lua.set( "KEY_BROWSER_FAVORITES", 0xAB );
    Lua.set( "KEY_BROWSER_HOME", 0xAC );
    Lua.set( "KEY_VOLUME_MUTE", 0xAD );
    Lua.set( "KEY_VOLUME_DOWN", 0xAE );
    Lua.set( "KEY_VOLUME_UP", 0xAF );
    Lua.set( "KEY_MEDIA_NEXT_TRACK", 0xB0 );
    Lua.set( "KEY_MEDIA_PREV_TRACK", 0xB1 );
    Lua.set( "KEY_MEDIA_STOP", 0xB2 );
    Lua.set( "KEY_MEDIA_PLAY_PAUSE", 0xB3 );
    Lua.set( "KEY_LAUNCH_MAIL", 0xB4 );
    Lua.set( "KEY_LAUNCH_MEDIA_SELECT", 0xB5 );
    Lua.set( "KEY_LAUNCH_APP1", 0xB6 );
    Lua.set( "KEY_LAUNCH_APP2", 0xB7 );
    Lua.set( "KEY_OEM_1", 0xBA );
    Lua.set( "KEY_OEM_PLUS", 0xBB );
    Lua.set( "KEY_OEM_COMMA", 0xBC );
    Lua.set( "KEY_OEM_MINUS", 0xBD );
    Lua.set( "KEY_OEM_PERIOD", 0xBE );
    Lua.set( "KEY_OEM_2", 0xBF );
    Lua.set( "KEY_OEM_3", 0xC0 );
    Lua.set( "KEY_OEM_4", 0xDB );
    Lua.set( "KEY_OEM_5", 0xDC );
    Lua.set( "KEY_OEM_6", 0xDD );
    Lua.set( "KEY_OEM_7", 0xDE );
    Lua.set( "KEY_OEM_8", 0xDF );
    Lua.set( "KEY_OEM_102", 0xE2 );
    Lua.set( "KEY_PROCESSKEY", 0xE5 );
    Lua.set( "KEY_PACKET", 0xE7 );
    Lua.set( "KEY_ATTN", 0xF6 );
    Lua.set( "KEY_CRSEL", 0xF7 );
    Lua.set( "KEY_EXSEL", 0xF8 );
    Lua.set( "KEY_EREOF", 0xF9 );
    Lua.set( "KEY_PLAY", 0xFA );
    Lua.set( "KEY_ZOOM", 0xFB );
    Lua.set( "KEY_NONAME", 0xFC );
    Lua.set( "KEY_PA1", 0xFD );
    Lua.set( "KEY_OEM_CLEAR", 0xFE );


    // Alias VK Keys
    Lua.set( "KEY_ENTER", 0x0D );
    Lua.set( "KEY_BACKSPACE", 0x08 );
    Lua.set( "KEY_LWIN", 0x5B );
    Lua.set( "KEY_RWIN", 0x5C );
    Lua.set( "KEY_CONTEXT_MENU", 0x5D );
    Lua.set( "KEY_PAUSE_BREAK", 0x13 );
    Lua.set( "KEY_PAGE_UP", 0x21 );
    Lua.set( "KEY_PAGE_DOWN", 0x22 );
    Lua.set( "KEY_UP", 0x26 );
    Lua.set( "KEY_DOWN", 0x28 );
    Lua.set( "KEY_LEFT", 0x25 );
    Lua.set( "KEY_RIGHT", 0x27 );
    Lua.set( "KEY_DECIMAL_POINT", 0x6E );
    Lua.set( "KEY_NUM_LOCK", 0x90 );
    Lua.set( "KEY_CTRL_LEFT", 0xA2 );
    Lua.set( "KEY_CTRL_RIGHT", 0xA3 );
    Lua.set( "KEY_SHIFT_LEFT", 0xA0 );
    Lua.set( "KEY_SHIFT_RIGHT", 0xA1 );
    Lua.set( "KEY_ALT_LEFT", 0xA4 );
    Lua.set( "KEY_ALT_RIGHT", 0xA5 );
    Lua.set( "KEY_MUTE", 0xAD );
    Lua.set( "KEY_VOL_DOWN", 0xAE );
    Lua.set( "KEY_VOL_UP", 0xAF );
    Lua.set( "KEY_NEXT_TRACK", 0xB0 );
    Lua.set( "KEY_PREV_TRACK", 0xB1 );
    Lua.set( "KEY_STOP_MEDIA", 0xB2 );
    Lua.set( "KEY_PLAY_PAUSE", 0xB3 );
    Lua.set( "KEY_EMAIL", 0xB4 );
    Lua.set( "KEY_CALCULATOR", 0xB6 );
    Lua.set( "KEY_MY_COMPUTER", 0xB7 );
    Lua.set( "KEY_COLON", 0xBA );
    Lua.set( "KEY_PLUS", 0xBB );
    Lua.set( "KEY_COMMA", 0xBC );
    Lua.set( "KEY_MINUS", 0xBD );
    Lua.set( "KEY_PERIOD", 0xBE );
    Lua.set( "KEY_SLASH", 0xBF );
    Lua.set( "KEY_TILDE", 0xC0 );
    Lua.set( "KEY_LEFT_BRACKET", 0xDB );
    Lua.set( "KEY_BACKSLASH", 0xDC );
    Lua.set( "KEY_RIGHT_BRACKET", 0xDD );
    Lua.set( "KEY_QUOTE", 0xDE );
    Lua.set( "KEY_LESS_THAN", 0xE2 );

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

    auto Audio = Lua.create_table( );
    Audio[ "OpenSound" ] = Audio::OpenSound;
    Audio[ "PlaySound" ] = Audio::PlaySound_;
    Audio[ "StopSound" ] = Audio::StopSound;
    Audio[ "StopAllSounds" ] = Audio::StopAllSounds;

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

    auto Json = Lua.create_table( );
    Json[ "Serialize" ] = Json::Serialize;
    Json[ "Deserialize" ] = Json::Deserialize;

    auto Utils = Lua.create_table( );
    Utils[ "Sha256Encode" ] = Utils::SHA256;
    Utils[ "Base64Encode" ] = Utils::Base64Encode;
    Utils[ "Base64Decode" ] = Utils::Base64Decode;

    Lua[ "Client" ] = Client;
    Lua[ "Audio" ] = Audio;
    Lua[ "Input" ] = Input;
    Lua[ "Graphics" ] = Graphics;
    Lua[ "Renderer" ] = Renderer;
    Lua[ "Math" ] = Math;
    Lua[ "Http" ] = Http;
    Lua[ "Json" ] = Json;
    Lua[ "Utils" ] = Utils;
}

std::string FormatSolError( const std::string& error_message ) {
    std::string ErrorMessage = error_message.substr( 9 );

    std::smatch LineMatch;
    std::regex LineRegex( ":([0-9]+):" );
    if ( std::regex_search( ErrorMessage, LineMatch, LineRegex ) ) {
        std::string LineNum = LineMatch.str( 1 );
        ErrorMessage = "Lua Error (" + LineNum + "): " + ErrorMessage;
    }

    std::regex_replace( ErrorMessage, std::regex( " near " ), ": " );

    return ErrorMessage;
}

int cWrapper::LoadScript( const std::string& source ) {
    if ( source.empty( ) )
        return 0;

    bool Error = false;

    Lua.script( source, [ &Error ] ( lua_State*, sol::protected_function_result Result ) {
        if ( !Result.valid( ) ) {
            std::cout << FormatSolError( static_cast< std::string >( Result.get<sol::error>( ).what( ) ) ) << "\n";
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