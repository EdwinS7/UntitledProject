#pragma once

#include "LuaWrapper.hpp"

namespace Client {
    void Log( LogLevel log_level, const std::string& message, sol::variadic_args args ) {
        std::ostringstream oss;
        oss << message;

        for ( auto arg : args )
            oss << arg.as<std::string>( );

        gLogger->Log( log_level, oss.str( ) );
    }

    void ClearLogs( LogLevel log_level ) {
        gLogger->ClearLogs( log_level );
    }

    // Helper function for GetLogs, you can't access pair's in lua I believe so we (ChatGPT) did this.
    sol::table PairsToLua( sol::state_view lua, const std::vector<std::pair<DWORD, std::string>>& vec ) {
        sol::table Table = lua.create_table( vec.size( ), 0 );

        for ( std::size_t i = 0; i < vec.size( ); ++i ) {
            sol::table Entry = lua.create_table( 0, 2 );
            Entry[ "color" ] = vec[ i ].first;
            Entry[ "message" ] = vec[ i ].second;
            Table[ i + 1 ] = Entry;
        }

        return Table;
    }

    sol::table GetLogs( sol::this_state s, LogLevel log_level ) {
        auto logs = gLogger->GetLogs( log_level );
        return PairsToLua( sol::state_view( s ), logs );
    }

    std::string GetUsername( ) {
        return gContext->GetUsername( );
    }

    int GetFPS( ) {
        return gContext->GetFrameRate( );
    }

    float GetRealTime( ) {
        return gContext->GetRealTime( );
    }

    float GetDeltaTime( ) {
        return gContext->GetDeltaTime( );
    }

    std::vector<std::string> GetFontList( ) {
        return gGraphics->RegistryFontList;
    }
};

namespace Audio {
    
};

namespace Input {
    bool IsMouseHoveringRect( const Vec2<int16_t> position, const Vec2<int16_t> size ) {
        return gInput->MouseHoveringRect( position, size );
    }

    Vec2<int16_t> GetMousePosition( ) {
        return gInput->GetMousePos( );
    }

    void SetMousePosition( Vec2<int16_t> position ) {
        return gInput->SetMousePos( position );
    }

    bool IsAnyKeyPressed( ) {
        return gInput->AnyKeyPressed( );
    }

    bool IsKeyPressed( int8_t key ) {
        return gInput->KeyPressed( key );
    }

    bool IsKeyHeld( int8_t key ) {
        return gInput->KeyHeld( key );
    }

    // @Todo: Define cursor styles!!
    void SetCursorStyle( LPSTR cursor_style ) {
        gInput->SetCursorStyle( cursor_style );
    }

    LPSTR GetCursorStyle( ) {
        return gInput->GetCursorStyle( );
    }
};

namespace Window {
    void SetFullscreen( bool fullscreen ) {
        gWindow->SetFullscreen( fullscreen );
    }

    bool GetFullscreen( ) {
        return gWindow->GetFullscreen( );
    }

    void SetPos( Vec2<int16_t>& position )  {
        gWindow->SetPos( position );
    }

    Vec2<int16_t> GetPos( ) {
        return gWindow->GetPos( );
    }

    void SetSize( Vec2<int16_t>& size ) {
        gWindow->SetSize( size );
    }

    Vec2<int16_t> GetSize( ) {
        return gWindow->GetSize( );
    }
};

namespace Graphics {
    void SetVerticalSync( const bool vertical_sync ) {
        gGraphics->SetVerticalSync( vertical_sync );
    }

    bool GetVerticalSync( ) {
        return gGraphics->GetVerticalSync( );
    }

    void SetClearColor( const Color clear_color ) {
        gGraphics->SetClearColor( clear_color );
    }

    Color GetClearColor( ) {
        return gGraphics->GetClearColor( );
    }
};

namespace Renderer {
    void Line( Vec2<int16_t> from, Vec2<int16_t> to, Color color ) {
        gBuffer->Line( from, to, color );
    }

    void Polyline( const sol::table& points, Color color ) {
        std::vector<Vec2<int16_t>> Points;

        for ( const auto& kv_pair : points ) {
            int16_t x = kv_pair.second.as<sol::table>( ).get<int>( "x" );
            int16_t y = kv_pair.second.as<sol::table>( ).get<int>( "y" );
            Points.emplace_back( Vec2<int16_t>( x, y ) );
        }

        gBuffer->Polyline( Points, color );
    }

    void Polygon( const sol::table& points, Color color ) {
        std::vector<Vec2<int16_t>> Points;

        for ( const auto& kv_pair : points ) {
            int16_t x = kv_pair.second.as<sol::table>( ).get<int>( "x" );
            int16_t y = kv_pair.second.as<sol::table>( ).get<int>( "y" );
            Points.emplace_back( Vec2<int16_t>( x, y ) );
        }

        gBuffer->Polygon( Points, color );
    }

    void Rectangle( Vec2<int16_t> position, Vec2<int16_t> size, Color color, int rounding, CornerFlags flags ) {
        gBuffer->Rectangle( position, size, color, rounding, flags );
    }

    void FilledRectangle( Vec2<int16_t> position, Vec2<int16_t> size, Color color, int rounding, CornerFlags flags ) {
        gBuffer->FilledRectangle( position, size, color, rounding, flags );
    }

    void Gradient( Vec2<int16_t> position, Vec2<int16_t> size, Color color_from, Color color_to, bool vertical ) {
        gBuffer->Gradient( position, size, color_from, color_to, vertical );
    }

    void FilledGradient( Vec2<int16_t> position, Vec2<int16_t> size, Color color_from, Color color_to, bool vertical ) {
        gBuffer->FilledGradient( position, size, color_from, color_to, vertical );
    }

    void Gradient4( Vec2<int16_t> position, Vec2<int16_t> size, Color color_top_left, Color color_top_right, Color color_bottom_right, Color color_bottom_left ) {
        gBuffer->Gradient( position, size, color_top_left, color_top_right, color_bottom_right, color_bottom_left );
    }

    void FilledGradient4( Vec2<int16_t> position, Vec2<int16_t> size, Color color_top_left, Color color_top_right, Color color_bottom_right, Color color_bottom_left ) {
        gBuffer->FilledGradient( position, size, color_top_left, color_top_right, color_bottom_right, color_bottom_left );
    }

    void Triangle( Vec2<int16_t> point1, Vec2<int16_t> point2, Vec2<int16_t> point3, Color color ) {
        gBuffer->Triangle( point1, point2, point3, color );
    }

    void FilledTriangle( Vec2<int16_t> point1, Vec2<int16_t> point2, Vec2<int16_t> point3, Color color ) {
        gBuffer->FilledTriangle( point1, point2, point3, color );
    }

    void Circle( Vec2<int16_t> position, int16_t radius, Color color ) {
        gBuffer->Circle( position, radius, color );
    }

    void FilledCircle( Vec2<int16_t> position, int16_t radius, Color color ) {
        gBuffer->FilledCircle( position, radius, color );
    }

    void Text( Font* font, const std::string& text, Vec2<int16_t> pos, Color color ) {
        gBuffer->Text( font, text, pos, color );
    }

    Vec2<int16_t> GetTextSize( Font* font, const std::string& text ) {
        return gBuffer->GetTextSize( font, text );
    }

    Vec2<int16_t> GetScreenSize( ) {
        return gWindow->GetSize( );
    }

    std::unique_ptr<Font> CreateFont_( std::string font_name, int16_t size, int16_t weight, Vec2<int16_t> padding, bool antialiasing ) {
        auto font = std::make_unique<Font>( );
        gGraphics->CreateFontFromName( font.get( ), font_name, size, weight, padding, antialiasing );
        return font;
    }

    Font* GetDefaultFont( ) {
        return gBuffer->GetDefaultFont( );
    }

    int GetVerticesCount( ) {
        return gBuffer->GetVerticesCount( );
    }

    int GetIndicesCount( ) {
        return gBuffer->GetIndicesCount( );
    }

    void PushClip( Rect<int16_t> Rect ) {
        gBuffer->PushClip( Rect );
    }

    void PopClip( ) {
        gBuffer->PopClip( );
    }
};

namespace Animations {
    float Lerp( float from, float to, float delta_time ) {
        return gAnimations->Lerp( from, to, delta_time );
    }

    float Coserp( float from, float to, float delta_time ) {
        return gAnimations->Coserp( from, to, delta_time );
    }

    float Smoothstep( float from, float to, float delta_time ) {
        return gAnimations->Smoothstep( from, to, delta_time );
    }
};

namespace Math {
    float RandomFloat( float min, float max ) {
        static std::random_device rdev;
        static std::default_random_engine re( rdev( ) );
        typedef typename std::conditional<
            std::is_floating_point<float>::value,
            std::uniform_real_distribution<float>,
            std::uniform_int_distribution<float>>::type dist_type;
        dist_type uni( min, max );
        return static_cast< float >( uni( re ) );
    }

    float RandomInt( int min, int max ) {
        static std::random_device rdev;
        static std::default_random_engine re( rdev( ) );
        typedef typename std::conditional<
            std::is_floating_point<int>::value,
            std::uniform_real_distribution<int>,
            std::uniform_int_distribution<int>>::type dist_type;
        dist_type uni( min, max );
        return static_cast< int >( uni( re ) );
    }
};

namespace Http {
    size_t WriteCallback( void* contents, size_t size, size_t nmemb, std::string* s ) {
        s->append( ( char* ) contents, size * nmemb );
        return size * nmemb;
    }

    std::string Get( const std::string& url ) {
        CURL* Curl;
        std::string Response;

        Curl = curl_easy_init( );
        if ( Curl ) {
            curl_easy_setopt( Curl, CURLOPT_URL, url.c_str( ) );
            curl_easy_setopt( Curl, CURLOPT_WRITEDATA, &Response );
            curl_easy_setopt( Curl, CURLOPT_WRITEFUNCTION, WriteCallback );

            CURLcode Result = curl_easy_perform( Curl );
            if ( Result != CURLE_OK ) {
                std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror( Result ) << std::endl;
            }

            curl_easy_cleanup( Curl );
        }

        return Response;
    }

    std::string Post( const std::string& url, const std::string& post_data ) {
        CURL* Curl;
        std::string Response;

        Curl = curl_easy_init( );
        if ( Curl ) {
            curl_easy_setopt( Curl, CURLOPT_POST, 1L );
            curl_easy_setopt( Curl, CURLOPT_URL, url.c_str( ) );
            curl_easy_setopt( Curl, CURLOPT_WRITEDATA, &Response );
            curl_easy_setopt( Curl, CURLOPT_POSTFIELDS, post_data.c_str( ) );
            curl_easy_setopt( Curl, CURLOPT_WRITEFUNCTION, WriteCallback );

            struct curl_slist* headers = NULL;
            headers = curl_slist_append( headers, "Content-Type: application/json" );
            curl_easy_setopt( Curl, CURLOPT_HTTPHEADER, headers );

            CURLcode Result = curl_easy_perform( Curl );
            if ( Result != CURLE_OK ) {
                std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror( Result ) << std::endl;
            }

            curl_slist_free_all( headers );
            curl_easy_cleanup( Curl );
        }

        return Response;
    }

    std::string Put( const std::string& url, const std::string& put_data ) {
        CURL* Curl;
        std::string Response;

        Curl = curl_easy_init( );
        if ( Curl ) {
            curl_easy_setopt( Curl, CURLOPT_CUSTOMREQUEST, "PUT" );
            curl_easy_setopt( Curl, CURLOPT_URL, url.c_str( ) );
            curl_easy_setopt( Curl, CURLOPT_WRITEDATA, &Response );
            curl_easy_setopt( Curl, CURLOPT_POSTFIELDS, put_data.c_str( ) );
            curl_easy_setopt( Curl, CURLOPT_WRITEFUNCTION, WriteCallback );

            struct curl_slist* headers = NULL;
            headers = curl_slist_append( headers, "Content-Type: application/json" );
            curl_easy_setopt( Curl, CURLOPT_HTTPHEADER, headers );

            CURLcode Result = curl_easy_perform( Curl );
            if ( Result != CURLE_OK ) {
                std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror( Result ) << std::endl;
            }

            curl_slist_free_all( headers );
            curl_easy_cleanup( Curl );
        }

        return Response;
    }

    std::string Delete( const std::string& url ) {
        CURL* Curl;
        std::string Response;

        Curl = curl_easy_init( );
        if ( Curl ) {
            curl_easy_setopt( Curl, CURLOPT_CUSTOMREQUEST, "DELETE" );
            curl_easy_setopt( Curl, CURLOPT_URL, url.c_str( ) );
            curl_easy_setopt( Curl, CURLOPT_WRITEDATA, &Response );
            curl_easy_setopt( Curl, CURLOPT_WRITEFUNCTION, WriteCallback );

            CURLcode Result = curl_easy_perform( Curl );
            if ( Result != CURLE_OK ) {
                std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror( Result ) << std::endl;
            }

            curl_easy_cleanup( Curl );
        }

        return Response;
    }
};

namespace Json {
    std::string Serialize( sol::object value ) {
        nlohmann::json j;

        if ( value.is<bool>( ) )
            j = value.as<bool>( );
        else if ( value.is<int>( ) )
            j = value.as<int>( );
        else if ( value.is<double>( ) )
            j = value.as<double>( );
        else if ( value.is<std::string>( ) )
            j = value.as<std::string>( );
        else if ( value.is<sol::table>( ) ) {
            sol::table table = value.as<sol::table>( );

            for ( auto& pair : table )
                j[ pair.first.as<std::string>( ) ] = Serialize( pair.second );
        }
        else
            j = nullptr;

        return j.dump( );
    }

    sol::object Deserialize( const std::string& json_str, sol::this_state L ) {
        nlohmann::json j = nlohmann::json::parse( json_str );

        if ( j.is_null( ) )
            return sol::nil;
        else if ( j.is_boolean( ) )
            return sol::make_object( L, j.get<bool>( ) );
        else if ( j.is_number_integer( ) )
            return sol::make_object( L, j.get<int>( ) );
        else if ( j.is_number_float( ) )
            return sol::make_object( L, j.get<double>( ) );
        else if ( j.is_string( ) )
            return sol::make_object( L, j.get<std::string>( ) );
        else if ( j.is_object( ) ) {
            sol::table result = sol::state_view( L ).create_table( );

            for ( auto it = j.begin( ); it != j.end( ); ++it )
                result[ it.key( ) ] = Deserialize( it.value( ).dump( ), L );

            return result;
        }
        else
            return sol::nil;
    }
};

namespace Utils {
    std::string SHA256( const std::string& str ) {
        cSHA256 Sha256;
        Sha256.update( str );
        std::array<uint8_t, 32> digest = Sha256.digest( );
        return Sha256.toString( digest );
    }

    std::string Base64Encode( const std::string& str ) {
        return base64::to_base64( str );
    }

    std::string Base64Decode( const std::string& str ) {
        return base64::from_base64( str );
    }
};

namespace Globals {
    static std::vector<std::string> CallbackIdentifiers {
        "OnInterfacePaint",
        "__OnInterfacePaint",
        "OnWorldPaint",
        "__OnWorldPaint",
        "OnInputUpdate",
        "__OnInputUpdate",
        "OnObjectCreation"
        "__OnInterfacePaint"
    };

    void AddCallback( sol::this_state s, std::string event_name, sol::protected_function function ) {
        if ( std::find( CallbackIdentifiers.begin( ), CallbackIdentifiers.end( ), event_name ) == CallbackIdentifiers.end( ) ) {
            std::cout << "Lua error: invalid callback \"" + event_name + '\"' << std::endl;
            return;
        }

        gLuaWrapper->RegisterCallback( event_name, function );
    }

    int LoadScript( const std::string& file_name ) {
        return gLuaWrapper->LoadScriptFromFile( FS_SCRIPTS_FOLDER, file_name );
    }

    int LoadString( const std::string& source ) {
        return gLuaWrapper->LoadScript( source );
    }
};