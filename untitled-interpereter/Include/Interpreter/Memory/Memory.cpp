#include "Memory.hpp"

template<typename var>
std::map<std::string, var> Variables;

int Untitled::Memory::GetType( std::string ref ) {
    if ( Utilities::IsNumber( ref ) )
        return VARIABLE_TYPE_INT;
    else if ( Utilities::ToLower( ref ) == "true" || Utilities::ToLower( ref ) == "false" )
        return VARIABLE_TYPE_BOOL;
    else                                // Add detection for strings.
        return VARIABLE_TYPE_STRING;

    return VARIABLE_TYPE_UNDEFINED;
}

bool Untitled::Memory::IsVariable( std::string ref ) {
    if ( Variables<int>.find( ref ) != Variables<int>.end( ) || Variables<bool>.find( ref ) != Variables<bool>.end( ) || Variables<std::string>.find( ref ) != Variables<std::string>.end( ) )
        return true;

    return false;
}

std::map<std::string, int> Untitled::Memory::GetIntegers( ) {
    return Variables<int>;
}

std::map<std::string, bool> Untitled::Memory::GetBooleans( ) {
    return Variables<bool>;
}

std::map<std::string, std::string> Untitled::Memory::GetStrings( ) {
    return Variables<std::string>;
}

int Untitled::Memory::GetInteger( std::string ref ) {
    return Variables<int>[ ref ];
}

bool Untitled::Memory::GetBoolean( std::string ref ) {
    return Variables<bool>[ ref ];
}

std::string Untitled::Memory::GetString( std::string ref ) {
    return Variables<std::string>[ ref ];
}

void Untitled::Memory::Set( std::string ref, int value ) {
    Variables<int>[ ref ] = value;
}

void Untitled::Memory::Set( std::string ref, bool value ) {
    Variables<bool>[ ref ] = value;
}

void Untitled::Memory::Set( std::string ref, std::string value ) {
    Variables<std::string>[ ref ] = value;
}