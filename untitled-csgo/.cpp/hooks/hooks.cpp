#include "../../inc.hpp"

auto g_imgui_opened = true;
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam );

namespace hooks {
	LRESULT __stdcall wnd_proc( const HWND hwnd, const UINT msg, const WPARAM wparam, const LPARAM lparam ) {
		if ( msg == WM_KEYUP
			&& wparam == VK_INSERT )
			g_imgui_opened ^= 1;

		if ( g_imgui_opened ) {
			ImGui_ImplWin32_WndProcHandler( hwnd, msg, wparam, lparam );

			return 1;
		}

		return CallWindowProc( orig_wnd_proc, hwnd, msg, wparam, lparam );
	}

	long D3DAPI	d3d9_reset( IDirect3DDevice9* const device, D3DPRESENT_PARAMETERS* const present_params ) {
		ImGui_ImplDX9_InvalidateDeviceObjects( );

		const auto ret = orig_d3d9_reset( device, present_params );

		ImGui_ImplDX9_CreateDeviceObjects( );

		return ret;
	}

	long D3DAPI d3d9_present(
		IDirect3DDevice9* const device, RECT* const src_rect,
		RECT* const dst_rect, const HWND dst_wnd_override, RGNDATA* const dirty_region
	) {
		ImGui_ImplDX9_NewFrame( );
		ImGui_ImplWin32_NewFrame( );

		ImGui::NewFrame( );

		if ( g_imgui_opened ) {
			ImGui::SetNextWindowPos(
				ImGui::GetIO( ).DisplaySize / 2.f
				- ImGui::GetStyle( ).WindowMinSize / 2.f,
				ImGuiCond_Once
			);

			ImGui::Begin( xorstr_( "untitled" ), &g_imgui_opened );

			if ( ImGui::CollapsingHeader( xorstr_( "aim bot##main" ) ) ) {
				auto& cfg = hacks::g_aim_bot->cfg( );

				ImGui::Checkbox( xorstr_( "enabled##aim_bot" ), &cfg.m_enabled );
				ImGui::Checkbox( xorstr_( "auto scope##aim_bot" ), &cfg.m_auto_scope );

				ImGui::SliderInt( xorstr_( "hit chance##aim_bot" ), &cfg.m_hit_chance, 0, 100 );

				ImGui::Checkbox( xorstr_( "scale dmg on hp##aim_bot" ), &cfg.m_scale_dmg_on_hp );

				ImGui::SliderInt( xorstr_( "min dmg##aim_bot" ), &cfg.m_min_dmg, 0, 100 );

				ImGui::Hotkey( xorstr_( "min dmg override key##aim_bot" ), &cfg.m_min_dmg_override_key );

				ImGui::SliderInt( xorstr_( "min dmg override##aim_bot" ), &cfg.m_min_dmg_override, 0, 100 );

				if ( ImGui::BeginCombo( xorstr_( "hitgroups##aim_bot" ), "" ) ) {
					const char* hitgroups_list[]{
						( "head##aim_bot_hitgroups" ),
						( "body##aim_bot_hitgroups" ),
						( "stomach##aim_bot_hitgroups" ),
						( "arms##aim_bot_hitgroups" ),
						( "legs##aim_bot_hitgroups" ),
						( "feet##aim_bot_hitgroups" )
					};

					static bool hitgroups_vars[ IM_ARRAYSIZE( hitgroups_list ) ]{};

					for ( std::size_t i{}; i < IM_ARRAYSIZE( hitgroups_list ); ++i ) {
						hitgroups_vars[ i ] = cfg.m_hitgroups & ( 1 << i );

						ImGui::Selectable(
							hitgroups_list[ i ], &hitgroups_vars[ i ],
							ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups
						);

						if ( hitgroups_vars[ i ] )
							cfg.m_hitgroups |= ( 1 << i );
						else
							cfg.m_hitgroups &= ~( 1 << i );
					}

					ImGui::EndCombo( );
				}

				if ( ImGui::BeginCombo( xorstr_( "multi points##aim_bot" ), "" ) ) {
					const char* multi_points_list[]{
						( "head##aim_bot_multipoints" ),
						( "body##aim_bot_multipoints" ),
						( "stomach##aim_bot_multipoints" ),
						( "arms##aim_bot_multipoints" ),
						( "legs##aim_bot_multipoints" ),
						( "feet##aim_bot_multipoints" )
					};

					static bool multi_points_vars[ IM_ARRAYSIZE( multi_points_list ) ]{};

					for ( std::size_t i{}; i < IM_ARRAYSIZE( multi_points_list ); ++i ) {
						multi_points_vars[ i ] = cfg.m_multi_points & ( 1 << i );

						ImGui::Selectable(
							multi_points_list[ i ], &multi_points_vars[ i ],
							ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups
						);

						if ( multi_points_vars[ i ] )
							cfg.m_multi_points |= ( 1 << i );
						else
							cfg.m_multi_points &= ~( 1 << i );
					}

					ImGui::EndCombo( );
				}

				ImGui::Checkbox( xorstr_( "static point scale##aim_bot" ), &cfg.m_static_point_scale );

				if ( cfg.m_static_point_scale ) {
					ImGui::SliderInt( xorstr_( "head scale##aim_bot" ), &cfg.m_head_scale, 0, 100 );
					ImGui::SliderInt( xorstr_( "body scale##aim_bot" ), &cfg.m_body_scale, 0, 100 );
				}

				ImGui::Hotkey( xorstr_( "force body aim key##aim_bot" ), &cfg.m_force_baim_key );
				ImGui::Hotkey( xorstr_( "force safe point key##aim_bot" ), &cfg.m_force_safe_point_key );

				const char* auto_stop_type_list[]{
					( "none" ),
					( "speed" ),
					( "accuracy" )
				};

				ImGui::Combo( xorstr_( "auto stop type##aim_bot" ), &cfg.m_auto_stop_type, auto_stop_type_list, IM_ARRAYSIZE( auto_stop_type_list ) );

				if ( cfg.m_auto_stop_type )
					ImGui::Checkbox( xorstr_( "predictive auto stop##aim_bot" ), &cfg.m_predictive_auto_stop );
			}

			if ( ImGui::CollapsingHeader( xorstr_( "exploits##main" ) ) ) {
				auto& cfg = hacks::g_exploits->cfg( );

				ImGui::Checkbox( xorstr_( "enabled##exploits" ), &cfg.m_enabled );

				ImGui::Hotkey( xorstr_( "dt key##exploits" ), &cfg.m_dt_key );

				ImGui::Hotkey( xorstr_( "hide shots key##exploits" ), &cfg.m_hide_shots_key );
			}

			if ( ImGui::CollapsingHeader( xorstr_( "anti aim##main" ) ) ) {
				auto& cfg = hacks::g_anti_aim->cfg( );

				ImGui::Checkbox( xorstr_( "enabled##anti_aim" ), &cfg.m_enabled );

				const char* pitch_list[]{
					( "none" ),
					( "down" ),
					( "up" ),
					( "zero" )
				};

				ImGui::Combo( xorstr_( "pitch##anti_aim" ), &cfg.m_pitch, pitch_list, IM_ARRAYSIZE( pitch_list ) );

				const char* yaw_list[]{
					( "none" ),
					( "backwards" ),
					( "back spin" ),
					( "slow spin" ),
					( "fast spin" ),
					( "random" ),
					( "static" )
				};

				ImGui::Combo( xorstr_( "yaw type##anti_aim" ), &cfg.m_yaw_type, yaw_list, IM_ARRAYSIZE( yaw_list ) );

				ImGui::Checkbox( xorstr_( "jitter side##anti_aim" ), &cfg.m_jitter_side );
				ImGui::Checkbox( xorstr_( "at target##anti_aim" ), &cfg.m_at_target );

				if ( cfg.m_at_target ) {
					const char* at_target_list[]{
						( "closest" ),
						( "average" ),
						( "fov" )
					};

					ImGui::Combo( xorstr_( "at target type##anti_aim" ), &cfg.m_at_target_type, at_target_list, IM_ARRAYSIZE( at_target_list ) );
				}

				ImGui::SliderFloat( xorstr_( "yaw offset##anti_aim" ), &cfg.m_yaw_offset, -180.f, 180.f );
				ImGui::SliderFloat( xorstr_( "yaw offset inverted##anti_aim" ), &cfg.m_yaw_offset_inverted, -180.f, 180.f );
				ImGui::SliderFloat( xorstr_( "jitter amount##anti_aim" ), &cfg.m_jitter_amount, 0.f, 180.f );

				ImGui::Hotkey( xorstr_( "manual left key##anti_aim" ), &cfg.m_manual_left_key );
				ImGui::Hotkey( xorstr_( "manual right key##anti_aim" ), &cfg.m_manual_right_key );

				ImGui::Checkbox( xorstr_( "desync##anti_aim" ), &cfg.m_desync );

				if ( cfg.m_desync ) {
					ImGui::Hotkey( xorstr_( "side inverter key##anti_aim" ), &cfg.m_side_invert_key );

					ImGui::SliderFloat( xorstr_( "desync limit##anti_aim" ), &cfg.m_desync_limit, 0.f, 60.f );

					const char* on_shot_side_list[]{
						( "none" ),
						( "left" ),
						( "right" ),
						( "opposite" ),
						( "switch" )
					};

					ImGui::Combo( xorstr_( "on shot side##anti_aim" ), &cfg.m_on_shot_side, on_shot_side_list, IM_ARRAYSIZE( on_shot_side_list ) );
				}

				const char* auto_dir_list[]{
					( "none" ),
					( "traditional" ),
					( "peek desync" ),
					( "peek real" )
				};

				ImGui::Combo( xorstr_( "auto direction##anti_aim" ), &cfg.m_auto_dir_type, auto_dir_list, IM_ARRAYSIZE( auto_dir_list ) );

				ImGui::Checkbox( xorstr_( "fake lag##anti_aim" ), &cfg.m_fake_lag.m_enabled );

				if ( cfg.m_fake_lag.m_enabled ) {
					ImGui::Checkbox( xorstr_( "adaptive##anti_aim" ), &cfg.m_fake_lag.m_adaptive );

					if ( ImGui::BeginCombo( xorstr_( "conditions##anti_aim" ), "" ) ) {
						const char* fake_lag_cond_list[]{
							( "always##anti_aim" ),
							( "on move##anti_aim" ),
							( "in air##anti_aim" )
						};

						static bool fake_lag_cond_vars[ IM_ARRAYSIZE( fake_lag_cond_list ) ]{};

						for ( std::size_t i{}; i < IM_ARRAYSIZE( fake_lag_cond_list ); ++i ) {
							fake_lag_cond_vars[ i ] = cfg.m_fake_lag.m_conditions & ( 1 << i );

							ImGui::Selectable(
								fake_lag_cond_list[ i ], &fake_lag_cond_vars[ i ],
								ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups
							);

							if ( fake_lag_cond_vars[ i ] )
								cfg.m_fake_lag.m_conditions |= ( 1 << i );
							else
								cfg.m_fake_lag.m_conditions &= ~( 1 << i );
						}

						ImGui::EndCombo( );
					}

					ImGui::SliderInt( xorstr_( "limit##anti_aim" ), &cfg.m_fake_lag.m_limit, 0, 14 );
				}
			}

			if ( ImGui::CollapsingHeader( xorstr_( "visuals - player##main" ) ) ) {
				auto& cfg = hacks::g_visuals->cfg( ).m_player;

				ImGui::Checkbox( xorstr_( "teammates##visuals_player" ), &cfg.m_teammates );

				ImGui::Checkbox( xorstr_( "dormant##visuals_player" ), &cfg.m_dormant );

				ImGui::Checkbox( xorstr_( "bbox##visuals_player" ), &cfg.m_bbox );
				ImGui::ColorEdit3( xorstr_( "bbox clr##visuals_player" ), cfg.m_bbox_clr );

				ImGui::Checkbox( xorstr_( "health bar##visuals_player" ), &cfg.m_health_bar );

				ImGui::Checkbox( xorstr_( "name##visuals_player" ), &cfg.m_name );
				ImGui::ColorEdit3( xorstr_( "name clr##visuals_player" ), cfg.m_name_clr );

				ImGui::Checkbox( xorstr_( "flags##visuals_player" ), &cfg.m_flags );

				ImGui::Checkbox( xorstr_( "weapon text##visuals_player" ), &cfg.m_wpn_text );
				ImGui::ColorEdit3( xorstr_( "weapon text clr##visuals_player" ), cfg.m_wpn_clr );

				ImGui::Checkbox( xorstr_( "ammo##visuals_player" ), &cfg.m_ammo );
				ImGui::ColorEdit3( xorstr_( "ammo clr##visuals_player" ), cfg.m_ammo_clr );

				ImGui::Checkbox( xorstr_( "distance##visuals_player" ), &cfg.m_dist );

				ImGui::Checkbox( xorstr_( "glow##visuals_player" ), &cfg.m_glow );
				ImGui::ColorEdit4( xorstr_( "glow clr##visuals_player" ), cfg.m_glow_clr );

				ImGui::Checkbox( xorstr_( "hit marker##visuals_player" ), &cfg.m_hit_marker );
				ImGui::Checkbox( xorstr_( "hit marker sound##visuals_player" ), &cfg.m_hit_marker_sound );

				ImGui::Checkbox( xorstr_( "visualize sounds##visuals_player" ), &cfg.m_sounds );
				ImGui::ColorEdit3( xorstr_( "visualize sounds clr##visuals_player" ), cfg.m_sounds_clr );

				ImGui::Checkbox( xorstr_( "money##visuals_player" ), &cfg.m_money );

				ImGui::Checkbox( xorstr_( "skeleton##visuals_player" ), &cfg.m_skeleton );
				ImGui::ColorEdit3( xorstr_( "skeleton clr##visuals_player" ), cfg.m_skeleton_clr );

				ImGui::Checkbox( xorstr_( "oof arrow##visuals_player" ), &cfg.m_oof_arrow );
				ImGui::ColorEdit3( xorstr_( "oof arrow clr##visuals_player" ), cfg.m_oof_arrow_clr );
				if ( cfg.m_oof_arrow ) {
					ImGui::SliderInt( xorstr_( "oof arrow size##visuals_player" ), &cfg.m_oof_arrow_size, 1, 48 );
					ImGui::SliderInt( xorstr_( "oof arrow range##visuals_player" ), &cfg.m_oof_arrow_dist, 1, 100 );
				}
			}

			if ( ImGui::CollapsingHeader( xorstr_( "visuals - clred mdls##main" ) ) ) {
				auto& cfg = hacks::g_visuals->cfg( ).m_clred_mdls;

				const char* mdl_type_list[]{
					"regular",
					"solid",
					"glow",
					"metallic"
				};

				ImGui::Checkbox( xorstr_( "player##visuals_clred_mdls" ), &cfg.m_player );
				ImGui::ColorEdit4( xorstr_( "player clr##visuals_clred_mdls" ), cfg.m_player_clr );
				ImGui::Checkbox( xorstr_( "player behind wall##visuals_clred_mdls" ), &cfg.m_player_behind_wall );
				ImGui::ColorEdit4( xorstr_( "player behind wall clr##visuals_clred_mdls" ), cfg.m_player_behind_wall_clr );
				if ( cfg.m_player
					|| cfg.m_player_behind_wall )
					ImGui::Combo(
						xorstr_( "player mdl type##visuals_clred_mdls" ),
						&cfg.m_player_mdl_type, mdl_type_list, IM_ARRAYSIZE( mdl_type_list )
					);

				ImGui::Checkbox( xorstr_( "teammate##visuals_clred_mdls" ), &cfg.m_teammate );
				ImGui::ColorEdit4( xorstr_( "teammate clr##visuals_clred_mdls" ), cfg.m_teammate_clr );
				ImGui::Checkbox( xorstr_( "teammate behind wall##visuals_clred_mdls" ), &cfg.m_teammate_behind_wall );
				ImGui::ColorEdit4( xorstr_( "teammate behind wall clr##visuals_clred_mdls" ), cfg.m_teammate_behind_wall_clr );
				if ( cfg.m_teammate
					|| cfg.m_teammate_behind_wall )
					ImGui::Combo(
						xorstr_( "teammate mdl type##visuals_clred_mdls" ),
						&cfg.m_teammate_mdl_type, mdl_type_list, IM_ARRAYSIZE( mdl_type_list )
					);

				ImGui::Checkbox( xorstr_( "local player##visuals_clred_mdls" ), &cfg.m_local_player );
				ImGui::ColorEdit4( xorstr_( "local player clr##visuals_clred_mdls" ), cfg.m_local_player_clr );
				if ( cfg.m_local_player )
					ImGui::Combo(
						xorstr_( "local player mdl type##visuals_clred_mdls" ),
						&cfg.m_local_player_mdl_type, mdl_type_list, IM_ARRAYSIZE( mdl_type_list )
					);

				ImGui::Checkbox( xorstr_( "local player fake##visuals_clred_mdls" ), &cfg.m_local_player_fake );
				ImGui::ColorEdit4( xorstr_( "local player fake clr##visuals_clred_mdls" ), cfg.m_local_player_fake_clr );
				if ( cfg.m_local_player_fake )
					ImGui::Combo(
						xorstr_( "local player fake mdl type##visuals_clred_mdls" ),
						&cfg.m_local_player_fake_mdl_type, mdl_type_list, IM_ARRAYSIZE( mdl_type_list )
					);

				ImGui::Checkbox( xorstr_( "on shot##visuals_clred_mdls" ), &cfg.m_on_shot );
				ImGui::ColorEdit4( xorstr_( "on shot clr##visuals_clred_mdls" ), cfg.m_on_shot_clr );
				if ( cfg.m_on_shot ) {
					ImGui::Combo(
						xorstr_( "on shot mdl type##visuals_clred_mdls" ),
						&cfg.m_on_shot_mdl_type, mdl_type_list, IM_ARRAYSIZE( mdl_type_list )
					);

					ImGui::SliderFloat( xorstr_( "on shot mdl time##visuals_clred_mdls" ), &cfg.m_on_shot_mdl_time, 0.1f, 5.f );
				}

				ImGui::Checkbox( xorstr_( "ragdolls##visuals_clred_mdls" ), &cfg.m_ragdoll );

				ImGui::Checkbox( xorstr_( "hands##visuals_clred_mdls" ), &cfg.m_hands );
				ImGui::ColorEdit4( xorstr_( "hands clr##visuals_clred_mdls" ), cfg.m_hands_clr );
				if ( cfg.m_hands )
					ImGui::Combo(
						xorstr_( "hands mdl type##visuals_clred_mdls" ),
						&cfg.m_hands_mdl_type, mdl_type_list, IM_ARRAYSIZE( mdl_type_list )
					);

				ImGui::Checkbox( xorstr_( "weapon viewmodel##visuals_clred_mdls" ), &cfg.m_wpn_view_model );
				ImGui::ColorEdit4( xorstr_( "weapon viewmodel clr##visuals_clred_mdls" ), cfg.m_wpn_view_model_clr );
				if ( cfg.m_wpn_view_model )
					ImGui::Combo(
						xorstr_( "weapon viewmodel mdl type##visuals_clred_mdls" ),
						&cfg.m_wpn_view_model_mdl_type, mdl_type_list, IM_ARRAYSIZE( mdl_type_list )
					);

				ImGui::Checkbox( xorstr_( "weapons##visuals_clred_mdls" ), &cfg.m_weapons );
				ImGui::ColorEdit4( xorstr_( "weapons clr##visuals_clred_mdls" ), cfg.m_weapons_clr );
				if ( cfg.m_weapons )
					ImGui::Combo(
						xorstr_( "weapons mdl type##visuals_clred_mdls" ),
						&cfg.m_weapons_mdl_type, mdl_type_list, IM_ARRAYSIZE( mdl_type_list )
					);

				ImGui::Checkbox( xorstr_( "shadow##visuals_clred_mdls" ), &cfg.m_shadow );
				ImGui::ColorEdit4( xorstr_( "shadow clr##visuals_clred_mdls" ), cfg.m_shadow_clr );
				if ( cfg.m_shadow )
					ImGui::Combo(
						xorstr_( "shadow mdl type##visuals_clred_mdls" ),
						&cfg.m_shadow_mdl_type, mdl_type_list, IM_ARRAYSIZE( mdl_type_list )
					);

				ImGui::Checkbox( xorstr_( "props##visuals_clred_mdls" ), &cfg.m_props );
				ImGui::ColorEdit3( xorstr_( "props clr##visuals_clred_mdls" ), cfg.m_props_clr );
			}

			if ( ImGui::CollapsingHeader( xorstr_( "visuals - other##main" ) ) ) {
				auto& cfg = hacks::g_visuals->cfg( ).m_other;

				ImGui::Checkbox( xorstr_( "radar##visuals_other" ), &cfg.m_radar );

				ImGui::Checkbox( xorstr_( "dropped weapons##visuals_other" ), &cfg.m_dropped_wpns );
				ImGui::ColorEdit3( xorstr_( "dropped weapons clr##visuals_other" ), cfg.m_dropped_wpns_clr );

				ImGui::Checkbox( xorstr_( "grenades##visuals_other" ), &cfg.m_grenades );
				ImGui::ColorEdit3( xorstr_( "grenades clr##visuals_other" ), cfg.m_grenades_clr );

				ImGui::Checkbox( xorstr_( "inaccuracy overlay##visuals_other" ), &cfg.m_inaccuracy );
				ImGui::ColorEdit3( xorstr_( "inaccuracy overlay clr##visuals_other" ), cfg.m_inaccuracy_clr );

				ImGui::Checkbox( xorstr_( "recoil overlay##visuals_other" ), &cfg.m_recoil );

				ImGui::Checkbox( xorstr_( "crosshair##visuals_other" ), &cfg.m_crosshair );

				ImGui::Checkbox( xorstr_( "bomb##visuals_other" ), &cfg.m_bomb );
				ImGui::ColorEdit4( xorstr_( "bomb clr##visuals_other" ), cfg.m_bomb_clr );

				ImGui::Checkbox( xorstr_( "grenade trajectory##visuals_other" ), &cfg.m_grenade_trajectory );
				ImGui::ColorEdit3( xorstr_( "grenade trajectory clr##visuals_other" ), cfg.m_grenade_trajectory_clr );

				ImGui::Checkbox( xorstr_( "grenade proximity warning##visuals_other" ), &cfg.m_grenade_warning );

				ImGui::Checkbox( xorstr_( "spectators##visuals_other" ), &cfg.m_spectators );

				ImGui::Checkbox( xorstr_( "penetration reticle##visuals_other" ), &cfg.m_penetration_reticle );

				if ( ImGui::BeginCombo( xorstr_( "indicators##visuals_other" ), "" ) ) {
					const char* indicators_list[]{
						( "lag comp##visuals_other_indicators" ),
						( "double tap##visuals_other_indicators" ),
						( "hide shots##visuals_other_indicators" ),
						( "min damage##visuals_other_indicators" ),
						( "force baim##visuals_other_indicators" ),
						( "force safe##visuals_other_indicators" )
					};

					static bool indicators_vars[ IM_ARRAYSIZE( indicators_list ) ]{};

					for ( std::size_t i{}; i < IM_ARRAYSIZE( indicators_list ); ++i ) {
						indicators_vars[ i ] = cfg.m_indicators & ( 1 << i );

						ImGui::Selectable(
							indicators_list[ i ], &indicators_vars[ i ],
							ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups
						);

						if ( indicators_vars[ i ] )
							cfg.m_indicators |= ( 1 << i );
						else
							cfg.m_indicators &= ~( 1 << i );
					}

					ImGui::EndCombo( );
				}
			}

			if ( ImGui::CollapsingHeader( xorstr_( "visuals - effects##main" ) ) ) {
				auto& cfg = hacks::g_visuals->cfg( ).m_effects;

				ImGui::Checkbox( xorstr_( "remove flash##visuals_effects" ), &cfg.m_remove_flash );

				ImGui::Checkbox( xorstr_( "remove smoke##visuals_effects" ), &cfg.m_remove_smoke );

				ImGui::Checkbox( xorstr_( "remove fog##visuals_effects" ), &cfg.m_remove_fog );

				ImGui::Checkbox( xorstr_( "remove skybox##visuals_effects" ), &cfg.m_remove_skybox );

				ImGui::Checkbox( xorstr_( "disable post processing##visuals_effects" ), &cfg.m_disable_post_processing );
				ImGui::Checkbox( xorstr_( "remove scope overlay##visuals_effects" ), &cfg.m_remove_scope_overlay );

				ImGui::Hotkey( xorstr_( "force third person key##visuals_effects" ), &cfg.m_force_thirdperson_key );
				ImGui::Checkbox( xorstr_( "force third person ( alive )##visuals_effects" ), &cfg.m_force_thirdperson_alive );
				ImGui::Checkbox( xorstr_( "force third person ( dead )##visuals_effects" ), &cfg.m_force_thirdperson_dead );
				ImGui::SliderInt( xorstr_( "third person distance##visuals_effects" ), &cfg.m_thirdperson_distance, 50, 150 );

				ImGui::Checkbox( xorstr_( "disable rendering of teammates##visuals_effects" ), &cfg.m_disable_rendering_of_teammates );
				ImGui::Checkbox( xorstr_( "disable rendering of ragdolls##visuals_effects" ), &cfg.m_disable_rendering_of_ragdolls );

				ImGui::SliderInt( xorstr_( "transparent walls##visuals_effects" ), &cfg.m_transparent_walls, 0, 100 );
				ImGui::SliderInt( xorstr_( "transparent props##visuals_effects" ), &cfg.m_transparent_props, 0, 100 );

				const char* recoil_adjustment_list[]{
					"none",
					"remove shake",
					"remove all"
				};

				ImGui::Combo(
					xorstr_( "visual recoil adjustmentpe##visuals_effects" ),
					&cfg.m_recoil_adjustment, recoil_adjustment_list, IM_ARRAYSIZE( recoil_adjustment_list )
				);

				const char* brigthtness_adj_list[]{
					"none",
					"fullbright",
					"nightmode"
				};

				ImGui::Combo(
					xorstr_( "brightness adjustment##visuals_effects" ),
					&cfg.m_brightness_adjustment, brigthtness_adj_list, IM_ARRAYSIZE( brigthtness_adj_list )
				);

				if ( cfg.m_brightness_adjustment == 2 )
					ImGui::ColorEdit3( xorstr_( "brightess mod##visuals_effects" ), cfg.m_brightess_mod );

				ImGui::Checkbox( xorstr_( "bullet tracers##visuals_effects" ), &cfg.m_bullet_tracers );
				ImGui::ColorEdit3( xorstr_( "bullet tracers clr##visuals_other" ), cfg.m_bullet_tracers_clr );

				ImGui::Checkbox( xorstr_( "bullet impacts##visuals_effects" ), &cfg.m_bullet_impacts );

				ImGui::Checkbox( xorstr_( "fov changer##visuals_effects" ), &cfg.m_fov_changer );

				if ( cfg.m_fov_changer )
					ImGui::SliderInt( xorstr_( "fov##visuals_effects" ), &cfg.m_fov, -20, 40 );

				ImGui::Checkbox( xorstr_( "viewmodel fov changer##visuals_effects" ), &cfg.m_viewmodel_fov_changer );

				if ( cfg.m_viewmodel_fov_changer )
					ImGui::SliderInt( xorstr_( "viewmodel fov##visuals_effects" ), &cfg.m_viewmodel_fov, -20, 40 );

				ImGui::Checkbox( xorstr_( "aspect ratio changer##visuals_effects" ), &cfg.m_aspect_ratio_changer );

				if ( cfg.m_aspect_ratio_changer )
					ImGui::SliderInt( xorstr_( "aspect ratio##visuals_effects" ), &cfg.m_aspect_ratio, 100, 200 );
			}

			if ( ImGui::CollapsingHeader( xorstr_( "movement##main" ) ) ) {
				auto& cfg = hacks::g_movement->cfg( );

				ImGui::Checkbox( xorstr_( "anti untrusted##movement" ), &cfg.m_anti_untrusted );

				ImGui::Checkbox( xorstr_( "slide walk##movement" ), &cfg.m_slide_walk );

				ImGui::Checkbox( xorstr_( "slow walk##movement" ), &cfg.m_slow_walk );

				ImGui::Hotkey( xorstr_( "slow walk key##movement" ), &cfg.m_slow_walk_key );

				ImGui::SliderFloat( xorstr_( "slow walk speed##movement" ), &cfg.m_slow_walk_speed, 0.f, 260.f );

				ImGui::Hotkey( xorstr_( "fake duck key##movement" ), &cfg.m_fake_duck_key );

				ImGui::Hotkey( xorstr_( "auto peek key##movement" ), &cfg.m_auto_peek_key );

				ImGui::Checkbox( xorstr_( "remove crouch cooldown##movement" ), &cfg.m_remove_crouch_cooldown );

				ImGui::Checkbox( xorstr_( "auto bhop##movement" ), &cfg.m_auto_bhop );

				ImGui::Checkbox( xorstr_( "fast stop##movement" ), &cfg.m_fast_stop );

				const char* auto_strafe_list[]{
					( "none" ),
					( "view angles" ),
					( "directional" )
				};

				ImGui::Combo( xorstr_( "auto strafe##movement" ), &cfg.m_auto_strafe_type, auto_strafe_list, IM_ARRAYSIZE( auto_strafe_list ) );
			}

			if ( ImGui::CollapsingHeader( xorstr_( "skins##main" ) ) ) {
				static std::size_t cur_wpn{}, cur_paint_kit[ hacks::c_skins::k_max_wpns ]{};
				static bool sort_by_weapon{false};

				auto& cfg = hacks::g_skins->cfg( );

				if ( ImGui::BeginCombo( xorstr_( "knife##skins" ), hacks::g_skins->knives( ).at( cfg.m_knife_index ).m_name.data( ) ) ) {
					for ( std::size_t i{}; i < hacks::c_skins::k_max_knives; ++i ) {
						const auto& knife = hacks::g_skins->knives( ).at( i );

						if ( ImGui::Selectable( knife.m_name.data( ), cfg.m_knife_index == i ) )
							cfg.m_knife_index = i;
					}

					ImGui::EndCombo( );
				}

				if ( ImGui::BeginCombo( xorstr_( "weapon##skins" ), hacks::g_skins->weapons( ).at( cur_wpn ).m_name.data( ) ) ) {
					for ( std::size_t i{}; i < hacks::c_skins::k_max_wpns; ++i ) {
						const auto& weapon = hacks::g_skins->weapons( ).at( i );

						if ( ImGui::Selectable( weapon.m_name.data( ), cur_wpn == i ) )
							cur_wpn = i;
					}

					ImGui::EndCombo( );
				}

				static char search_filter[ 64u ]{};

				ImGui::InputText( xorstr_( "search filter##skins" ), search_filter, 64u );

				auto& menu_paint_kit = cur_paint_kit[ cur_wpn ];
				auto& cfg_paint_kit = cfg.m_paint_kits.at( cur_wpn );

				static const auto paint_kits_count = hacks::g_skins->paint_kits( ).size( );
				for ( std::size_t i{}; i < paint_kits_count; ++i ) {
					const auto& paint_kit = hacks::g_skins->paint_kits( ).at( i );

					if ( cfg_paint_kit == paint_kit.m_id ) {
						menu_paint_kit = i;
						break;
					}
				}

				if ( ImGui::BeginCombo( xorstr_( "paint kit##skins" ), hacks::g_skins->paint_kits( ).at( menu_paint_kit ).m_name.data( ) ) ) {
					for ( std::size_t i{}; i < paint_kits_count; ++i ) {
						const auto& paint_kit = hacks::g_skins->paint_kits( ).at( i );

						std::string paint_name = paint_kit.m_name;

						for ( auto i = 0; i < paint_name.size( ); i++ )
							if ( iswalpha( paint_name.at( i ) ) )
								paint_name.at( i ) = towlower( paint_name.at( i ) );

						std::string search = search_filter;

						for ( auto i = 0; i < search.size( ); i++ )
							if ( iswalpha( search.at( i ) ) )
								search.at( i ) = towlower( search.at( i ) );

						if ( paint_name.find( search_filter ) == std::string::npos )
							continue;

						if ( ImGui::Selectable( paint_kit.m_name.data( ), cfg_paint_kit == paint_kit.m_id ) ) {
							cfg_paint_kit = paint_kit.m_id;
							menu_paint_kit = i;
						}
					}

					ImGui::EndCombo( );
				}

				ImGui::Checkbox( xorstr_( "include weapon name" ), &sort_by_weapon );

				if ( ImGui::Button( xorstr_( "update" ) ) )
					hacks::g_skins->force_update( ) = true;
			}

			if ( ImGui::CollapsingHeader( xorstr_( "cfg##main" ) ) ) {
				static char name[ 64u ]{};

				ImGui::InputText( xorstr_( "name##cfg" ), name, 64u );

				if ( ImGui::Button( xorstr_( "load##cfg" ) ) )
					sdk::g_cfg->load( name );

				if ( ImGui::Button( xorstr_( "save##cfg" ) ) )
					sdk::g_cfg->save( name );
			}

			ImGui::End( );
		}

		ImGui::EndFrame( );

		ImGui::Render( );

		ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData( ) );

		return orig_d3d9_present( device, src_rect, dst_rect, dst_wnd_override, dirty_region );
	}

	void* __fastcall alloc_key_values_memory( const std::uintptr_t ecx, const std::uintptr_t edx, const int size ) {
		const auto ret_addr = *reinterpret_cast< std::uintptr_t* >( _AddressOfReturnAddress( ) );
		if ( ret_addr == g_ctx->addresses( ).m_alloc_key_values_engine
			|| ret_addr == g_ctx->addresses( ).m_alloc_key_values_client )
			return nullptr;

		return orig_alloc_key_values_memory( ecx, edx, size );
	}

	void __fastcall paint( const std::uintptr_t ecx, const std::uintptr_t edx, const int mode ) {
		orig_paint( ecx, edx, mode );

		if ( mode & 1 || mode & 2 ) {
			valve::g_surface->start_drawing( );

			hacks::g_visuals->on_paint( );

			valve::g_surface->finish_drawing( );
		}
	}

	int __fastcall sv_cheats_get_int( valve::cvar_t* const ecx, const std::uintptr_t edx ) {
		if ( *reinterpret_cast< std::uintptr_t* >( _AddressOfReturnAddress( ) ) != g_ctx->addresses( ).m_cam_sv_cheats_ret )
			return orig_sv_cheats_get_int( ecx, edx );

		return 1;
	}

	void __fastcall draw_model(
		valve::studio_render_t* const ecx, const std::uintptr_t edx,
		const std::uintptr_t a0, const valve::draw_model_info_t& info,
		mat3x4_t* const bones, float* const a3, float* const a4, const vec3_t& origin, int flags
	) {
		if ( !hacks::g_visuals->on_draw_model( ecx, edx, a0, info, bones, a3, a4, origin, flags ) )
			return;

		orig_draw_model( ecx, edx, a0, info, bones, a3, a4, origin, flags );
	}

	void __fastcall lock_cursor( const std::uintptr_t ecx, const std::uintptr_t edx ) {
		using unlock_cursor_t = void( __thiscall* )( const std::uintptr_t );

		if ( g_imgui_opened )
			return ( *reinterpret_cast< unlock_cursor_t** >( ecx ) )[ 66u ]( ecx );

		orig_lock_cursor( ecx, edx );
	}

	void __fastcall on_screen_size_change( const std::uintptr_t ecx, const std::uintptr_t edx, int old_width, int old_height ) {
		orig_on_screen_size_change( ecx, edx, old_width, old_height );

		hacks::g_visuals->init( );
	}

	void __fastcall draw_set_clr(
		valve::c_surface* const ecx, const std::uintptr_t edx, const int r, const int g, const int b, const int a
	) {
		if ( !hacks::g_visuals->cfg( ).m_effects.m_remove_scope_overlay )
			return orig_draw_set_clr( ecx, edx, r, g, b, a );

		const auto ret_addr = *reinterpret_cast< std::uintptr_t* >( _AddressOfReturnAddress( ) );
		if ( ret_addr == g_ctx->addresses( ).m_ret_to_scope_arc
			|| ret_addr == g_ctx->addresses( ).m_ret_to_scope_lens )
			return orig_draw_set_clr( ecx, edx, r, g, b, 0 );

		if ( ret_addr != g_ctx->addresses( ).m_ret_to_scope_clear 
			&& ret_addr != g_ctx->addresses( ).m_ret_to_scope_blurry )
			return orig_draw_set_clr( ecx, edx, r, g, b, a );

		orig_draw_set_clr( ecx, edx, 0, 0, 0, 255 );

		const auto& screen_size = ImGui::GetIO( ).DisplaySize;
		const auto center = screen_size / 2.f;

		ecx->add_line( { center.x, 0.f }, { center.x, screen_size.y } );
		ecx->add_line( { 0.f, center.y }, { screen_size.x, center.y } );

		orig_draw_set_clr( ecx, edx, r, g, b, 0 );
	}

	__declspec ( naked ) void __stdcall create_move_proxy( const int seq_number, const float input_sample_frame_time, const bool active ) {
		__asm {
			push ebp
			mov ebp, esp
			push ebx
			push esp
			push dword ptr[ ebp + 16 ]
			push dword ptr[ ebp + 12 ]
			push dword ptr[ ebp + 8 ]
			call create_move
			pop ebx
			pop ebp
			retn 12
		}
	}

	void __stdcall create_move( const int seq_number, const float input_sample_frame_time, const bool active, bool& send_packet ) {
		orig_create_move( valve::g_client, seq_number, input_sample_frame_time, active );

		send_packet = true;

		hacks::g_movement->should_fake_duck( ) = false;

		if ( !valve::g_local_player )
			return;

		auto& user_cmd = valve::g_input->m_cmds[ seq_number % 150 ];
		if ( !user_cmd.m_number )
			return;

		const auto net_channel_info = valve::g_engine->net_channel_info( );
		if ( !net_channel_info )
			return;

		{
			auto& net_info = g_ctx->net_info( );

			net_info.m_lerp = std::max(
				g_ctx->cvars( ).m_cl_interp->get_float( ),
				g_ctx->cvars( ).m_cl_interp_ratio->get_float( ) / g_ctx->cvars( ).m_cl_updaterate->get_float( )
			);
			net_info.m_latency = { net_channel_info->latency( 1 ), net_channel_info->latency( 0 ) };
		}

		if ( !valve::g_local_player->alive( ) )
			return;

		if ( ( g_ctx->weapon( ) = valve::g_local_player->weapon( ) ) )
			g_ctx->wpn_data( ) = g_ctx->weapon( )->wpn_data( );
		else
			g_ctx->wpn_data( ) = nullptr;

		{
			static float prev_spawn_time = valve::g_local_player->spawn_time( );
			if ( prev_spawn_time != valve::g_local_player->spawn_time( ) ) {
				hacks::g_eng_pred->reset_on_spawn( );

				hacks::g_exploits->ticks_allowed( ) = 0;

				prev_spawn_time = valve::g_local_player->spawn_time( );
			}
		}

		hacks::g_movement->should_fake_duck( )
			= hacks::g_movement->cfg( ).m_fake_duck_key
			&& HIWORD( GetKeyState( hacks::g_movement->cfg( ).m_fake_duck_key ) )
			&& valve::g_local_player->flags( ) & valve::e_ent_flags::on_ground
			&& !( user_cmd.m_buttons & valve::e_buttons::in_jump );

		hacks::g_eng_pred->old_user_cmd( ) = user_cmd;

		if ( !hacks::g_exploits->try_to_recharge( ) ) {
			hacks::g_eng_pred->prepare( );

			hacks::g_exploits->manage_wpn( user_cmd );

			if ( g_ctx->weapon( ) && g_ctx->wpn_data( )
				&& !( g_ctx->flags( ) & e_context_flags::can_shoot )
				&& ( ( static_cast< std::size_t >( g_ctx->wpn_data( )->m_unk_type - 2 ) <= 5
					|| g_ctx->weapon( )->item_index( ) == valve::e_item_index::taser )
					&& g_ctx->weapon( )->item_index( ) != valve::e_item_index::revolver ) )
				user_cmd.m_buttons &= ~valve::e_buttons::in_attack;

			hacks::g_movement->on_create_move( user_cmd );

			hacks::g_visuals->on_create_move( user_cmd );

			hacks::g_eng_pred->predict( &user_cmd );

			hacks::g_anti_aim->choke( &user_cmd );

			if ( g_ctx->flags( ) & e_context_flags::recharge ) {
				if ( !hacks::g_movement->should_fake_duck( )
					&& !hacks::g_exploits->shift( )
					&& !hacks::g_exploits->charged( ) )
					g_ctx->flags( ) &= ~e_context_flags::choke;

				user_cmd.m_buttons &= ~valve::e_buttons::in_attack;
			}
			else {
				hacks::g_aim_bot->on_create_move( user_cmd );

				if ( user_cmd.m_buttons & valve::e_buttons::in_attack
					&& !( g_ctx->flags( ) & e_context_flags::aim_fire )
					&& g_ctx->flags( ) & e_context_flags::can_shoot ) {
					user_cmd.m_view_angles -= valve::g_local_player->aim_punch( ) * g_ctx->cvars( ).m_weapon_recoil_scale->get_float( );

					user_cmd.m_view_angles.x = std::remainder( user_cmd.m_view_angles.x, 360.f );
					user_cmd.m_view_angles.y = std::remainder( user_cmd.m_view_angles.y, 360.f );
					user_cmd.m_view_angles.z = std::remainder( user_cmd.m_view_angles.z, 360.f );

					user_cmd.m_view_angles.x = std::clamp( user_cmd.m_view_angles.x, -89.f, 89.f );
					user_cmd.m_view_angles.y = std::clamp( user_cmd.m_view_angles.y, -180.f, 180.f );
					user_cmd.m_view_angles.z = std::clamp( user_cmd.m_view_angles.z, -90.f, 90.f );
				}
			}

			hacks::g_anti_aim->set_pitch( user_cmd );

			hacks::g_movement->normalize( user_cmd );

			hacks::g_movement->rotate(
				user_cmd, hacks::g_eng_pred->old_user_cmd( ).m_view_angles,
				valve::g_local_player->flags( ), valve::g_local_player->move_type( )
			);

			if ( g_ctx->flags( ) & e_context_flags::can_shoot
				&& g_ctx->will_shoot( g_ctx->weapon( ), user_cmd ) ) {
				auto& anim_data = hacks::g_anim_sync->local_data( );

				anim_data.m_shot_cmd_number = user_cmd.m_number;

				if ( g_ctx->wpn_data( ) ) {
					if ( static_cast< std::size_t >( g_ctx->wpn_data( )->m_unk_type - 2 ) > 5 )
						anim_data.m_shot_valid_wpn = g_ctx->weapon( )->item_index( ) == valve::e_item_index::taser;
					else
						anim_data.m_shot_valid_wpn = true;
				}
				else
					anim_data.m_shot_valid_wpn = false;

				if ( anim_data.m_shot_valid_wpn ) {
					if ( !hacks::g_movement->should_fake_duck( ) ) {
						hacks::g_exploits->cur_shift_amount( ) = hacks::g_exploits->next_shift_amount( );

						if ( hacks::g_exploits->force_choke( )
							&& g_ctx->flags( ) & e_context_flags::can_choke )
							g_ctx->flags( ) |= e_context_flags::choke;
						else
							g_ctx->flags( ) &= ~e_context_flags::choke;
					}

					if ( g_ctx->flags( ) & e_context_flags::aim_fire ) {
						const auto& aim_target = hacks::g_aim_bot->last_target( );

						hacks::g_shots->add(
							g_ctx->shoot_pos( ), aim_target.m_entry,
							aim_target.m_lag_record, aim_target.m_dmg,
							hacks::g_exploits->next_shift_amount( )
						);
					}
					else
						hacks::g_shots->add(
							g_ctx->shoot_pos( ), nullptr, nullptr,
							0, hacks::g_exploits->next_shift_amount( )
						);
				}
			}

			hacks::g_anim_sync->update_local_real( user_cmd );

			hacks::g_eng_pred->restore( );

			if ( g_ctx->weapon( ) ) {
				g_ctx->weapon( )->recoil_index( ) = hacks::g_eng_pred->recoil_index( );
				g_ctx->weapon( )->accuracy_penalty( ) = hacks::g_eng_pred->accuracy_penalty( );
			}

			send_packet = !( g_ctx->flags( ) & e_context_flags::choke );
		}
		else {
			send_packet = false;

			user_cmd.m_tick = std::numeric_limits< int >::max( );

			hacks::g_anim_sync->setup_local_bones( );

			auto& local_data = hacks::g_eng_pred->local_data( ).at( user_cmd.m_number % 150 );

			local_data.init( user_cmd );
			local_data.save( user_cmd.m_number );
		}

		hacks::g_eng_pred->local_data( ).at( user_cmd.m_number % 150 ).m_move = hacks::g_eng_pred->old_user_cmd( ).m_move;

		hacks::g_movement->prev_view_yaw( ) = hacks::g_eng_pred->old_user_cmd( ).m_view_angles.y;

		hacks::g_movement->normalize( user_cmd );

		bool break_lc{};

		if ( valve::g_client_state->m_last_cmd_out != hacks::g_exploits->recharge_cmd( )
			&& hacks::g_movement->is_peeking( )
			&& hacks::g_exploits->next_shift_amount( )
			&& !hacks::g_exploits->cur_shift_amount( )
			&& ( hacks::g_exploits->type( ) == 2 || hacks::g_exploits->type( ) == 3 ) ) {
			hacks::g_exploits->type( ) = 5;

			auto& local_data = hacks::g_eng_pred->local_data( ).at( user_cmd.m_number % 150 );

			local_data.m_override_tick_base = local_data.m_restore_tick_base = true;
			local_data.m_adjusted_tick_base = local_data.m_tick_base - hacks::g_exploits->next_shift_amount( );

			break_lc = true;
		}

		if ( !send_packet ) {
			auto& net_channel = valve::g_client_state->m_net_channel;

			const auto backup_choked_packets = net_channel->m_choked_packets;

			net_channel->m_choked_packets = 0;
			net_channel->send_datagram( 0 );
			--net_channel->m_out_seq_number;

			net_channel->m_choked_packets = backup_choked_packets;
		}
		else {
			g_ctx->broke_lc( ) = ( valve::g_local_player->origin( ) - g_ctx->last_sent_origin( ) ).length_2d_sqr( ) > 4096.f;

			g_ctx->last_sent_origin( ) = valve::g_local_player->origin( );

			if ( !hacks::g_exploits->cur_shift_amount( )
				|| ( hacks::g_exploits->type( ) != 3 && hacks::g_exploits->type( ) != 4 ) )
				g_ctx->add_packet( user_cmd.m_number );

			if ( valve::g_client_state->m_last_cmd_out == hacks::g_exploits->recharge_cmd( ) ) {
				auto& local_data = hacks::g_eng_pred->local_data( ).at( user_cmd.m_number % 150 );

				local_data.m_override_tick_base = true;
				local_data.m_adjusted_tick_base = hacks::g_exploits->adjust_tick_base(
					valve::g_client_state->m_choked_cmds + 1, 1, -valve::g_client_state->m_choked_cmds
				);
			}
			else if ( break_lc ) {
				hacks::g_exploits->type( ) = 5;
				hacks::g_exploits->cur_shift_amount( ) = hacks::g_exploits->next_shift_amount( );
			}
		}

		g_ctx->last_cmd_number( ) = user_cmd.m_number;

		hacks::g_exploits->charged( ) = false;

		if ( user_cmd.m_tick != std::numeric_limits< int >::max( )
			&& send_packet ) {
			if ( !hacks::g_exploits->cur_shift_amount( )
				|| hacks::g_exploits->type( ) == 3
				|| hacks::g_exploits->type( ) == 4 )
				if ( valve::g_client_state->m_last_cmd_out != hacks::g_exploits->recharge_cmd( ) )
					g_ctx->last_sent_cmd_number( ) = user_cmd.m_number;
		}

		if ( g_ctx->flags( ) & e_context_flags::aim_fire
			|| user_cmd.m_buttons & valve::e_buttons::in_attack )
			hacks::g_anim_sync->local_data( ).m_last_shot_time = valve::g_global_vars->m_cur_time;

		hacks::g_eng_pred->last_user_cmd( ) = user_cmd;

		valve::g_local_player->velocity_modifier( ) = hacks::g_eng_pred->velocity_modifier( );

		valve::g_input->m_verified_cmds[ seq_number % 150 ] = { user_cmd, user_cmd.calc_checksum( ) };
	}

	void __stdcall frame_stage_notify( const valve::e_frame_stage stage ) {
		if ( !valve::g_local_player )
			return orig_frame_stage_notify( stage );

		hacks::g_skins->on_frame_stage( stage );

		const auto in_game = valve::g_engine->in_game( );

		if ( stage == valve::e_frame_stage::render_start ) {
			hacks::g_visuals->on_render_start( );

			if ( in_game ) {
				if ( hacks::g_exploits->type( ) == 5 ) {
					auto& var_mapping = valve::g_local_player->var_mapping( );

					for ( int i{}; i < var_mapping.m_interpolated_entries; ++i )
						var_mapping.m_entries.at( i ).m_needs_to_interpolate = false;
				}

				const auto correction_ticks = hacks::g_exploits->calc_correction_ticks( );
				for ( auto i = 1; i <= valve::g_global_vars->m_max_clients; ++i ) {
					const auto player = static_cast< valve::c_player* >(
						valve::g_entity_list->find_entity( i )
					);
					if ( !player
						|| player->dormant( )
						|| !player->alive( )
						|| player == valve::g_local_player )
						continue;

					auto& entry = hacks::g_lag_comp->entry( i - 1 );

					if ( player->sim_time( ) == player->old_sim_time( )
						|| ( entry.m_unk = false, correction_ticks == -1 )
						|| std::abs( valve::to_ticks( player->sim_time( ) ) - valve::g_client_state->m_server_tick ) <= ( correction_ticks + 1 ) ) {
						if ( !entry.m_unk )
							continue;
					}
					else
						entry.m_unk = true;

					auto& var_mapping = player->var_mapping( );

					for ( int j{}; j < var_mapping.m_interpolated_entries; ++j )
						var_mapping.m_entries.at( j ).m_needs_to_interpolate = false;
				}
			}
		}

		orig_frame_stage_notify( stage );

		if ( stage == valve::e_frame_stage::net_update_end ) {
			if ( in_game ) {
				const auto& vis_cfg = hacks::g_visuals->cfg( );
				if ( vis_cfg.m_clred_mdls.m_ragdoll
					|| vis_cfg.m_effects.m_disable_rendering_of_ragdolls ) {
					for ( auto i = 1; i <= valve::g_entity_list->highest_index( ); ++i ) {
						const auto entity = valve::g_entity_list->find_entity( i );
						if ( !entity )
							continue;

						const auto client_class = entity->client_class( );
						if ( !client_class
							|| client_class->m_class_id != valve::e_class_id::ragdoll )
							continue;

						entity->use_fast_path( ) = false;
					}
				}

				const auto correction_ticks = hacks::g_exploits->calc_correction_ticks( );
				if ( correction_ticks == -1 )
					hacks::g_exploits->correction_amount( ) = 0;
				else {
					if ( valve::g_local_player->sim_time( ) > valve::g_local_player->old_sim_time( ) ) {
						const auto delta = valve::to_ticks( valve::g_local_player->sim_time( ) ) - valve::g_client_state->m_server_tick;
						if ( std::abs( delta ) <= correction_ticks )
							hacks::g_exploits->correction_amount( ) = delta;
					}
				}

				valve::g_engine->fire_events( );
			}

			hacks::g_shots->on_net_update( );

			hacks::g_lag_comp->on_net_update( );
		}
		else if ( stage == valve::e_frame_stage::render_end )
			hacks::g_visuals->on_render_end( );

		if ( in_game ) {
			if ( const auto view_model = valve::g_local_player->view_model( ) ) {
				static float cycle{}, anim_time{};

				if ( stage == valve::e_frame_stage::net_update_post_data_update_start ) {
					view_model->cycle( ) = cycle;
					view_model->anim_time( ) = anim_time;
				}

				cycle = view_model->cycle( );
				anim_time = view_model->anim_time( );
			}
		}
	}
	
	void __fastcall update_client_side_anim( valve::c_player* const ecx, const std::uintptr_t edx ) {
		if ( !ecx->alive( )
			|| ecx->index( ) < 1
			|| ecx->index( ) > 64 )
			return orig_update_client_side_anim( ecx, edx );

		if ( !g_ctx->allow_anim_update( ) ) {
			const auto mdl_bone_counter = **reinterpret_cast< unsigned long** >(
				g_ctx->addresses( ).m_invalidate_bone_cache + 0xau
			);

			static auto prev_mdl_bone_counter = ecx->mdl_bone_counter( );

			if ( mdl_bone_counter != prev_mdl_bone_counter )
				hacks::g_anim_sync->setup_local_bones( );

			prev_mdl_bone_counter = mdl_bone_counter;

			return;
		}

		for ( auto& layer : ecx->anim_layers( ) )
			layer.m_owner = ecx;

		orig_update_client_side_anim( ecx, edx );
	}

	bool __fastcall setup_bones(
		const std::uintptr_t ecx, const std::uintptr_t edx, mat3x4_t* const bones, int max_bones, int mask, float time
	) {
		const auto player = reinterpret_cast< valve::c_player* >( ecx - sizeof( std::uintptr_t ) );
		if ( !player->alive( )
			|| player->index( ) < 1
			|| player->index( ) > 64 )
			return orig_setup_bones( ecx, edx, bones, max_bones, mask, time );

		if ( !g_ctx->allow_setup_bones( ) ) {
			if ( !bones 
				|| max_bones == -1 )
				return true;

			if ( player == valve::g_local_player ) {
				std::memcpy(
					bones, hacks::g_anim_sync->local_data( ).m_real.m_bones.data( ),
					max_bones * sizeof( mat3x4_t )
				);
			}
			else {
				const auto& entry = hacks::g_lag_comp->entry( player->index( ) - 1 );

				std::memcpy(
					bones, entry.m_bones.data( ),
					max_bones * sizeof( mat3x4_t )
				);
			}

			return true;
		}

		if ( const auto backup_last_setup_bones_frame = player->last_setup_bones_frame( ) ) {
			const auto backup_frame_count = valve::g_global_vars->m_frame_count;

			const auto index = player->index( );

			auto adjusted = false;

			if ( valve::g_global_vars->m_frame_time < 0.0033333334 ) {
				const auto frame_count = index % 3 + 2 * ( index % 3 ) + 1;

				valve::g_global_vars->m_frame_count = frame_count;
				player->last_setup_bones_frame( ) = frame_count;

				adjusted = true;
			}
			else if ( valve::g_global_vars->m_frame_time < 0.0050000004 ) {
				const auto frame_count = 2 * ( index % 2 ) + 1;

				valve::g_global_vars->m_frame_count = frame_count;
				player->last_setup_bones_frame( ) = frame_count;

				adjusted = true;
			}
			else if ( valve::g_global_vars->m_frame_time < 0.0066666668 ) {
				const auto frame_count = 3 * ( index % 3 );

				valve::g_global_vars->m_frame_count = frame_count;
				player->last_setup_bones_frame( ) = frame_count;

				adjusted = true;
			}

			if ( adjusted ) {
				const auto ret = orig_setup_bones( ecx, edx, bones, max_bones, mask, time );

				valve::g_global_vars->m_frame_count = backup_frame_count;
				player->last_setup_bones_frame( ) = backup_last_setup_bones_frame;

				return ret;
			}
		}

		return orig_setup_bones( ecx, edx, bones, max_bones, mask, time );
	}

	void __fastcall do_extra_bones_processing(
		valve::c_player* const ecx, const std::uintptr_t edx, int a0, int a1, int a2, int a3, int a4, int a5
	) {}

	void __fastcall accumulate_layers(
		valve::c_player* const ecx, const std::uintptr_t edx, int a0, int a1, float a2, int a3
	) {
		if ( ecx->index( ) < 1
			|| ecx->index( ) > 64 )
			return orig_accumulate_layers( ecx, edx, a0, a1, a2, a3 );

		if ( const auto anim_state = ecx->anim_state( ) ) {
			const auto backup_first_update = anim_state->m_first_update;

			anim_state->m_first_update = true;

			orig_accumulate_layers( ecx, edx, a0, a1, a2, a3 );

			anim_state->m_first_update = backup_first_update;

			return;
		}

		orig_accumulate_layers( ecx, edx, a0, a1, a2, a3 );
	}

	void __fastcall standard_blending_rules(
		valve::c_player* const ecx, const std::uintptr_t edx, valve::model_data_t* const mdl_data, int a1, int a2, float a3, int mask
	) {
		if ( ecx->index( ) < 1
			|| ecx->index( ) > 64 )
			return orig_standard_blending_rules( ecx, edx, mdl_data, a1, a2, a3, mask );

		if ( g_ctx->force_bone_mask( ) )
			mask = 0x100;

		const auto bone_index = ecx->lookup_bone( "lean_root" );
		if ( bone_index == -1 )
			return orig_standard_blending_rules( ecx, edx, mdl_data, a1, a2, a3, mask );

		auto& bone_flags = mdl_data->m_bone_flags.at( bone_index );

		const auto backup_bone_flags = bone_flags;
		
		bone_flags = 0u;

		orig_standard_blending_rules( ecx, edx, mdl_data, a1, a2, a3, mask );

		bone_flags = backup_bone_flags;
	}

	void __fastcall packet_start(
		const std::uintptr_t ecx, const std::uintptr_t edx, const int in_seq, const int out_acked
	) {
		if ( !valve::g_local_player 
			|| !valve::g_local_player->alive( ) )
			return orig_packet_start( ecx, edx, in_seq, out_acked );

		auto& sented_cmds = g_ctx->sented_cmds( );
		if ( sented_cmds.empty( )
			|| std::find( sented_cmds.rbegin( ), sented_cmds.rend( ), out_acked ) == sented_cmds.rend( ) )
			return;

		sented_cmds.erase(
			std::remove_if(
				sented_cmds.begin( ), sented_cmds.end( ),
				[ & ] ( const int cmd_number ) {
					return cmd_number < out_acked;
				}
			),
			sented_cmds.end( )
		);

		return orig_packet_start( ecx, edx, in_seq, out_acked );
	}

	void __fastcall packet_end( const std::uintptr_t ecx, const std::uintptr_t edx ) {
		if ( !valve::g_local_player
			|| valve::g_client_state->m_server_tick != valve::g_client_state->m_delta_tick )
			return orig_packet_end( ecx, edx );

		const auto& local_data = hacks::g_eng_pred->local_data( ).at( valve::g_client_state->m_cmd_ack % 150 );
		if ( local_data.m_spawn_time == valve::g_local_player->spawn_time( )
			&& local_data.m_shift_amount > 0
			&& local_data.m_tick_base > valve::g_local_player->tick_base( )
			&& ( local_data.m_tick_base - valve::g_local_player->tick_base( ) ) <= 17 )
			valve::g_local_player->tick_base( ) = local_data.m_tick_base + 1;

		orig_packet_end( ecx, edx );
	}

	void __fastcall physics_simulate( valve::c_player* const ecx, const std::uintptr_t edx ) {
		if ( ecx != valve::g_local_player
			|| !ecx->alive( )
			|| ecx->sim_tick( ) == valve::g_global_vars->m_tick_count )
			return orig_physics_simulate( ecx, edx );

		const auto& user_cmd = ecx->cmd_context( ).m_user_cmd;
		if ( user_cmd.m_tick >= std::numeric_limits< int >::max( ) ) {
			ecx->sim_tick( ) = valve::g_global_vars->m_tick_count;

			return hacks::g_eng_pred->net_vars( ).at( user_cmd.m_number % 150 ).store( user_cmd.m_number );
		}

		if ( user_cmd.m_number == ( valve::g_client_state->m_cmd_ack + 1 ) )
			ecx->velocity_modifier( ) = hacks::g_eng_pred->net_velocity_modifier( );

		hacks::g_eng_pred->net_vars( ).at( ( user_cmd.m_number - 1 ) % 150 ).restore( user_cmd.m_number - 1 );

		const auto backup_tick_base = ecx->tick_base( );

		const auto& local_data = hacks::g_eng_pred->local_data( ).at( user_cmd.m_number % 150 );
		if ( local_data.m_spawn_time == ecx->spawn_time( ) && local_data.m_override_tick_base )
			ecx->tick_base( ) = local_data.m_adjusted_tick_base;

		const auto weapon = ecx->weapon( );
		if ( weapon
			&& weapon->item_index( ) == valve::e_item_index::revolver
			&& weapon->postpone_fire_ready_time( ) == std::numeric_limits< float >::max( ) )
			weapon->postpone_fire_ready_time( ) = hacks::g_eng_pred->postpone_fire_ready_time( );

		orig_physics_simulate( ecx, edx );

		if ( weapon
			&& weapon->item_index( ) == valve::e_item_index::revolver
			&& weapon->postpone_fire_ready_time( ) == std::numeric_limits< float >::max( ) )
			weapon->postpone_fire_ready_time( ) = hacks::g_eng_pred->postpone_fire_ready_time( );

		if ( local_data.m_spawn_time == ecx->spawn_time( )
			&& local_data.m_override_tick_base && local_data.m_restore_tick_base )
			ecx->tick_base( ) = backup_tick_base + ecx->tick_base( ) - local_data.m_adjusted_tick_base;

		ecx->physics_collision_state( ) = 0;

		hacks::g_eng_pred->net_vars( ).at( user_cmd.m_number % 150 ).store( user_cmd.m_number );
	}

	void __fastcall on_latch_interpolated_vars(
		valve::c_player* const ecx, const std::uintptr_t edx, const int flags
	) {
		if ( !valve::g_engine->in_game( )
			|| ecx != valve::g_local_player )
			return orig_on_latch_interpolated_vars( ecx, edx, flags );

		const auto backup_sim_time = ecx->sim_time( );

		const auto& local_data = hacks::g_eng_pred->local_data( ).at( valve::g_client_state->m_cmd_ack % 150 );
		if ( local_data.m_spawn_time == valve::g_local_player->spawn_time( )
			&& local_data.m_shift_amount > 0 )
			ecx->sim_time( ) += valve::to_time( local_data.m_shift_amount );

		orig_on_latch_interpolated_vars( ecx, edx, flags );

		ecx->sim_time( ) = backup_sim_time;
	}

	bool __fastcall should_interpolate( valve::c_player* const ecx, const std::uintptr_t edx ) {
		if ( ecx != valve::g_local_player
			|| valve::g_client_state->m_last_cmd_out != hacks::g_exploits->recharge_cmd( ) )
			return orig_should_interpolate( ecx, edx );

		auto& var_mapping = ecx->var_mapping( );

		for ( int i{}; i < var_mapping.m_interpolated_entries; ++i )
			var_mapping.m_entries.at( i ).m_needs_to_interpolate = false;

		return false;
	}

	bool __fastcall write_user_cmd_delta_to_buffer(
		const std::uintptr_t ecx, const std::uintptr_t edx,
		const int slot, valve::bf_write_t* const buffer,
		int from, int to, const bool is_new_cmd
	) {
		if ( !valve::g_local_player )
			return orig_write_user_cmd_delta_to_buffer( ecx, edx, slot, buffer, from, to, is_new_cmd );

		const auto move_msg = reinterpret_cast< valve::move_msg_t* >( 
			*reinterpret_cast< std::uintptr_t* >(
				reinterpret_cast< std::uintptr_t >( _AddressOfReturnAddress( ) ) - sizeof( std::uintptr_t )
			) - 0x58u
		);

		if ( hacks::g_exploits->cur_shift_amount( )
			|| valve::g_client_state->m_last_cmd_out == hacks::g_exploits->recharge_cmd( ) ) {
			if ( from == -1 ) {
				if ( valve::g_client_state->m_last_cmd_out == hacks::g_exploits->recharge_cmd( ) ) {
					move_msg->m_new_cmds = 1;
					move_msg->m_backup_cmds = 0;

					const auto next_cmd_number = valve::g_client_state->m_choked_cmds + valve::g_client_state->m_last_cmd_out + 1;

					for ( to = next_cmd_number - move_msg->m_new_cmds + 1; to <= next_cmd_number; ++to ) {
						if ( !orig_write_user_cmd_delta_to_buffer( ecx, edx, slot, buffer, from, to, true ) )
							break;

						from = to;
					}
				}
				else if ( hacks::g_exploits->type( ) == 5 ) {
					hacks::g_exploits->handle_break_lc( ecx, edx, slot, buffer, from, to, move_msg );
					return true;
				}
				else
					hacks::g_exploits->handle_other_shift( ecx, edx, slot, buffer, from, to, move_msg );
			}

			return true;
		}
		
		if ( from == -1 ) {
			const auto v86 = std::min( move_msg->m_new_cmds + hacks::g_exploits->ticks_allowed( ), 16 );

			int v69{};

			const auto v70 = v86 - move_msg->m_new_cmds;
			if ( v70 >= 0 )
				v69 = v70;

			hacks::g_exploits->ticks_allowed( ) = v69;
		}

		return orig_write_user_cmd_delta_to_buffer( ecx, edx, slot, buffer, from, to, is_new_cmd );
	}

	void __cdecl velocity_modifier( valve::recv_proxy_data_t* const data, valve::c_entity* const entity, void* const out ) {
		orig_velocity_modifier( data, entity, out );

		if ( !valve::g_local_player
			|| entity->index( ) != valve::g_local_player->index( )
			|| data->m_value.m_float == hacks::g_eng_pred->net_velocity_modifier( ) )
			return;

		hacks::g_eng_pred->net_velocity_modifier( ) = data->m_value.m_float;

		valve::g_prediction->m_prev_ack_had_errors = true;
	}

	void __cdecl sequence( valve::recv_proxy_data_t* const data, valve::c_entity* const entity, void* const out ) {
		if ( valve::g_local_player
			&& valve::g_local_player->alive( ) )
			if ( const auto owner = valve::g_entity_list->find_entity( entity->view_model_owner( ) ) )
				if ( owner->index( ) == valve::g_local_player->index( ) )
					if ( const auto weapon = static_cast< valve::c_weapon* >( valve::g_entity_list->find_entity( entity->view_model_weapon_handle( ) ) ) )
						if ( const auto client_class = weapon->client_class( ); client_class && client_class->m_class_id == valve::e_class_id::knife )
							data->m_value.m_int = hacks::g_skins->correct_sequence( weapon->item_index( ), data->m_value.m_int );

		orig_sequence( data, entity, out );
	}

	void __fastcall modify_eye_pos( valve::anim_state_t* const ecx, const std::uintptr_t edx, vec3_t& eye_pos ) {
		const auto backup_smooth_height_valid = ecx->m_smooth_height_valid;
		const auto backup_camera_smooth_height = ecx->m_camera_shooth_height;

		ecx->m_smooth_height_valid = false;

		orig_modify_eye_pos( ecx, edx, eye_pos );

		ecx->m_smooth_height_valid = backup_smooth_height_valid;
		ecx->m_camera_shooth_height = backup_camera_smooth_height;

		eye_pos.z -= ecx->m_player->view_offset( ).z - std::floor( ecx->m_player->view_offset( ).z );
	}

	bool __fastcall process_temp_entities( const std::uintptr_t ecx, const std::uintptr_t edx, const std::uintptr_t msg ) {
		const auto backup_max_clients = valve::g_client_state->m_max_clients;

		valve::g_client_state->m_max_clients = 1;

		const auto ret = orig_process_temp_entities( ecx, edx, msg );

		valve::g_client_state->m_max_clients = backup_max_clients;

		valve::g_engine->fire_events( );

		return ret;
	}

	void __fastcall level_shutdown( const std::uintptr_t ecx, const std::uintptr_t edx ) {
		hacks::g_eng_pred->reset_on_spawn( );

		hacks::g_shots->elements( ).clear( );

		hacks::g_exploits->recharge_cmd( ) = 
			hacks::g_exploits->ticks_allowed( ) =
				hacks::g_exploits->cur_shift_amount( ) = 
					hacks::g_exploits->next_shift_amount( ) = 0;

		g_ctx->freeze_time( ) = false;

		orig_level_shutdown( ecx, edx );
	}

	float __fastcall hooks::get_screen_aspect_ratio( const std::uintptr_t ecx, const std::uintptr_t edx, const int w, const int h ) {
		const auto& cfg = hacks::g_visuals->cfg( );

		if ( !cfg.m_effects.m_aspect_ratio_changer )
			return orig_get_screen_aspect_ratio( ecx, edx, w, h );

		return cfg.m_effects.m_aspect_ratio / 100.f;
	}

	bool __cdecl glow_effect_spectator(
		valve::c_player* const player, valve::c_player* const local,
		int& style, vec3_t& clr, float& alpha_from, float& alpha_to,
		float& time_from, float& time_to, bool& animate
	) {
		const auto& cfg = hacks::g_visuals->cfg( );
		if ( !cfg.m_player.m_glow
			|| player->friendly( ) )
			return orig_glow_effect_spectator(
				player, local, style, clr, alpha_from, alpha_to,
				time_from, time_to, animate
			);

		style = 0;

		clr.x = cfg.m_player.m_glow_clr[ 0u ];
		clr.y = cfg.m_player.m_glow_clr[ 1u ];
		clr.z = cfg.m_player.m_glow_clr[ 2u ];

		alpha_to = cfg.m_player.m_glow_clr[ 3u ];

		return true;
	}

	void __fastcall get_clr_modulation(
		valve::material_t* const ecx, const std::uintptr_t edx,
		float* const r, float* const g, float* const b
	) {
		orig_get_clr_modulation( ecx, edx, r, g, b );

		const auto texture_group_name = ecx->texture_group( );
		const auto prop_mat = *reinterpret_cast< const std::uint32_t* >( texture_group_name + 8u ) == 't po';

		const auto& cfg = hacks::g_visuals->cfg( );

		/* idc */
		if ( cfg.m_effects.m_remove_skybox
			&& *reinterpret_cast< const std::uint32_t* >( texture_group_name ) == 'BykS' ) {
			*r = *g = *b = 0.f;

			return;
		}

		if ( prop_mat
			&& cfg.m_clred_mdls.m_props ) {
			*r *= cfg.m_clred_mdls.m_props_clr[ 0u ];
			*g *= cfg.m_clred_mdls.m_props_clr[ 1u ];
			*b *= cfg.m_clred_mdls.m_props_clr[ 2u ];

			return;
		}

		if ( cfg.m_effects.m_brightness_adjustment != 2 )
			return;

		if ( !prop_mat
			&& *reinterpret_cast< const std::uint32_t* >( texture_group_name + 3u ) != 't dl' )
			return;

		*r *= cfg.m_effects.m_brightess_mod[ 0u ];
		*g *= cfg.m_effects.m_brightess_mod[ 1u ];
		*b *= cfg.m_effects.m_brightess_mod[ 2u ];
	}

	bool __stdcall is_using_static_prop_debug_modes( ) {
		const auto& cfg = hacks::g_visuals->cfg( );

		return cfg.m_clred_mdls.m_props
			|| cfg.m_effects.m_transparent_walls != 100
			|| cfg.m_effects.m_transparent_props != 100
			|| cfg.m_effects.m_brightness_adjustment == 2;
	}

	int __fastcall list_leaves_in_box(
		const std::uintptr_t ecx, const std::uintptr_t edx,
		const vec3_t& mins, const vec3_t& maxs, const uint16_t* const list, const int max
	) {
		if ( *reinterpret_cast< std::uintptr_t* >( _AddressOfReturnAddress( ) ) != g_ctx->addresses( ).m_ret_insert_into_tree )
			return orig_list_leaves_in_box( ecx, edx, mins, maxs, list, max );

		struct renderable_info_t {
			valve::c_renderable*	m_renderable{};
			std::uintptr_t			m_alpha_property{};
			int						m_enum_count{};
			int						m_render_frame{};
			std::uint16_t			m_first_shadow{};
			std::uint16_t			m_leaf_list{};
			short					m_area{};
			std::uint16_t			m_flags0{};
			std::uint16_t			m_flags1{};
			vec3_t					m_bloated_abs_min{};
			vec3_t					m_bloated_abs_max{};
			vec3_t					m_abs_min{};
			vec3_t					m_abs_max{};
			char					pad0[ 4u ]{};
		};

		const auto info = *reinterpret_cast< renderable_info_t** >(
			reinterpret_cast< std::uintptr_t >( _AddressOfReturnAddress( ) ) + 0x14u
		);
		if ( !info
			|| !info->m_renderable )
			return orig_list_leaves_in_box( ecx, edx, mins, maxs, list, max );

		const auto entity = info->m_renderable->get_client_unknown( )->get_entity( );
		if ( !entity
			|| !entity->is_player( ) )
			return orig_list_leaves_in_box( ecx, edx, mins, maxs, list, max );

		info->m_flags0 &= ~0x100;
		info->m_flags1 |= 0xC0;

		return orig_list_leaves_in_box(
			ecx, edx,
			{ -16384.f, -16384.f, -16384.f },
			{ 16384.f, 16384.f, 16384.f },
			list, max
		);
	}

	bool __fastcall override_cfg(
		valve::c_mat_system* const ecx, const std::uintptr_t edx,
		valve::mat_sys_cfg_t* const cfg, const bool update
	) {
		if ( hacks::g_visuals->cfg( ).m_effects.m_brightness_adjustment == 1 )
			cfg->m_fullbright = true;

		return orig_override_cfg( ecx, edx, cfg, update );
	}

	bool __fastcall fog_enabled( const std::uintptr_t ecx, const std::uintptr_t edx ) {
		if ( hacks::g_visuals->cfg( ).m_effects.m_remove_fog )
			return false;

		return orig_fog_enabled( ecx, edx );
	}

	void __fastcall override_view(
		const std::uintptr_t ecx, const std::uintptr_t edx, valve::view_setup_t* const setup
	) {
		hacks::g_visuals->on_override_view( setup );

		orig_override_view( ecx, edx, setup );
	}

	int __fastcall do_post_screen_space_effects(
		const std::uintptr_t ecx, const std::uintptr_t edx, valve::view_setup_t* const setup
	) {
		hacks::g_visuals->on_post_screen_effects( );

		return orig_do_post_screen_space_effects( ecx, edx, setup );
	}

	int __fastcall weapon_debug_spread_show_get_int( valve::cvar_t* const ecx, const std::uintptr_t edx ) {
		if ( hacks::g_visuals->cfg( ).m_other.m_crosshair 
			&& *reinterpret_cast< std::uintptr_t* >( _AddressOfReturnAddress( ) ) == g_ctx->addresses( ).m_crosshair_ret
			&& valve::g_local_player && !valve::g_local_player->scoped( ) )
			return 3;

		return orig_weapon_debug_spread_show_get_int( ecx, edx );
	}

	void __fastcall calc_view(
		valve::c_player* const ecx, const std::uintptr_t edx,
		vec3_t& origin, qangle_t& angles, float& z_near, float& z_far, float& fov
	) {
		if ( ecx != valve::g_local_player )
			return orig_calc_view( ecx, edx, origin, angles, z_near, z_far, fov );

		hacks::g_visuals->on_calc_view( ecx, edx, origin, angles, z_near, z_far, fov );
	}

	qangle_t* __fastcall get_eye_angles( valve::c_player* const ecx, const std::uintptr_t edx ) {
		if ( ecx != valve::g_local_player
			|| *reinterpret_cast< std::uintptr_t* >( _AddressOfReturnAddress( ) ) != g_ctx->addresses( ).m_ret_to_eye_pos_and_vectors )
			return orig_get_eye_angles( ecx, edx );

		return &hacks::g_eng_pred->local_data( ).at( g_ctx->last_sent_cmd_number( ) % 150 ).m_user_cmd.m_view_angles;
	}

	bool __fastcall is_paused( const std::uintptr_t ecx, const std::uintptr_t edx ) {
		if ( *reinterpret_cast< std::uintptr_t* >( _AddressOfReturnAddress( ) ) == g_ctx->addresses( ).m_ret_to_extrapolation )
			return true;

		return orig_is_paused( ecx, edx );
	}

	void event_listener_t::fire_game_event( valve::game_event_t* const event ) {
		hacks::g_shots->on_new_event( event );
	}
}