#pragma once
#ifndef NETWORKING_H
#define NETWORKING_H

#include <Common.hpp>

class cNetworking {
public:
    std::string Get( const std::string& url );
    std::string Post( const std::string& url, const std::string& post_data );
    std::string Put( const std::string& url, const std::string& put_data );
    std::string Delete( const std::string& url );
};

inline const auto gNetworking = std::make_unique<cNetworking>( );

#include "Networking.inl"
#endif