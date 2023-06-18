#include "../../.hpp/gfx/gl3.hpp"

#ifdef UNTITLED_USE_GL3

void c_gfx::create_context( ) {
	if ( !glfwInit( ) )
		throw std::runtime_error( "create_context failed (glfwInit)" );
}

bool c_gfx::create_device( hwnd_t hwnd ) {
	//glfwMakeContextCurrent( hwnd );
}

void c_gfx::begin_scene( ) {
	
}

void c_gfx::end_scene( ) {
	
}

void c_gfx::render_draw_data( ) {

}

void c_gfx::release( ) {
	
}

#endif //  __UNTITLED_GL3__