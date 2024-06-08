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
    Lua.set( "KEY_LMOUSE", KEY_LMOUSE );
    Lua.set( "KEY_RMOUSE", KEY_RMOUSE );
    Lua.set( "KEY_CANCEL", KEY_CANCEL );
    Lua.set( "KEY_MMOUSE", KEY_MMOUSE );
    Lua.set( "KEY_MOUSE_5", KEY_MOUSE_5 );
    Lua.set( "KEY_MOUSE_6", KEY_MOUSE_6 );

    // Keyboard VK Keys
    Lua.set( "KEY_BACK", KEY_BACK );
    Lua.set( "KEY_TAB", KEY_TAB );
    Lua.set( "KEY_CLEAR", KEY_CLEAR );
    Lua.set( "KEY_RETURN", KEY_RETURN );
    Lua.set( "KEY_SHIFT", KEY_SHIFT );
    Lua.set( "KEY_CONTROL", KEY_CONTROL );
    Lua.set( "KEY_ALT", KEY_ALT );
    Lua.set( "KEY_PAUSE", KEY_PAUSE );
    Lua.set( "KEY_CAPSLOCK", KEY_CAPSLOCK );
    Lua.set( "KEY_ESCAPE", KEY_ESCAPE );
    Lua.set( "KEY_SPACE", KEY_SPACE );
    Lua.set( "KEY_PAGEUP", KEY_PAGEUP );
    Lua.set( "KEY_PAGEDOWN", KEY_PAGEDOWN );
    Lua.set( "KEY_END", KEY_END );
    Lua.set( "KEY_HOME", KEY_HOME );
    Lua.set( "KEY_LEFT_ARROW", KEY_LEFT_ARROW );
    Lua.set( "KEY_UP_ARROW", KEY_UP_ARROW );
    Lua.set( "KEY_RIGHT_ARROW", KEY_RIGHT_ARROW );
    Lua.set( "KEY_DOWN_ARROW", KEY_DOWN_ARROW );
    Lua.set( "KEY_SELECT", KEY_SELECT );
    Lua.set( "KEY_PRINT", KEY_PRINT );
    Lua.set( "KEY_EXECUTE", KEY_EXECUTE );
    Lua.set( "KEY_PRINT_SCREEN", KEY_PRINT_SCREEN );
    Lua.set( "KEY_INSERT", KEY_INSERT );
    Lua.set( "KEY_DELETE", KEY_DELETE );
    Lua.set( "KEY_HELP", KEY_HELP );
    Lua.set( "KEY_0", KEY_0 );
    Lua.set( "KEY_1", KEY_1 );
    Lua.set( "KEY_2", KEY_2 );
    Lua.set( "KEY_3", KEY_3 );
    Lua.set( "KEY_4", KEY_4 );
    Lua.set( "KEY_5", KEY_5 );
    Lua.set( "KEY_6", KEY_6 );
    Lua.set( "KEY_7", KEY_7 );
    Lua.set( "KEY_8", KEY_8 );
    Lua.set( "KEY_9", KEY_9 );
    Lua.set( "KEY_A", KEY_A );
    Lua.set( "KEY_B", KEY_B );
    Lua.set( "KEY_C", KEY_C );
    Lua.set( "KEY_D", KEY_D );
    Lua.set( "KEY_E", KEY_E );
    Lua.set( "KEY_F", KEY_F );
    Lua.set( "KEY_G", KEY_G );
    Lua.set( "KEY_H", KEY_H );
    Lua.set( "KEY_I", KEY_I );
    Lua.set( "KEY_J", KEY_J );
    Lua.set( "KEY_K", KEY_K );
    Lua.set( "KEY_L", KEY_L );
    Lua.set( "KEY_M", KEY_M );
    Lua.set( "KEY_N", KEY_N );
    Lua.set( "KEY_O", KEY_O );
    Lua.set( "KEY_P", KEY_P );
    Lua.set( "KEY_Q", KEY_Q );
    Lua.set( "KEY_R", KEY_R );
    Lua.set( "KEY_S", KEY_S );
    Lua.set( "KEY_T", KEY_T );
    Lua.set( "KEY_U", KEY_U );
    Lua.set( "KEY_V", KEY_V );
    Lua.set( "KEY_W", KEY_W );
    Lua.set( "KEY_X", KEY_X );
    Lua.set( "KEY_Y", KEY_Y );
    Lua.set( "KEY_Z", KEY_Z );
    Lua.set( "KEY_LEFT_WIN", KEY_LEFT_WIN );
    Lua.set( "KEY_RIGHT_WIN", KEY_RIGHT_WIN );
    Lua.set( "KEY_APPS", KEY_APPS );
    Lua.set( "KEY_SLEEP", KEY_SLEEP );
    Lua.set( "KEY_NUMPAD_0", KEY_NUMPAD_0 );
    Lua.set( "KEY_NUMPAD_1", KEY_NUMPAD_1 );
    Lua.set( "KEY_NUMPAD_2", KEY_NUMPAD_2 );
    Lua.set( "KEY_NUMPAD_3", KEY_NUMPAD_3 );
    Lua.set( "KEY_NUMPAD_4", KEY_NUMPAD_4 );
    Lua.set( "KEY_NUMPAD_5", KEY_NUMPAD_5 );
    Lua.set( "KEY_NUMPAD_6", KEY_NUMPAD_6 );
    Lua.set( "KEY_NUMPAD_7", KEY_NUMPAD_7 );
    Lua.set( "KEY_NUMPAD_8", KEY_NUMPAD_8 );
    Lua.set( "KEY_NUMPAD_9", KEY_NUMPAD_9 );
    Lua.set( "KEY_MULTIPLY", KEY_MULTIPLY );
    Lua.set( "KEY_ADD", KEY_ADD );
    Lua.set( "KEY_SEPARATOR", KEY_SEPARATOR );
    Lua.set( "KEY_SUBTRACT", KEY_SUBTRACT );
    Lua.set( "KEY_DECIMAL", KEY_DECIMAL );
    Lua.set( "KEY_DIVIDE", KEY_DIVIDE );
    Lua.set( "KEY_F1", KEY_F1 );
    Lua.set( "KEY_F2", KEY_F2 );
    Lua.set( "KEY_F3", KEY_F3 );
    Lua.set( "KEY_F4", KEY_F4 );
    Lua.set( "KEY_F5", KEY_F5 );
    Lua.set( "KEY_F6", KEY_F6 );
    Lua.set( "KEY_F7", KEY_F7 );
    Lua.set( "KEY_F8", KEY_F8 );
    Lua.set( "KEY_F9", KEY_F9 );
    Lua.set( "KEY_F10", KEY_F10 );
    Lua.set( "KEY_F11", KEY_F11 );
    Lua.set( "KEY_F12", KEY_F12 );
    Lua.set( "KEY_NUMLOCK", KEY_NUMLOCK );
    Lua.set( "KEY_SCROLL_LOCK", KEY_SCROLL_LOCK );
    Lua.set( "KEY_LSHIFT", KEY_LSHIFT );
    Lua.set( "KEY_RSHIFT", KEY_RSHIFT );
    Lua.set( "KEY_LCONTROL", KEY_LCONTROL );
    Lua.set( "KEY_RCONTROL", KEY_RCONTROL );
    Lua.set( "KEY_LMENU", KEY_LMENU );
    Lua.set( "KEY_RMENU", KEY_RMENU );
    Lua.set( "KEY_BROWSER_BACK", KEY_BROWSER_BACK );
    Lua.set( "KEY_BROWSER_FORWARD", KEY_BROWSER_FORWARD );
    Lua.set( "KEY_BROWSER_REFRESH", KEY_BROWSER_REFRESH );
    Lua.set( "KEY_BROWSER_STOP", KEY_BROWSER_STOP );
    Lua.set( "KEY_BROWSER_SEARCH", KEY_BROWSER_SEARCH );
    Lua.set( "KEY_BROWSER_FAVORITES", KEY_BROWSER_FAVORITES );
    Lua.set( "KEY_BROWSER_HOME", KEY_BROWSER_HOME );
    Lua.set( "KEY_VOLUME_MUTE", KEY_VOLUME_MUTE );
    Lua.set( "KEY_VOLUME_DOWN", KEY_VOLUME_DOWN );
    Lua.set( "KEY_VOLUME_UP", KEY_VOLUME_UP );
    Lua.set( "KEY_MEDIA_NEXT_TRACK", KEY_MEDIA_NEXT_TRACK );
    Lua.set( "KEY_MEDIA_PREV_TRACK", KEY_MEDIA_PREV_TRACK );
    Lua.set( "KEY_MEDIA_STOP", KEY_MEDIA_STOP );
    Lua.set( "KEY_MEDIA_PLAY_PAUSE", KEY_MEDIA_PLAY_PAUSE );
    Lua.set( "KEY_LAUNCH_MAIL", KEY_LAUNCH_MAIL );
    Lua.set( "KEY_LAUNCH_MEDIA_SELECT", KEY_LAUNCH_MEDIA_SELECT );
    Lua.set( "KEY_LAUNCH_APP1", KEY_LAUNCH_APP1 );
    Lua.set( "KEY_LAUNCH_APP2", KEY_LAUNCH_APP2 );
    Lua.set( "KEY_OEM_1", KEY_OEM_1 );
    Lua.set( "KEY_OEM_PLUS", KEY_OEM_PLUS );
    Lua.set( "KEY_OEM_COMMA", KEY_OEM_COMMA );
    Lua.set( "KEY_OEM_MINUS", KEY_OEM_MINUS );
    Lua.set( "KEY_OEM_PERIOD", KEY_OEM_PERIOD );
    Lua.set( "KEY_OEM_2", KEY_OEM_2 );
    Lua.set( "KEY_OEM_3", KEY_OEM_3 );
    Lua.set( "KEY_OEM_4", KEY_OEM_4 );
    Lua.set( "KEY_OEM_5", KEY_OEM_5 );
    Lua.set( "KEY_OEM_6", KEY_OEM_6 );
    Lua.set( "KEY_OEM_7", KEY_OEM_7 );
    Lua.set( "KEY_OEM_8", KEY_OEM_8 );
    Lua.set( "KEY_OEM_102", KEY_OEM_102 );
    Lua.set( "KEY_PROCESSKEY", KEY_PROCESSKEY );
    Lua.set( "KEY_PACKET", KEY_PACKET );
    Lua.set( "KEY_ATTN", KEY_ATTN );
    Lua.set( "KEY_CRSEL", KEY_CRSEL );
    Lua.set( "KEY_EXSEL", KEY_EXSEL );
    Lua.set( "KEY_EREOF", KEY_EREOF );
    Lua.set( "KEY_PLAY", KEY_PLAY );
    Lua.set( "KEY_ZOOM", KEY_ZOOM );
    Lua.set( "KEY_NONAME", KEY_NONAME );
    Lua.set( "KEY_PA1", KEY_PA1 );
    Lua.set( "KEY_OEM_CLEAR", KEY_OEM_CLEAR );

    // Alias VK Keys
    Lua.set( "KEY_ENTER", KEY_ENTER );
    Lua.set( "KEY_BACKSPACE", KEY_BACKSPACE );
    Lua.set( "KEY_LWIN", KEY_LWIN );
    Lua.set( "KEY_RWIN", KEY_RWIN );
    Lua.set( "KEY_CONTEXT_MENU", KEY_CONTEXT_MENU );
    Lua.set( "KEY_PAUSE_BREAK", KEY_PAUSE_BREAK );
    Lua.set( "KEY_PAGE_UP", KEY_PAGE_UP );
    Lua.set( "KEY_PAGE_DOWN", KEY_PAGE_DOWN );
    Lua.set( "KEY_UP", KEY_UP );
    Lua.set( "KEY_DOWN", KEY_DOWN );
    Lua.set( "KEY_LEFT", KEY_LEFT );
    Lua.set( "KEY_RIGHT", KEY_RIGHT );
    Lua.set( "KEY_DECIMAL_POINT", KEY_DECIMAL_POINT );
    Lua.set( "KEY_NUM_LOCK", KEY_NUM_LOCK );
    Lua.set( "KEY_CTRL_LEFT", KEY_CTRL_LEFT );
    Lua.set( "KEY_CTRL_RIGHT", KEY_CTRL_RIGHT );
    Lua.set( "KEY_SHIFT_LEFT", KEY_SHIFT_LEFT );
    Lua.set( "KEY_SHIFT_RIGHT", KEY_SHIFT_RIGHT );
    Lua.set( "KEY_ALT_LEFT", KEY_ALT_LEFT );
    Lua.set( "KEY_ALT_RIGHT", KEY_ALT_RIGHT );
    Lua.set( "KEY_MUTE", KEY_MUTE );
    Lua.set( "KEY_VOL_DOWN", KEY_VOL_DOWN );
    Lua.set( "KEY_VOL_UP", KEY_VOL_UP );
    Lua.set( "KEY_NEXT_TRACK", KEY_NEXT_TRACK );
    Lua.set( "KEY_PREV_TRACK", KEY_PREV_TRACK );
    Lua.set( "KEY_STOP_MEDIA", KEY_STOP_MEDIA );
    Lua.set( "KEY_STOP_MEDIA", KEY_STOP_MEDIA );
    Lua.set( "KEY_EMAIL", KEY_EMAIL );
    Lua.set( "KEY_CALCULATOR", KEY_CALCULATOR );
    Lua.set( "KEY_MY_COMPUTER", KEY_MY_COMPUTER );
    Lua.set( "KEY_COLON", KEY_COLON );
    Lua.set( "KEY_PLUS", KEY_PLUS );
    Lua.set( "KEY_COMMA", KEY_COMMA );
    Lua.set( "KEY_MINUS", KEY_MINUS );
    Lua.set( "KEY_PERIOD", KEY_PERIOD );
    Lua.set( "KEY_SLASH", KEY_SLASH );
    Lua.set( "KEY_TILDE", KEY_TILDE );
    Lua.set( "KEY_LEFT_BRACKET", KEY_LEFT_BRACKET );
    Lua.set( "KEY_BACKSLASH", KEY_BACKSLASH );
    Lua.set( "KEY_RIGHT_BRACKET", KEY_RIGHT_BRACKET );
    Lua.set( "KEY_QUOTE", KEY_QUOTE );
    Lua.set( "KEY_LESS_THAN", KEY_LESS_THAN );

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