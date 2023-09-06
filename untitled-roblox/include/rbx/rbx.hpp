#pragma once

#include "../../inc.hpp"

class c_rbx {
public:
	// scheduler
	int get_job( const std::string& job );

	uintptr_t get_script_context( );
	uintptr_t get_global_state( );

	// execution
	std::string serialize( const std::string& bytecode);
	void run_script( const std::string& source );

	// callcheck
	std::vector<int> breakpoints;
	std::vector<uintptr_t> closure_addresses;

	int convert_closure(uintptr_t closure_address);

	// functions
	void set_identity( const uint8_t& identity );
	void pop_stack( uint8_t amount);
	int get_identity( );

	void register_functions( uintptr_t rl );
};

inline const auto g_rbx = std::make_unique<c_rbx>( );