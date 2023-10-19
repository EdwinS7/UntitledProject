#pragma once

#include "../../common.hpp"

class CContext {
public:
	void RenderWindow( );

private:
	const char* m_Software{ "MX Simulator" };

	int m_ActiveRegion, m_CurrentSelection;
	std::string m_Username, m_Password;
};

inline const auto Context = std::make_unique<CContext>( );