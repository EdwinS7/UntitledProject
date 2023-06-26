#pragma once

#include "../shots.hpp"

namespace hacks {
	__forceinline std::string c_shots::translate_hitgroup( const int index ) {
		switch ( index ) {
		case 0: return xorstr_( "generic" ); break;
		case 1: return xorstr_( "head" ); break;
		case 2: return xorstr_( "chest" ); break;
		case 3: return xorstr_( "stomach" ); break;
		case 4: return xorstr_( "left arm" ); break;
		case 5: return xorstr_( "right arm" ); break;
		case 6: return xorstr_( "left leg" ); break;
		case 7: return xorstr_( "right leg" ); break;
		case 8: return xorstr_( "neck" ); break;
		default: break;
		}

		return xorstr_( "generic" );
	}

	__forceinline std::deque< shot_t >& c_shots::elements( ) { return m_elements; }

	__forceinline shot_t* c_shots::last_unprocessed( ) {
		if ( m_elements.empty( ) )
			return nullptr;

		const auto shot = std::find_if(
			m_elements.begin( ), m_elements.end( ),
			[ ] ( const shot_t& shot ) {
				return !shot.m_processed
					&& shot.m_server_info.m_fire_tick
					&& shot.m_server_info.m_fire_tick == valve::g_client_state->m_server_tick;
			}
		);

		return shot == m_elements.end( ) ? nullptr : &*shot;
	}

	__forceinline void c_shots::add(
		const vec3_t& src, player_entry_t* entry,
		const std::shared_ptr< lag_record_t >& lag_record,
		const int dmg, const int next_shift_amount
	) {
		m_elements.emplace_back( src, entry, lag_record, dmg, next_shift_amount );

		if ( m_elements.size( ) < 128 )
			return;

		m_elements.pop_front( );
	}
}