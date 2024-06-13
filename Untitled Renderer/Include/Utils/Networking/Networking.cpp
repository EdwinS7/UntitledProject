#include "Networking.hpp"

size_t WriteCallback( void* contents, size_t size, size_t nmemb, std::string* s ) {
    s->append( ( char* ) contents, size * nmemb );
    return size * nmemb;
}

std::string cNetworking::Get( const std::string& url ) {
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

std::string cNetworking::Post( const std::string& url, const std::string& post_data ) {
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

std::string cNetworking::Put( const std::string& url, const std::string& put_data ) {
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

std::string cNetworking::Delete( const std::string& url ) {
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