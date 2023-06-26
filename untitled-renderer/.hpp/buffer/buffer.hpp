#pragma once

// includes
#include "../../inc.hpp"
#include "components.hpp"

class c_buffer {
public:
    void create_objects( );

    void write_to_buffer(
        const uint8_t primitive,
        const std::vector<vertex_t>* vertices
    );

    void build_draw_commands(
        const std::vector<draw_command_t>& draw_commands
    );

    void line(
        const vector2_t<uint16_t> from,
        const vector2_t<uint16_t> to,
        const color_t clr
    );

    void polyline(
        const std::vector<vector2_t<uint16_t>> points,
        const color_t clr
    );

    void polygon(
        const std::vector<vector2_t<uint16_t>> points,
        const color_t clr
    );

    void rectangle(
        const vector2_t<uint16_t> pos,
        const vector2_t<uint16_t> size,
        const color_t clr
    );

    void filled_rectangle(
        const vector2_t<uint16_t> pos,
        const vector2_t<uint16_t> size,
        const color_t clr
    );

    void gradient(
        const vector2_t<uint16_t> pos,
        const vector2_t<uint16_t> size,
        const color_t clr,
        const color_t clr2,
        const bool vertical
    );

    void filled_gradient(
        const vector2_t<uint16_t> pos,
        const vector2_t<uint16_t> size,
        const color_t clr,
        const color_t clr2,
        const bool vertical
    );

    void circle(
        const vector2_t<uint16_t> pos,
        const uint16_t radius,
        const color_t clr
    );

    void filled_circle(
        const vector2_t<uint16_t> pos,
        const uint16_t size,
        const color_t clr
    );

    void filled_circle_multi_color(
        const vector2_t<uint16_t> pos,
        const uint16_t radius,
        const color_t outer_clr,
        const color_t inner_clr
    );

    command_t get_command( );

    void push_command( const command_t command );

    compiled_draw_command_t get_draw_command( );

    std::vector<draw_command_t> get_draw_commands( );

    void clear_commands( );

private:
	std::vector < draw_command_t > m_draw_commands;
	compiled_draw_command_t m_draw_command;
	command_t m_command;

	void generate_arc_points( 
		std::vector<vector2_t<uint16_t>>* points,
		const vector2_t<uint16_t>* pos, 
		const uint16_t radius,
		const uint16_t completion,
		const uint16_t rotation,
		const uint16_t segments,
		const bool filled
	);

	void make_vertices( 
		std::vector<vertex_t>* vertices, 
		const std::vector<vector2_t<uint16_t>>* points, 
		const color_t* color
	);
};

inline const auto g_buffer = std::make_unique<c_buffer>( );