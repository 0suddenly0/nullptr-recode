#pragma once
#include "../functions/notify/notify.h"
#include "../functions/changers/skin changer/skins.h"
#include "globals.h"
#include "../helpers/input.h"
#include "../helpers/color.h"
#include "../helpers/math/vector.h"

extern std::map<int, std::string> mode_names;

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

namespace settings {
	namespace visuals {
		bool night_mode = false;
		bool ingame_radar = false;
		bool sniper_crosshair = false;
		bool bomb_timer = false;
		int bomb_timer_pos = 0;

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

		namespace props {
			bool enable = false;
			int alpha = 255;
			bool request = false;
		}

		namespace grenade_prediction
		{
			bool enable = false;
			bool radius = true;

			float line_thickness = 2.f;
			float colision_box_size = 2.f;
			float main_colision_box_size = 3.f;

			color radius_color = color(0, 0, 0, 255);
			color main = color(0, 0, 0, 255);
			color main_box = color(0, 0, 0, 255);
			color end_box = color(0, 0, 0, 255);
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

		namespace fog {
			bool enable = false;
			color clr = color(50, 50, 50, 120);
			float start_dist = 20.f;
			float end_dist = 100.f;
		}

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
		bool bind_window_show = false;
		int bind_window_alpha = 255;
		vec2 bind_window_pos = vec2(100, 100);

		bool spectator_list_show = false;
		int spectator_list_alpha = 255;
		vec2 spectator_list_pos = vec2(300, 100);
	}
}