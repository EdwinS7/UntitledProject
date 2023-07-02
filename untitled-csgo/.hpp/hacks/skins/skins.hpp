#pragma once

namespace hacks {
	class c_skins {
	public:
		static constexpr auto k_max_wpns = 36u;
		static constexpr auto k_max_knives = 20u;
		//static constexpr auto k_max_gloves = 10u;

	private:
		bool set_paint_kit( valve::c_weapon* const weapon, const bool is_knife );

		struct weapon_t {
			valve::e_item_index m_index{};
			std::string			m_name{};
		};

		/* too lazy to write parser for item_schema */
		std::array< weapon_t, k_max_wpns >		m_weapons {
			weapon_t{ valve::e_item_index::knife, xorstr_( "knife" ) },
			//{ valve::e_item_index::knife, xorstr_( "glove" ) },
			{ valve::e_item_index::ak47, xorstr_( "ak47" ) },
			{ valve::e_item_index::aug, xorstr_( "aug" ) },
			{ valve::e_item_index::awp, xorstr_( "awp" ) },
			{ valve::e_item_index::famas, xorstr_( "famas" ) },
			{ valve::e_item_index::g3sg1, xorstr_( "g3sg1" ) },
			{ valve::e_item_index::galil, xorstr_( "galil" ) },
			{ valve::e_item_index::m249, xorstr_( "m249" ) },
			{ valve::e_item_index::m4a1, xorstr_( "m4a1" ) },
			{ valve::e_item_index::mac10, xorstr_( "mac10" ) },
			{ valve::e_item_index::p90, xorstr_( "p90" ) },
			{ valve::e_item_index::mp5sd, xorstr_( "mp5sd" ) },
			{ valve::e_item_index::ump45, xorstr_( "ump45" ) },
			{ valve::e_item_index::xm1014, xorstr_( "xm1014" ) },
			{ valve::e_item_index::bizon, xorstr_( "bizon" ) },
			{ valve::e_item_index::mag7, xorstr_( "mag7" ) },
			{ valve::e_item_index::negev, xorstr_( "negev" ) },
			{ valve::e_item_index::sawedoff, xorstr_( "sawedoff" ) },
			{ valve::e_item_index::tec9, xorstr_( "tec9" ) },
			{ valve::e_item_index::hkp2000, xorstr_( "hkp2000" ) },
			{ valve::e_item_index::mp7, xorstr_( "mp7" ) },
			{ valve::e_item_index::mp9, xorstr_( "mp9" ) },
			{ valve::e_item_index::nova, xorstr_( "nova" ) },
			{ valve::e_item_index::p250, xorstr_( "p250" ) },
			{ valve::e_item_index::scar20, xorstr_( "scar20" ) },
			{ valve::e_item_index::sg556, xorstr_( "sg556" ) },
			{ valve::e_item_index::ssg08, xorstr_( "ssg08" ) },
			{ valve::e_item_index::cz75a, xorstr_( "cz75a" ) },
			{ valve::e_item_index::deagle, xorstr_( "deagle" ) },
			{ valve::e_item_index::elite, xorstr_( "elite" ) },
			{ valve::e_item_index::fiveseven, xorstr_( "fiveseven" ) },
			{ valve::e_item_index::glock, xorstr_( "glock" ) },
			{ valve::e_item_index::m4a1_silencer, xorstr_( "m4a1_silencer" ) },
			{ valve::e_item_index::revolver, xorstr_( "revolver" ) },
			{ valve::e_item_index::usp_silencer, xorstr_( "usp_silencer" ) }
		};

		std::array< weapon_t, k_max_knives >	m_knives {
			weapon_t{ valve::e_item_index::none, xorstr_( "default" ) },
			{ valve::e_item_index::knife_bayonet, xorstr_( "bayonet" ) },
			{ valve::e_item_index::knife_css, xorstr_( "classic" ) },
			{ valve::e_item_index::knife_skeleton, xorstr_( "skeleton" ) },
			{ valve::e_item_index::knife_outdoor, xorstr_( "nomad" ) },
			{ valve::e_item_index::knife_cord, xorstr_( "paracord" ) },
			{ valve::e_item_index::knife_canis, xorstr_( "survival" ) },
			{ valve::e_item_index::knife_flip, xorstr_( "flip" ) },
			{ valve::e_item_index::knife_gut, xorstr_( "gut" ) },
			{ valve::e_item_index::knife_karambit, xorstr_( "karambit" ) },
			{ valve::e_item_index::knife_m9_bayonet, xorstr_( "m9 bayonet" ) },
			{ valve::e_item_index::knife_tactical, xorstr_( "huntsman" ) },
			{ valve::e_item_index::knife_falchion, xorstr_( "falchion" ) },
			{ valve::e_item_index::knife_survival_bowie, xorstr_( "bowie" ) },
			{ valve::e_item_index::knife_butterfly, xorstr_( "butterfly" ) },
			{ valve::e_item_index::knife_push, xorstr_( "shadow daggers" ) },
			{ valve::e_item_index::knife_ursus, xorstr_( "ursus" ) },
			{ valve::e_item_index::knife_gypsy_jackknife, xorstr_( "navaja" ) },
			{ valve::e_item_index::knife_stiletto, xorstr_( "stiletto" ) },
			{ valve::e_item_index::knife_widowmaker, xorstr_( "talon" ) }
		};

		std::array< std::string, k_max_knives - 1u > m_knife_mdls {
			xorstr_( "models/weapons/v_knife_bayonet.mdl" ),
				xorstr_( "models/weapons/v_knife_css.mdl" ),
				xorstr_( "models/weapons/v_knife_skeleton.mdl" ),
				xorstr_( "models/weapons/v_knife_outdoor.mdl" ),
				xorstr_( "models/weapons/v_knife_cord.mdl" ),
				xorstr_( "models/weapons/v_knife_canis.mdl" ),
				xorstr_( "models/weapons/v_knife_flip.mdl" ),
				xorstr_( "models/weapons/v_knife_gut.mdl" ),
				xorstr_( "models/weapons/v_knife_karam.mdl" ),
				xorstr_( "models/weapons/v_knife_m9_bay.mdl" ),
				xorstr_( "models/weapons/v_knife_tactical.mdl" ),
				xorstr_( "models/weapons/v_knife_falchion_advanced.mdl" ),
				xorstr_( "models/weapons/v_knife_survival_bowie.mdl" ),
				xorstr_( "models/weapons/v_knife_butterfly.mdl" ),
				xorstr_( "models/weapons/v_knife_push.mdl" ),
				xorstr_( "models/weapons/v_knife_ursus.mdl" ),
				xorstr_( "models/weapons/v_knife_gypsy_jackknife.mdl" ),
				xorstr_( "models/weapons/v_knife_stiletto.mdl" ),
				xorstr_( "models/weapons/v_knife_widowmaker.mdl" ),
		};

		/*std::array< weapon_t, k_max_gloves>	m_gloves {
			weapon_t{ valve::e_item_index::none, xorstr_( "default" ) },
			{ valve::e_item_index::knife_bayonet, xorstr_( "bayonet" ) },
			{ valve::e_item_index::knife_css, xorstr_( "css" ) },
			{ valve::e_item_index::knife_skeleton, xorstr_( "skeleton" ) },
			{ valve::e_item_index::knife_outdoor, xorstr_( "outdoor" ) },
			{ valve::e_item_index::knife_cord, xorstr_( "cord" ) },
			{ valve::e_item_index::knife_canis, xorstr_( "canis" ) },
			{ valve::e_item_index::knife_flip, xorstr_( "flip" ) },
			{ valve::e_item_index::knife_gut, xorstr_( "gut" ) },
			{ valve::e_item_index::knife_karambit, xorstr_( "karambit" ) },
			{ valve::e_item_index::knife_m9_bayonet, xorstr_( "m9 bayonet" ) },
			{ valve::e_item_index::knife_tactical, xorstr_( "tactical" ) },
			{ valve::e_item_index::knife_falchion, xorstr_( "falchion" ) },
			{ valve::e_item_index::knife_survival_bowie, xorstr_( "bowie" ) },
			{ valve::e_item_index::knife_butterfly, xorstr_( "butterfly" ) },
			{ valve::e_item_index::knife_push, xorstr_( "push" ) },
			{ valve::e_item_index::knife_ursus, xorstr_( "ursus" ) },
			{ valve::e_item_index::knife_gypsy_jackknife, xorstr_( "gypsy" ) },
			{ valve::e_item_index::knife_stiletto, xorstr_( "stiletto" ) },
			{ valve::e_item_index::knife_widowmaker, xorstr_( "widowmaker" ) }
		};

		std::array< std::string, k_max_gloves - 1u > m_glove_mdls {
			xorstr_( "models/weapons/w_models/arms/w_glove_bloodhound.mdl" ),
				xorstr_( "models/weapons/w_models/arms/w_glove_fingerless.mdl" ),
				xorstr_( "models/weapons/w_models/arms/w_glove_hardknuckle.mdl" ),
				xorstr_( "models/weapons/w_models/arms/w_glove_sporty.mdl" ),
				xorstr_( "models/weapons/w_models/arms/w_glove_slick.mdl" ),
				xorstr_( "models/weapons/w_models/arms/w_glove_handwrap_leathery.mdl" ),
				xorstr_( "models/weapons/w_models/arms/w_glove_motorcycle.mdl" ),
				xorstr_( "models/weapons/w_models/arms/w_glove_specialist.mdl" ),
				xorstr_( "models/weapons/w_models/arms/w_glove_bloodhound_hydra.mdl" )
		};*/

		struct paint_kit_t {
			__forceinline constexpr paint_kit_t( ) = default;

			__forceinline  paint_kit_t( std::size_t id, std::string&& name )
				: m_id{ id }, m_name{ name } {}

			std::size_t m_id{};
			std::string m_name{};
		};

		struct cfg_t {
			std::size_t								m_knife_index{};
			std::array< std::size_t, k_max_wpns >	m_paint_kits{};
		};

		sdk::cfg_var_t< cfg_t >					m_cfg{ 0xbd113e91, {} };

		bool									m_update_hud{},
			m_in_force_update{};

		std::atomic< bool >						m_force_update{};
		float									m_last_update_time{};

		std::vector< paint_kit_t >				m_paint_kits{},
												m_raw_paint_kits{};

		std::size_t								m_old_knife_index{};
		std::array< std::size_t, k_max_wpns >	m_old_paint_kits{};
	public:
		void init( );

		int correct_sequence( const valve::e_item_index index, const int seq ) const;

		void on_frame_stage( const valve::e_frame_stage stage );

		__forceinline const std::array< weapon_t, k_max_wpns >& weapons( );

		__forceinline const std::array< weapon_t, k_max_knives >& knives( );

		__forceinline const std::vector< paint_kit_t >& paint_kits( );

		__forceinline const std::vector< paint_kit_t >& raw_paint_kits( );

		__forceinline std::atomic< bool >& force_update( );

		__forceinline cfg_t& cfg( );
	};

	inline const auto g_skins = std::make_unique< c_skins >( );
}

#include "skins.inl"