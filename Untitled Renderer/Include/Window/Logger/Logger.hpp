#pragma once
#include "../../../common.hpp"

class cConsole {
public:
	int InitConsole( const char* title );
	void ExitConsole( );

	bool Print( int level, const char* parent, const char* message );

	inline Vec2<int16_t> GetPos( );
	inline Vec2<int16_t> GetSize( );
	inline Rect<int16_t> GetClipRect( );
	inline Rect<int16_t> GetRect( );
	inline HWND GetHwnd( );
private:
	bool m_Open;

};

inline const auto gConsole = std::make_unique<cConsole>( );

#include "Logger.inl"