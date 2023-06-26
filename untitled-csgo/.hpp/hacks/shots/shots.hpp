#pragma once

namespace hacks {
	struct shot_t {
		__forceinline constexpr shot_t( ) = default;

		__forceinline shot_t(
			const vec3_t& src, player_entry_t* const entry, 
			const std::shared_ptr< lag_record_t >& lag_record,
			const int dmg, const int next_shift_amount
		) : m_src{ src }, m_entry{ entry }, m_lag_record{ lag_record },
			m_dmg{ dmg }, m_target_index{ entry && entry->m_player ? entry->m_player->index( ) : -1 } {}

		vec3_t							m_src{};

		player_entry_t*					m_entry{};
		std::shared_ptr< lag_record_t > m_lag_record{};

		bool							m_processed{};
		int								m_dmg{},
										m_target_index{},
										m_next_shift_amount{},
										m_cmd_number{ -1 }, m_process_tick{};
		struct {
			vec3_t	m_impact_pos{};
			int		m_fire_tick{}, m_hurt_tick{}, m_hitgroup{};
		}								m_server_info{};
	};

	class c_shots {
	private:
		std::deque< shot_t > m_elements{};

		__forceinline std::string translate_hitgroup( const int index );
	public:
		void on_net_update( );

		void on_new_event( valve::game_event_t* const event );

		__forceinline std::deque< shot_t >& elements( );

		__forceinline shot_t* last_unprocessed( );

		__forceinline void add(
			const vec3_t& src, player_entry_t* const entry,
			const std::shared_ptr< lag_record_t >& lag_record, 
			const int dmg, const int next_shift_amount
		);
	};

	inline const auto g_shots = std::make_unique< c_shots >( );
}

#include "../../../.hpp/hacks/shots/inl/shots.inl"