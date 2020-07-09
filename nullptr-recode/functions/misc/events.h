#pragma once
#include "../../sdk/sdk.h"

namespace event_manager {
	void initialization();
	void shutdown();

	extern std::map<int, std::string> hitbox_to_string;
	extern std::map<int, int> get_event_bone;
	extern std::map<std::string, std::string> fixed_weapon_names_by_name;
}