#pragma once

// includes
#include "../../inc.hpp"

#ifdef UNTITLED_USE_GL3

class c_gfx {
public:
	void create_context( );

	bool create_device( HWND hwnd );

	void release( );

	void begin_scene( );

	void end_scene( );

	void render_draw_data( );

private:

};

inline const auto g_gfx = std::make_unique<c_gfx>( );

#endif //  __UNTITLED_GL3__