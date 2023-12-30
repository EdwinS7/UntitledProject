#pragma once

#include "../Gui.hpp"

class WindowMovementInfo {
public:
	Vec2<int16_t> Difference;
	bool StartedOutside, Active;
};

class WindowColorPalate {
public:
	Color Accent = { 255, 124, 124, 255 };
	Color Background = { 18, 18, 18, 255 };
	Color Outline = { 40, 40, 40, 255 };
	Color Shadow = { 0, 0, 0, 255 };
	Color Header = { 10, 10, 10, 255 };
	Color Footer = { 10, 10, 10, 255 };
	Color Title = { 240, 240, 240, 255 };
	Color Description = { 85, 85, 85, 255 };
	Color TabTextActive = Accent;
	Color TabTextInactive{ 140, 140, 140, 255 };
};

extern class Tab;

class Window {
public:
	Window( ) = default;

	Window( const char* Title, const char* Description, Vec2<int16_t> Pos, Vec2<int16_t> Size, Vec2<int16_t> MinSize, Vec2<int16_t> MaxSize )
		: m_Title( Title ), m_Description( Description ), m_Pos( Pos ), m_Size( Size ), m_MinSize( MinSize ), m_MaxSize( MaxSize ) { }

	void Render( );
	void HandleInput( );

	Tab* AddTab( const char* Title );

	inline WindowColorPalate GetColorPalate( );
	void SetColorPalate( WindowColorPalate Palate );

	Vec2<int16_t> GetElementFramePos( );
	Vec2<int16_t> GetElementFrameSize( );

	void Destroy( );

private:
	std::string m_Title, m_Description;

	Vec2<int16_t> m_MinSize, m_MaxSize;
	Vec2<int16_t> m_Pos, m_Size;

	WindowColorPalate m_ColorPalate;

	WindowMovementInfo m_Dragging, m_Resizing;

	int m_HeaderSize, m_FooterSize;

	int m_LeftPadding, m_RightPadding,
		m_TopPadding, m_BottomPadding;

	std::vector<Tab> m_Tabs;

	int m_CurrentTab{ 0 };
};

class Group {
public:
	void Begin( Vec2<int16_t> Size );
	void Destroy( );

	Group( ) = default;

	Group( const char* Title, Window* Window, Vec2<int16_t> Padding )
		: m_Title( Title ), m_Parent( Window ), m_Padding( Padding ) {}

private:
	Window* m_Parent;
	WindowColorPalate m_ColorPalate;

	Vec2<int16_t> m_Padding;
	std::string m_Title;

	int m_HeaderSize;
};