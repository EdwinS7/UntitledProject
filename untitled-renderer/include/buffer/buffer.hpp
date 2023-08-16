#pragma once

// includes
#include "../../inc.hpp"
#include "components.hpp"
#include "fonts/font.hpp"

class c_buffer {
public:
    void create_objects( );
    void destroy_objects( );

    void write_to_buffer( const int8_t primitive, const std::vector<vertex_t>* vertices, const std::vector<int32_t>* indices );
    void build_draw_commands( const std::vector<draw_command_t>& draw_commands );

    void line( const vector2_t<int16_t> from, const vector2_t<int16_t> to, const color_t clr );

    void polyline( const std::vector<vector2_t<int16_t>> points, const color_t clr );

    void polygon(
        const std::vector<vector2_t<int16_t>> points,
        const color_t clr
    );

    void rectangle(
        const vector2_t<int16_t> pos,
        const vector2_t<int16_t> size,
        const color_t clr,
        const int16_t rounding,
        const corner_flags flags = corner_all
    );

    void filled_rectangle(
        const vector2_t<int16_t> pos,
        const vector2_t<int16_t> size,
        const color_t clr,
        const int16_t rounding,
        const corner_flags flags = corner_all
    );

    void textured_rectangle(
        texture* resource,
        const vector2_t<int16_t> pos,
        const vector2_t<int16_t> size,
        const color_t clr
    );

    void gradient(
        const vector2_t<int16_t> pos,
        const vector2_t<int16_t> size,
        const color_t clr,
        const color_t clr2,
        const bool vertical
    );

    void filled_gradient(
        const vector2_t<int16_t> pos,
        const vector2_t<int16_t> size,
        const color_t clr,
        const color_t clr2,
        const bool vertical
    );

    void triangle( 
        const vector2_t<int16_t> p1,
        const vector2_t<int16_t> p2,
        const vector2_t<int16_t> p3,
        const color_t clr
    );

    void filled_triangle(
       const vector2_t<int16_t> p1,
       const vector2_t<int16_t> p2,
       const vector2_t<int16_t> p3,
       const color_t clr
    );

    void circle(
        const vector2_t<int16_t> pos,
        const int16_t radius,
        const color_t clr
    );

    void filled_circle(
        const vector2_t<int16_t> pos,
        const int16_t size,
        const color_t clr
    );

    void text(
       const font_t* font,
       const char* str,
       const vector2_t<int16_t> pos,
       const color_t clr
    );

    vector2_t<int16_t> get_text_size(
         const font_t* font,
        const char* str
    );

// inline
    ALWAYS_INLINE void clear_commands( );

    ALWAYS_INLINE command_t get_command( );

    ALWAYS_INLINE compiled_draw_command_t get_draw_command( );

    ALWAYS_INLINE std::vector<draw_command_t> get_draw_commands( );

    ALWAYS_INLINE int get_num_of_commands( );

    ALWAYS_INLINE int get_num_of_vertices( );

    ALWAYS_INLINE int get_num_of_indices( );

    ALWAYS_INLINE void push_clip(
        RECT rect
    );

    ALWAYS_INLINE void pop_clip( );

    ALWAYS_INLINE void push_texture(
        texture resource
    );

    ALWAYS_INLINE void pop_texture( );

// @fonts
    font_t default_font;
    font_t interface_font;

// @textures
    texture demo_texture;

    void generate_quadratic_bezier_points(
       std::vector<vector2_t<int16_t>>* points,
       const vector2_t<int16_t> p0,
       const vector2_t<int16_t> p1,
       const vector2_t<int16_t> p2
    );

private:
    int m_vertices_count;
    int m_indices_count;

	std::vector < draw_command_t > m_draw_commands;
	compiled_draw_command_t m_draw_command;
	command_t m_command;

#ifdef UNTITLED_USE_PRE_COMPUTED_SIN_COS
    std::vector<double> m_pre_computed_sin, m_pre_computed_cos;
#endif

	void generate_arc_points( 
		std::vector<vector2_t<int16_t>>* points,
		const vector2_t<int16_t>* pos, 
		const int16_t radius,
		const int16_t completion,
		const int16_t rotation,
		const int16_t segments
	);

	void make_vertices( 
		std::vector<vertex_t>* vertices, 
		const std::vector<vector2_t<int16_t>>* points, 
		const color_t* color
	);
};

inline const auto g_buffer = std::make_unique<c_buffer>( );

/* lower = performance, higher = quality */
#define CIRCLE_SEGMENTS 64
#define RECTANGLE_SEGMENTS 16
#define BEZIER_CUBIC_SEGMENTS 128
#define BEZIER_QUADRATIC_SEGMENTS 128

#include "buffer.inl"