#pragma once
#include "../sdk/structures/structures.h"

struct bullet_impact_t {
	vec3 pos;
	int time_del;
};

struct grenade_info_t {
	vec3 position;
	float time_to_expire;
};

struct damage_indicator_t {
	bool init;
	vec3 hit_pos;
	vec3 end_pos;
	int damage;
	float time_del;
	bool killed;
};

namespace globals {
	std::vector<grenade_info_t> molotov_info;
	std::vector<grenade_info_t> smoke_info;
	std::vector<damage_indicator_t> damage_indicators;
	std::vector<bullet_impact_t> hitmarkers;
	bullet_impact_t hitmarker_last;

	color menu_color = color(43, 90, 255, 255);
	bool unloading = false;
	bool show_menu = false;
	bool profile_update_request = false;
}