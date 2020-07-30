#pragma once
#include "../functions/notify/notify.h"
#include "../functions/changers/skin changer/skins.h"
#include "globals.h"
#include "../helpers/input.h"
#include "../helpers/color.h"
#include "../helpers/math/vector.h"

extern std::map<int, std::string> mode_names;

std::vector<std::string> weapon_groups = {
	"knife",
	"zeus",
	"pistols",
	"semi rifle",
	"rifle", 
	"shotgun", 
	"sniper rifle", 
	"machine gun" 
};

std::vector<std::string> hitboxes_string = {
	"head",
	"neck",
	"upper chest",
	"chest",
	"lower chest",
	"stomach",
	"pelvis",
	"arms",
	"legs",
	"feet" 
};

struct statrack_setting_t {
	int definition_index = 1;
	struct
	{
		int counter = 0;
	}statrack_new;
};

struct profile_settings_t {
	int wins = 0;
	int rank_id = 0;
};

enum esp_types : int {
	enemies = 0,
	teammates = 1,
	local_player = 2
};

enum chams_layer : int {
	lower = 0,
	upper = 1
};

enum weapon_chams_types : int {
	in_hands = 0,
	dropped = 1
};


struct chams_layer_settings_t {
	bool enable = false;
	bool only_visible = false;

	int type = 0;

	color visible = color(255, 255, 255, 255);
	color invisible = color(0, 0, 0, 255);
};

struct chams_settings_t {
	std::array< chams_layer_settings_t, 2 > layers;
};

struct glow_settings_t {
	bool enable = false;
	bool visible_only = false;

	color visible = { 0, 0, 0, 255 };
	color invisible = { 0, 0, 0, 255 };

	int type = 0;
};

struct window_settings_t {
	std::string name;
	int alpha;
	vec2 pos;
	bool show;
};

struct legitbot_settings_t {
	bool enabled = false;
	bool silent = false;
	bool autopistol = false;

	float fov = 0.f;
	float silent_fov = 0.f;
	int silent_chance = 100;
	float smooth = 1.f;

	std::array<bool, 10> hitboxes = { true, true, true, true, true, true, true, true, true, true };

	bool team_check = true;
	bool flash_check = true;
	bool smoke_check = true;

	int shot_delay = 0;
	int kill_delay = 0;

	int backtrack_ticks = 0;
	bool aim_at_backtrack = false;
	int backtrack_priority = 0;

	bool autowall = false;
	int autowall_min_damage = 0;

	bool autofire = false;
	key_bind_t autofire_bind = { 0, 0, false };
	int autofire_chance = 100;

	bool rcs_enabled = false;
	int rcs_start = 1;
	int rcs_type = 0;
	int rcs_x = 100;
	int rcs_y = 100;
};

char* player_tabs[] = {
	"enemies", 
	"teammates", 
	"local player"
};

char* layer_tabs[] = {
	"bottom",
	"upper"
};

char* ragdoll_tabs[] = {
	"enemies",
	"teammates",
};

char* weapon_tabs[] = {
	"in hands",
	"dropped",
};

char* legitbot_groups[] = {
	"all",
	"groups",
	"on weapon"
};

struct esp_settings_t {
	bool enable = false;
	bool only_visible = false;

	bool skeleton = false;
	bool box = false;
	bool name = false;

	bool health_bar = false;
	bool health_in_bar = false;
	bool health_based = false;

	bool armor_bar = false;
	bool armor_in_bar = false;

	bool weapon = false;
	bool ammo_in_bar = false;
	bool weapon_ammo = false;

	bool flags_scoped = false;
	bool flags_flashed = false;
	bool flags_defusing = false;
	bool flags_planting = false;
	bool flags_bomb_carrier = false;
	bool flags_reloading = false;
	bool flags_armor = false;
	bool flags_helmet = false;

	color dormant = color(0, 0, 0, 255);

	color box_visible = color(0, 0, 0, 255);
	color box_invisible = color(0, 0, 0, 255);

	color skeleton_visible = color(0, 0, 0, 255);
	color skeleton_invisible = color(0, 0, 0, 255);

	color health_bar_outline = color(0, 0, 0, 255);
	color health_bar_background = color(0, 0, 0, 255);
	color health_bar_main = color(0, 0, 0, 255);

	color armor_bar_outline = color(0, 0, 0, 255);
	color armor_bar_background = color(0, 0, 0, 255);
	color armor_bar_main = color(0, 0, 0, 255);

	color ammo_bar_outline = color(0, 0, 0, 255);
	color ammo_bar_background = color(0, 0, 0, 255);
	color ammo_bar_main = color(0, 0, 0, 255);
};

struct weapon_name_t {
	constexpr weapon_name_t(int32_t definition_index, const char* name) :
		definition_index(definition_index),
		name(name) {
	}

	int32_t definition_index = 0;
	const char* name = nullptr;
};

std::vector< weapon_name_t > weapon_names = {
{ knife, "knife" },
{ taser, "zeus" },

{ usp_silencer, "usp-s" },
{ glock, "glock-18" },
{ hkp2000, "p2000" },
{ elite,  "dual berettas" },
{ p250, "p250" },
{ cz75a, "cz75 auto" },
{ fiveseven, "five-seven" },
{ tec9, "tec-9" },
{ revolver, "r8 revolver" },
{ deagle,  "desert eagle" },

{ mp9, "mp9" },
{ mac10, "mac-10" },
{ mp7, "mp7" },
{ mp5, "mp5-sd" },
{ ump45, "ump-45" },
{ p90, "p90" },
{ bizon, "pp-bizon" },

{ galilar, "galil ar" },
{ famas, "famas" },
{ ak47,  "ak-47" },
{ m4a1_silencer, "m4a1-s" },
{ m4a1, "m4a1" },
{ aug, "aug" },
{ ssg08, "ssg 08" },
{ sg556, "sg 553" },
{ awp, "awp" },
{ scar20, "scar-20" },
{ g3sg1, "g3sg1" },

{ nova, "nova" },
{ xm1014, "xm1014" },
{ sawedoff, "sawed-off" },
{ mag7, "mag-7" },
{ m249, "m249" },
{ negev, "negev" },
};

namespace settings {
	namespace legitbot {
		int settings_type = 0;
		int group_id = 0;
		int weapon_id = 0;
		int weapon_id_in_list = 0;
		bool auto_current = false;

		legitbot_settings_t legitbot_items_all;
		extern std::map<int, legitbot_settings_t> legitbot_items;
		extern std::map<int, legitbot_settings_t> legitbot_items_groups;

		key_bind_t bind = { 0, 0, false };
		bool using_bind = false;
	}
	namespace visuals {
		bool night_mode = false;
		bool ingame_radar = false;
		bool sniper_crosshair = false;
		bool bomb_timer = false;
		int bomb_timer_pos = 0;

		namespace esp {
			bool using_bind = false;
			key_bind_t bind = { 0, 0, false };
			std::array<esp_settings_t, 3> esp_items;
		}

		namespace glow {
			bool using_bind = false;
			key_bind_t bind = { 0, 0, false };
			std::array<glow_settings_t, 3> glow_items;
		}

		namespace chams {
			int backtrack_tick_draw = 0;

			bool using_bind = false;
			key_bind_t bind = { 0, 0, false };
			std::array<chams_settings_t, 3> player_items;

			std::array<chams_settings_t, 2> ragdoll_items;
			std::array<chams_settings_t, 2> weapon_items;
			chams_settings_t hands;
			chams_settings_t backtrack;
		}

		namespace logs {
			bool enable = true;
			log_settings_t hurt       = log_settings_t{ true,  true,  false, false, color(255,255,255,255), color(255, 100, 100, 255), log_type::standart };
			log_settings_t player_buy = log_settings_t{ false, false, false, false, color(255,255,255,255), color(100, 100, 255, 255), log_type::standart };
			log_settings_t planting   = log_settings_t{ true,  true,  false, false, color(255,255,255,255), color(100, 100, 255, 255), log_type::standart };
			log_settings_t defusing   = log_settings_t{ true,  true,  false, false, color(255,255,255,255), color(100, 100, 255, 255), log_type::standart };
			log_settings_t config     = log_settings_t{ true,  true,  true,  false, color(255,255,255,255), globals::menu_color,       log_type::big };
			log_settings_t lua        = log_settings_t{ true,  true,  true,  false, color(255,255,255,255), globals::menu_color,       log_type::big };
		}

		namespace impacts{
			namespace client {
				bool enable = false;
				color clr = color(255, 0, 0, 127);
				int size = 2;
				int show_time = 4;
			}

			namespace server {
				bool enable = false;
				color clr = color(0, 0, 255, 127);
				int size = 2;
				int show_time = 4;
			}
		}

		namespace spread_circle {
			bool enable = false;
			color clr = color(255, 255, 255, 20);
		}

		namespace aimbot_fov {
			bool standart_outline = false;
			bool standart_filled = false;
			color standart_outline_clr = color(255, 255, 255, 255);
			color standart_filled_clr = color(255, 255, 255, 10);

			bool silent_outline = false;
			bool silent_filled = false;
			color silent_outline_clr = color(255, 50, 50, 255);
			color silent_filled_clr = color(255, 50, 50, 10);
		}

		namespace hitbox {
			bool enable;
			bool show_only_kill;
			color clr;
			float show_time;
		}

		namespace damage_indicator {
			bool enable = false;

			float show_time = 5.f;
			float speed = 0.2f;
			float max_pos_y = 20.f;
			float range_offset_x = 10.f;
			float offset_if_kill = 10.f;
			float offset_hit = 0.f;

			color kill_color = color(255, 50, 50, 255);
		}

		namespace hitmarker {
			bool enable = true;
			float size = 3.f;
			float show_time = 2.f;
			int type = 1;
		}

		namespace hitsound {
			bool enable = true;
			float volume = 1.f;
		}

		namespace dropped_weapon {
			bool enable = true;
			bool box = true;
			bool ammo_bar = true;
			bool ammo_in_bar = true;

			color box_color = color(255, 255, 255, 255);

			color bar_outline = color(0, 0, 0, 255);
			color bar_background = color(10, 10, 10, 20);
			color bar_main = color(100, 100, 255, 255);
		}

		namespace ofc {
			bool enable = false;
			bool visible_check = false;

			float range = 26.f;
			float size = 10.f;

			int type = 0;

			color clr = color(255, 255, 255, 255);
		}

		namespace grenades {
			bool enable = false;

			bool smoke_timer = false;
			bool smoke_bar = true;
			bool smoke_radius = true;

			bool molotov_timer = false;
			bool molotov_bar = true;
			bool molotov_radius = false;

			color color_smoke = color(255, 255, 255, 255);
			color color_molotov = color(255, 255, 255, 255);
			color color_decoy = color(255, 255, 255, 255);
			color color_flash = color(255, 255, 255, 255);
			color color_tactical = color(255, 255, 255, 255);
			color color_frag = color(255, 255, 255, 255);

			color color_bar_smoke_back = color(20, 20, 20, 100);
			color color_bar_smoke_main = color(100, 100, 255, 255);
			color color_smoke_radius = color(100, 100, 255, 255);

			color color_bar_molotov_back = color(20, 20, 20, 100);
			color color_bar_molotov_main = color(255, 100, 100, 255);
			color color_molotov_radius = color(255, 100, 100, 255);
		}

		namespace props {
			bool enable = false;
			int alpha = 255;
		}

		namespace fog {
			bool enable = false;
			color clr = color(50, 50, 50, 120);
			float start_dist = 20.f;
			float end_dist = 100.f;
		}

		namespace grenade_prediction {
			bool enable = false;
			bool radius = true;

			float line_thickness = 2.f;
			float colision_box_size = 2.f;
			float main_colision_box_size = 3.f;

			color radius_color = color(255, 255, 255, 255);
			color main = color(255, 255, 255, 255);
			color main_box = color(100, 100, 255, 255);
			color end_box = color(255, 100, 100, 255);
		}
	}

	namespace misc {
		bool obs_bypass = false;
		bool moon_walk = false;
		bool auto_accept = false;
		bool reveal_money = false;
		bool reveal_rank = false;
		bool fast_duck = false;
		bool watermark = true;
		bool disable_flash = false;
		int flash_alpha = 50;
		bool disable_smoke = false;
		bool disable_zoom = false;
		bool disable_zoom_border = false;
		bool unlock_inventory = false;
		bool chat_filter_bypass = true;

		namespace bhop {
			bool enable = false;
			int strafe_type = 0;
		}

		namespace fake_latency {
			bool enable = false;
			int amount = 0;
		}

		namespace prepare_revolver {
			bool enable = false;
			key_bind_t bind = { 0, 0, false };
		}

		namespace third_person {
			bool enable = false;
			key_bind_t bind = { 0, 0, false };
			int dist = 50;
		}

		namespace viewmodel {
			float override = 90;
			float viewmodel = 68;
			float fov_x = 0;
			float fov_y = 0;
			float fov_z = 0;
			float roll = 0;
			bool save_roll_side = 0.f;
		}

		namespace inverse_gravity {
			bool enable_slow = true;
			bool enable = false;
			int value = 4;
		}

		namespace clantag {
			bool enable = false;
			bool check_empty = true;

			std::string clantag = "nullptr ";
			std::string clantag_visible = "nullptr ";

			int clantag_type = 0;
			int animation_type = 0;
			int custom_type = 0;
			float speed = 0.4f;
		}

		namespace edge_jump {
			bool enable = false;
			bool auto_duck = false;
			key_bind_t bind = { 0, 0, false };
		}

		namespace block_bot {
			bool enable = false;
			int max_dist = 200;
			key_bind_t bind = { 0, 0, false };
		}
	}

	namespace changers {
		namespace profile {
			extern std::map<int, profile_settings_t> profile_items;

			int mode_id;
			int private_id = 0;
			int private_xp = 0;
			int teacher = 0;
			int leader = 0;
			int friendly = 0;
			int ban_type = 0;
			int ban_time = 0;
			int ban_time_type = 0;
		}

		namespace skin {
			bool skin_preview = false;
			bool show_cur = true;

			extern std::map<int, statrack_setting_t> statrack_items;
			extern std::map<int, item_setting_t> m_items;
			extern std::map<std::string, std::string> m_icon_overrides;
		}
	}

	namespace windows {
		window_settings_t bind_window = { "binds", 255, vec2(100, 100), false };
		window_settings_t spectator_list = { "spectator list", 255, vec2(100, 100), false };
	}
}