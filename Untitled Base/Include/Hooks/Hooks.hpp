#pragma once

#include "../../Common.hpp"

namespace Hooks {
	int Initialize( );
	void Shutdown( );

	// Hooks
	bool __fastcall IsLicenseKeyValid( const char* key );
	inline decltype( &IsLicenseKeyValid ) IsLicenseKeyValidFn{};
}