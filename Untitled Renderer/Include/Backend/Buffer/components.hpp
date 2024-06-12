#pragma once
#include "../Graphics/Components.hpp"

enum CornerFlags {
	CornerNone = 0 << 0,
	CornerTopLeft = 1 << 0,
	CornerTopRight = 1 << 1,
	CornerBottomLeft = 1 << 2,
	CornerBottomRight = 1 << 3,

	CornerTop = CornerTopLeft | CornerTopRight,
	CornerRight = CornerTopRight | CornerBottomRight,
	CornerBottom = CornerBottomLeft | CornerBottomRight,
	CornerLeft = CornerTopLeft | CornerBottomLeft,

	CornerAll = CornerTopLeft | CornerTopRight | CornerBottomLeft | CornerBottomRight
};

class CommandResources {
public:
	std::vector<IDirect3DTexture9*> TextureStack;
	std::vector<RECT> ClipStack;
};

class DrawCommand {
public:
	int8_t Primitive{ 0 };
	CommandResources Resources;
	std::vector<Vertex> Vertices;
	std::vector<std::int32_t> Indices;
	int VerticesCount{ 0 }, IndicesCount{ 0 };

	constexpr DrawCommand( ) noexcept = default;
	constexpr DrawCommand( int8_t Primitive, std::vector<Vertex> Vertices, std::vector<std::int32_t> Indices, CommandResources Resources, int VerticesCount, int IndicesCount ) noexcept
		: Primitive( Primitive ), Vertices( Vertices ), Indices( Indices ), Resources( Resources ), VerticesCount( VerticesCount ), IndicesCount( IndicesCount ) {}
};

class CompiledDrawCommand {
public:
	std::vector<Vertex> Vertices;
	std::vector<std::int32_t> Indices;
	int VerticesCount{ 0 }, IndicesCount{ 0 };

	constexpr CompiledDrawCommand( ) noexcept = default;
	constexpr CompiledDrawCommand( std::vector<Vertex> Vertices, std::vector<std::int32_t> Indices, int VerticesCount, int IndicesCount ) noexcept
		: Vertices( Vertices ), Indices( Indices ), VerticesCount( VerticesCount ), IndicesCount( IndicesCount ) {}

	void reset( ) {
		if ( !Vertices.empty( ) )
			Vertices.clear( );

		if ( !Indices.empty( ) )
			Indices.clear( );

		VerticesCount = 0;
		IndicesCount = 0;
	}
};