#pragma once
#include "../../settings/settings.h"
#include "../../sdk/structures/structures.h"
#include "../backtrack/backtrack.h"

namespace legitbot {
	legitbot_settings_t* get_settings();

	void run(c_user_cmd* cmd);
	bool is_enabled(c_user_cmd* cmd);
	float get_fov_to_player(qangle view_angle, qangle aim_angle);

	float get_fov();
	bool hit_chance(qangle angles, c_base_player* player, float chance);
	void rcs(qangle& angle, c_base_player* target);
	bool is_line_goes_through_smoke(vec3 start_pos, vec3 end_pos);
	void smooth(qangle current_angle, qangle aim_angle, qangle& angle);
	bool is_silent();
	c_base_player* get_closest_player(c_user_cmd* cmd, int& best_bone, float& fov, qangle& angles);

	legitbot_settings_t settings;
	c_base_player* target = nullptr;
	int last_shot_tick;
	int shots_fired;
	bool shot_delay = false;
	int shot_delay_time = 0;
	bool kill_delay = false;
	int kill_delay_time = 0;
	qangle current_punch = { 0, 0, 0 };
	qangle last_punch = { 0, 0, 0 };
}