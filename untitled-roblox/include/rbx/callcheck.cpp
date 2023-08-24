#include "rbx.hpp"

int c_rbx::convert_closure( uintptr_t closure_address = 0 ) {
    closure_addresses.push_back( closure_address );
    int closure_index = closure_addresses.size( ) - 1;
    int assigned_breakpoint = breakpoints[ closure_index ];
    return assigned_breakpoint;
}