#pragma once

#include "../../inc.hpp"

// @note: pasted from lnk1181 nemesis.
struct code_section_t {
	__forceinline constexpr code_section_t( ) = default;

	__forceinline constexpr code_section_t(
		const std::size_t size, const std::uintptr_t base
	) : m_size{ size }, m_base{ base } {}

	__forceinline code_section_t( const HMODULE module ) {
		const auto dos_header = reinterpret_cast< PIMAGE_DOS_HEADER >( module );
		if ( dos_header->e_magic != IMAGE_DOS_SIGNATURE )
			return;

		const auto nt_headers = reinterpret_cast< PIMAGE_NT_HEADERS >(
			reinterpret_cast< std::uintptr_t >( module ) + dos_header->e_lfanew
		);
		if ( nt_headers->Signature != IMAGE_NT_SIGNATURE )
			return;

		m_size = nt_headers->OptionalHeader.SizeOfCode;
		m_base = reinterpret_cast< std::uintptr_t >( module ) + nt_headers->OptionalHeader.BaseOfCode;
	}

	std::size_t		m_size{};
	std::uintptr_t	m_base{};
};

class c_mem {
public:
	std::uintptr_t find_pattern( const code_section_t& module, const std::string& sequence );

private:


};

inline const auto g_mem = std::make_unique<c_mem>( );

#include "mem.inl"