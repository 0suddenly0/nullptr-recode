#pragma once
#include "item_definitions.h"
#include "../../../sdk/sdk.h"
#include "../../../sdk/structures/structures.h"

struct item_setting_t {
	char name[32] = "default";
	bool enabled = false;
	int stickers_place = 0;
	int definition_vector_index = 0;
	int definition_index = 0;
	bool   enabled_stickers = 0;
	int paint_kit_vector_index = 0;
	int paint_kit_index = 0;
	int definition_override_vector_index = 0;
	int definition_override_index = 0;
	int seed = 0;
	bool stat_trak = 0;
	float wear = FLT_MIN;
	char custom_name[32] = "";
};

static const char* get_cs_weapon_name(const short id) {
	switch (id) {
	case item_definition_index::deagle: return "deagle";
	case item_definition_index::elite: return "elite";
	case item_definition_index::fiveseven: return "fiveseven";
	case item_definition_index::glock: return "glock";
	case item_definition_index::ak47: return "ak47";
	case item_definition_index::aug: return "aug";
	case item_definition_index::awp: return "awp";
	case item_definition_index::famas: return "famas";
	case item_definition_index::g3sg1: return "g3sg1";
	case item_definition_index::galilar: return "galilar";
	case item_definition_index::m249: return "m249";
	case item_definition_index::m4a1_silencer: return "m4a1_silencer";
	case item_definition_index::m4a1: return "m4a1";
	case item_definition_index::mac10: return "mac10";
	case item_definition_index::p90: return "p90";
	case item_definition_index::ump45: return "ump45";
	case item_definition_index::xm1014: return "xm1014";
	case item_definition_index::bizon: return "bizon";
	case item_definition_index::mag7: return "mag7";
	case item_definition_index::negev: return "negev";
	case item_definition_index::sawedoff: return "sawedoff";
	case item_definition_index::tec9: return "tec9";
	case item_definition_index::hkp2000: return "hkp2000";
	case item_definition_index::mp5: return "mp5sd";
	case item_definition_index::mp7: return "mp7";
	case item_definition_index::mp9: return "mp9";
	case item_definition_index::nova: return "nova";
	case item_definition_index::p250: return "p250";
	case item_definition_index::scar20: return "scar20";
	case item_definition_index::sg556: return "sg556";
	case item_definition_index::ssg08: return "ssg08";
	case item_definition_index::usp_silencer: return "usp_silencer";
	case item_definition_index::cz75a: return "cz75a";
	case item_definition_index::revolver: return "revolver";
	case item_definition_index::knife: return "knife";
	case item_definition_index::knife_t: return "knife_t";
	case item_definition_index::knife_m9_bayonet: return "knife_m9_bayonet";
	case item_definition_index::knife_bayonet: return "bayonet";
	case item_definition_index::knife_css: return "knife_css";
	case item_definition_index::knife_cord: return "knife_cord";
	case item_definition_index::knife_canis: return "knife_canis";
	case item_definition_index::knife_outdoor: return "knife_outdoor";
	case item_definition_index::knife_skeleton: return "knife_skeleton";
	case item_definition_index::knife_flip: return "knife_flip";
	case item_definition_index::knife_gut:return "knife_gut";
	case item_definition_index::knife_karambit: return "knife_karambit";
	case item_definition_index::knife_tactical: return "knife_tactical";
	case item_definition_index::knife_falchion: return "knife_falchion";
	case item_definition_index::knife_survival_bowie: return "knife_survival_bowie";
	case item_definition_index::knife_butterfly: return "knife_butterfly";
	case item_definition_index::knife_push: return "knife_push";
	case item_definition_index::knife_ursus: return "knife_ursus";
	case item_definition_index::knife_gypsy_jack: return "knife_gypsy_jackknife";
	case item_definition_index::knife_stiletto: return "knife_stiletto";
	case item_definition_index::knife_widowmaker: return "knife_widowmaker";
	case item_definition_index::glove_studded_bloodhound: return "studded_bloodhound_gloves";
	case item_definition_index::glove_t_side:return "t_gloves";
	case item_definition_index::glove_ct_side: return "ct_gloves";
	case item_definition_index::glove_sporty: return "sporty_gloves";
	case item_definition_index::glove_slick: return "slick_gloves";
	case item_definition_index::glove_leather_wrap: return "leather_handwraps";
	case item_definition_index::glove_motorcycle: return "motorcycle_gloves";
	case item_definition_index::glove_specialist: return "specialist_gloves";
	case item_definition_index::glove_hydra: return "studded_hydra_gloves";
	default: return "";
	}
}

namespace skin_changer {
	namespace deeps {
		extern const std::map<size_t, weapon_info> weapon_models_info;
		extern std::vector<weapon_name> t_knife_names;
		extern std::vector<weapon_name> t_glove_names;
		extern std::vector<weapon_name> ct_knife_names;
		extern std::vector<weapon_name> ct_glove_names;
		extern std::vector<weapon_name> weapon_names;

		extern const std::vector<weapon_name> knife_names_preview;
		extern const std::vector<weapon_name> glove_names_preview;
		extern const std::vector<weapon_name> weapon_names_preview;

		create_client_class_fn get_wearable_create_fn();
		void erase_override_if_exists_by_index(const int definition_index);
		void apply_config_on_attributable_item(c_base_attributable_item* item, const item_setting_t* config, const unsigned xuid_low);

		IDirect3DTexture9* get_preview_image();
	}

	void update();
	color get_color_ratiry(int rar);
	const char* get_icon_override(const std::string original);
	void on_frame_stage_notify(frame_stage_t stage);
};