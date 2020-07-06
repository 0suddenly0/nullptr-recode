#pragma once
#include "../gui/menu/menu.h"
#include "../sdk/structures/structures.h"

struct bullet_impact_t {
	vec3 pos;
	int time_life;
	int time_del;
	color clr;
	int size;
};

struct grenade_info_t {
	vec3 position;
	float time_to_expire;
};

namespace globals {
	std::vector<grenade_info_t> molotov_info;
	std::vector<grenade_info_t> smoke_info;

	color menu_color = color(43, 90, 255, 255);
	std::vector<bullet_impact_t> server_impacts;
	bool unloading = false;
	bool show_menu = false;
	bool profile_update_request = false;
}