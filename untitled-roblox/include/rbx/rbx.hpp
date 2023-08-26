#pragma once

#include "../../inc.hpp"

// @note: roblox encoder. pulled from kawaii compiler.
class bytecode_encoder_t : public Luau::BytecodeEncoder {
	std::uint8_t encodeOp(const std::uint8_t opcode) {
		return opcode * 227;
	}
};

class c_rbx {
public:
	// scheduler
	int get_job( const std::string& job );

	uintptr_t get_script_context( );
	uintptr_t get_global_state( );

	// execution
	std::string compress_to_bytecode( const std::string& bytecode);
	void run_script( uintptr_t rl, const std::string& source );

	// callcheck
	std::vector<int> breakpoints;
	std::vector<uintptr_t> closure_addresses;

	int convert_closure(uintptr_t closure_address);

	// functions
	void set_identity( const uintptr_t& state, const uint8_t& identity );
	void push_global(uintptr_t rl, int fn, const char* name);
	void pop_stack(uintptr_t state, uint8_t amount);
	void register_functions(uintptr_t rl );
	void teleport_handler();
};

inline const auto g_rbx = std::make_unique<c_rbx>( );