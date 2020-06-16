#pragma once
#include "../helpers/input.h"
#include "../helpers/color.h"

namespace settings {
	namespace visuals {
		bool ingame_radar = false;
	}

	namespace misc {
		bool bind_window = false;

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
		bool sniper_crosshair = false;

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
}