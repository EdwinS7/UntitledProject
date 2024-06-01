#include <random>
std::vector<std::string> CallbackIdentifiers{
    "OnInterfacePaint",
    "OnWorldPaint",
     "OnInputUpdate",
    "OnObjectCreation"
};

namespace Client {
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

namespace Graphics {
    void SetClearColor( Color clear_color ) {
        return gGraphics->SetClearColor( clear_color );
    }

    Color GetClearColor( ) {
        return gGraphics->GetClearColor( );
    }
};

namespace Renderer {
    void Line( const Vec2<int16_t> from, const Vec2<int16_t> to, const Color color ) {
        gBuffer->Line( from, to, color );
    }

    void Polyline( const std::vector<Vec2<int16_t>>& points, const Color color ) {
        gBuffer->Polyline( points, color );
    }

    void Polygon( const std::vector<Vec2<int16_t>>& points, const Color color ) {
        gBuffer->Polygon( points, color );
    }

    void Rectangle( const Vec2<int16_t> position, const Vec2<int16_t> size, const Color color, const int rounding ) {
        gBuffer->Rectangle( position, size, color, rounding, CornerAll );
    }

    void FilledRectangle( const Vec2<int16_t> position, const Vec2<int16_t> size, const Color color, const int rounding ) {
        gBuffer->FilledRectangle( position, size, color, rounding, CornerAll );
    }

    void Gradient( const Vec2<int16_t> position, const Vec2<int16_t> size, const Color color_from, const Color color_to, const bool vertical ) {
        gBuffer->Gradient( position, size, color_from, color_to, vertical );
    }

    void FilledGradient( const Vec2<int16_t> position, const Vec2<int16_t> size, const Color color_from, const Color color_to, const bool vertical ) {
        gBuffer->FilledGradient( position, size, color_from, color_to, vertical );
    }

    void Gradient4( const Vec2<int16_t> position, const Vec2<int16_t> size, const Color color_top_left, const Color color_top_right, const Color color_bottom_right, const Color color_bottom_left ) {
        gBuffer->Gradient( position, size, color_top_left, color_top_right, color_bottom_right, color_bottom_left );
    }

    void FilledGradient4( const Vec2<int16_t> position, const Vec2<int16_t> size, const Color color_top_left, const Color color_top_right, const Color color_bottom_right, const Color color_bottom_left ) {
        gBuffer->FilledGradient( position, size, color_top_left, color_top_right, color_bottom_right, color_bottom_left );
    }

    void Triangle( const Vec2<int16_t> point1, const Vec2<int16_t> point2, const Vec2<int16_t> point3, const Color color ) {
        gBuffer->Triangle( point1, point2, point3, color );
    }

    void FilledTriangle( const Vec2<int16_t> point1, const Vec2<int16_t> point2, const Vec2<int16_t> point3, const Color color ) {
        gBuffer->FilledTriangle( point1, point2, point3, color );
    }

    void Circle( const Vec2<int16_t> position, const int16_t radius, const Color color ) {
        gBuffer->Circle( position, radius, color );
    }

    void FilledCircle( const Vec2<int16_t> position, const int16_t radius, const Color color ) {
        gBuffer->FilledCircle( position, radius, color );
    }

    void Text( Font* font, std::string text, Vec2<int16_t> pos, Color color ) {
        gBuffer->Text( font, text, pos, color );
    }

    Vec2<int16_t> GetTextSize( const Font* font, const std::string& text ) {
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
    std::string Get( const std::string& url ) {
        CURL* Curl;
        std::string Response;

        Curl = curl_easy_init( );
        if ( Curl ) {
            curl_easy_setopt( Curl, CURLOPT_URL, url );
            curl_easy_setopt( Curl, CURLOPT_WRITEDATA, &Response );
            curl_easy_setopt( Curl, CURLOPT_WRITEFUNCTION, +[ ] ( void* contents, size_t size, size_t nmemb, std::string* s ) -> size_t {
                s->append( ( char* ) contents, size * nmemb );
                return size * nmemb;
            } );
            
            auto Result = curl_easy_perform( Curl );
            if ( Result != CURLE_OK )
                std::cout << curl_easy_strerror( Result ) << "\n";

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
            curl_easy_setopt( Curl, CURLOPT_URL, url );
            curl_easy_setopt( Curl, CURLOPT_WRITEDATA, &Response );
            curl_easy_setopt( Curl, CURLOPT_POSTFIELDS, post_data );
            curl_easy_setopt( Curl, CURLOPT_WRITEFUNCTION, +[ ] ( void* contents, size_t size, size_t nmemb, std::string* s ) -> size_t {
                s->append( ( char* ) contents, size * nmemb );
                return size * nmemb;
            } );

            CURLcode Result = curl_easy_perform( Curl );
            if ( Result != CURLE_OK )
                std::cout << curl_easy_strerror( Result ) << "\n";

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
            curl_easy_setopt( Curl, CURLOPT_WRITEFUNCTION, +[ ] ( void* contents, size_t size, size_t nmemb, std::string* s ) -> size_t {
                s->append( ( char* ) contents, size * nmemb );
                return size * nmemb;
            } );

            CURLcode Result = curl_easy_perform( Curl );
            if ( Result != CURLE_OK )
                std::cerr << curl_easy_strerror( Result ) << "\n";

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
            curl_easy_setopt( Curl, CURLOPT_WRITEFUNCTION, +[ ] ( void* contents, size_t size, size_t nmemb, std::string* s ) -> size_t {
                s->append( ( char* ) contents, size * nmemb );
                return size * nmemb;
            } );

            CURLcode Result = curl_easy_perform( Curl );
            if ( Result != CURLE_OK )
                std::cerr << curl_easy_strerror( Result ) << "\n";

            curl_easy_cleanup( Curl );
        }

        return Response;
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

void AddCallback( sol::this_state s, std::string event_name, sol::protected_function function ) {
    if ( std::find( CallbackIdentifiers.begin( ), CallbackIdentifiers.end( ), event_name ) == CallbackIdentifiers.end( ) ) {
        std::cout << "Lua error: invalid callback \"" + event_name + '\"' << std::endl;
        return;
    }

    gWrapper->RegisterCallback( event_name, function );
}