#pragma once
#if 0
namespace hacks {
	struct precip_entity_t {
		valve::c_entity*	m_entity;
		void*				m_networkable;
		vec3_t				m_mins, m_maxs;
		valve::vcollide_t	m_collide;
		bool				m_collide_init = false;
		int					m_model_index = 0,
							m_abs_model_index = 0,
							m_type;
	};

	class c_weather {
	private:
		valve::vcollide_t				m_collide;
		std::vector< precip_entity_t >	m_precip_list;
		std::list< int >				m_model_indexes;
		bool							m_created = false;

		void create_precip_entity( precip_entity_t* ent_info, int precip_mode ) {
			static valve::client_class_t* precipitation_client_class = nullptr;

			if ( !precipitation_client_class )
				for ( auto pclass = valve::g_client->all_classes( ); pclass && !precipitation_client_class; pclass = pclass->m_next )
					if ( pclass->m_class_id == valve::e_class_id::precipitation )
						precipitation_client_class = pclass;

			ent_info->m_networkable = ( ( IClientNetworkable * ( * )( int, int ) )precipitation_client_class->m_create_fn )( 2048 - 1, 0 );
			if ( !ent_info->m_networkable )
				return;

			if ( !ent_info->m_collide_init ) {
				valve::g_phys_collision->VCollideLoad( &ent_info->m_collide, 1, ( const char* ) collide_data, 546, false );
				ent_info->m_collide_init = true;
			}

			ent_info->m_entity = static_cast< valve::c_entity* >(
						valve::g_entity_list->find_entity( 2048 - 1 )
					);

			// vfunc< void ( __thiscall * )(void *, int, int) > ( ent_info->m_entity, 97 ) (ent_info->m_entity, 0, 0);
			ent_info->m_entity->index( ) = -1;

			ent_info->m_entity->m_nPrecipType( ) = precip_mode;

			ent_info->m_entity->m_Collision( ).m_vecMins( ) = vec3_t( -2048.f, -2048.f, -2048.f );
			ent_info->m_entity->m_Collision( ).m_vecMaxs( ) = vec3_t( 2048.f, 2048.f, 2048.f );

			C_Collision* coll = &ent_info->m_entity->m_Collision( );
			*vfunc< c_vector3* ( __thiscall* )( void* ) >( coll, 1 ) ( coll ) = c_vector3( -64.f, -64.f, -64.f );
			*vfunc< c_vector3* ( __thiscall* )( void* ) >( coll, 2 ) ( coll ) = c_vector3( 64.f, 64.f, 64.f );

			ent_info->m_entity->m_nModelIndex( ) = ent_info->m_model_index;
			ent_info->m_entity->dormant( ) = false;

			ent_info->m_entity->networkable( )->OnDataChanged( valve::DATA_UPDATE_CREATED );
		}

		void unload_entity( precip_entity_t& precip ) {
			static auto destructor = pattern_t::new_from_array( IDA2BYTES( "55 8B EC 51 56 8B F1 8D 45 FC 57" ) ).search( FIND_MODULE( "client.dll" ) ).get< void( __thiscall* ) ( void* ) >( );

			precip.m_entity->m_nModelIndex( ) = 0;
			destructor( precip.m_entity );

			if ( precip.m_collide_init )
				valve::g_phys_collision->VCollideUnload( &precip.m_collide );
		}

		void remove_unused_entities( int type ) {
			if ( m_precip_list.empty( ) )
				return;

			bool clear = false;
			for ( auto& it : m_precip_list ) {
				if ( it.m_type != type ) {
					unload_entity( it );
					clear = true;
				}
			}
			if ( clear ) {
				m_precip_list.clear( );
				m_model_indexes.clear( );
			}
		}

		bool is_type_active( int type ) {
			for ( auto& it : m_precip_list ) {
				if ( it.m_type == type )
					return true;
			}
			return false;
		}

		void apply_weather_effect( int type ) {
			if ( !valve::g_engine->in_game( ) )
				return;

			if ( !valve::g_local_player
				|| !valve::g_local_player->alive( ) )
				return;

			int mdl_index = 0x7FF;
			if ( !m_model_indexes.empty( ) ) {
				int v11 = 0x7FF;
				while ( 1 )
				{
					auto begin = m_model_indexes.begin( );
					if ( m_model_indexes.empty( ) )
						break;
					while ( *begin != mdl_index )
					{
						if ( ++begin == m_model_indexes.end( ) )
							goto LABEL_7;
					}
					if ( begin == m_model_indexes.end( ) )
						break;
					--mdl_index;
				}
			LABEL_7:
				v11 = mdl_index;
			}

			//for ( int i = 0; i < 3; i++ ) {
			precip_entity_t& precip_ent = m_precip_list.emplace_back( );
			precip_ent.m_mins = vec3_t( -2048.f, -2048.f, -2048.f );
			precip_ent.m_maxs = vec3_t( 2048.f, 2048.f, 2048.f );
			precip_ent.m_collide_init = false;
			precip_ent.m_model_index = mdl_index - 0x800;
			precip_ent.m_abs_model_index = mdl_index;
			memset( &precip_ent.m_collide, 0, sizeof( precip_ent.m_collide ) );

			precip_ent.m_type = type;

			create_precip_entity( &precip_ent, type );
		}
	public:
		void clean_up( ) {
			if ( m_precip_list.empty( ) )
				return;

			for ( auto& it : m_precip_list ) {
				unload_entity( it );
			}
			m_precip_list.clear( );
			m_model_indexes.clear( );
		}

		void frame( valve::e_frame_stage stage, bool called_original ) {
			if ( !called_original && stage == valve::e_frame_stage::start ) {
				/*switch ( g_config->m_misc.m_weather_mode ) {
				case 1:
					remove_unused_entities( PRECIPITATION_TYPE_RAIN );
					if ( is_type_active( PRECIPITATION_TYPE_RAIN ) )
						return;

					apply_weather_effect( PRECIPITATION_TYPE_RAIN );
					break;
				case 2:
					remove_unused_entities( PRECIPITATION_TYPE_PARTICLERAIN );
					if ( is_type_active( PRECIPITATION_TYPE_PARTICLERAIN ) )
						return;

					apply_weather_effect( PRECIPITATION_TYPE_PARTICLERAIN );
					break;
				case 3:
					remove_unused_entities( PRECIPITATION_TYPE_PARTICLESNOW );
					if ( is_type_active( PRECIPITATION_TYPE_PARTICLESNOW ) )
						return;

					apply_weather_effect( PRECIPITATION_TYPE_PARTICLESNOW );
					break;
				default:
					clean_up( );
					break;
				}*/
			}

			if ( stage != valve::e_frame_stage::render_start || called_original )
				return;

			for ( auto it : m_precip_list ) {
				const auto new_mins = g_ctx->shoot_pos( ) + it.m_mins;
				const auto new_maxs = g_ctx->shoot_pos( ) + it.m_maxs;

				it.m_entity->m_Collision( ).m_vecMins( ) = new_mins;
				it.m_entity->m_Collision( ).m_vecMaxs( ) = new_maxs;

				C_Collision* coll = &it.m_entity->m_Collision( );
				*vfunc< vec3_t* ( __thiscall* )( void* ) >( coll, 1 ) ( coll ) = new_mins;
				*vfunc< vec3_t* ( __thiscall* )( void* ) >( coll, 2 ) ( coll ) = new_maxs;
			}
		}
	};

	inline const auto g_weather = std::make_unique< c_weather >( );
}
#endif

#include "../../../.hpp/hacks/weather/inl/weather.inl"