#pragma once
#include <map>
#include <string>
#include <vector>
#include "../../../sdk/structures/structures.h"

static auto is_knife(const int i) -> bool {
	return (i >= item_definition_index::knife_bayonet && i < item_definition_index::glove_studded_bloodhound) || i == item_definition_index::knife_t || i == item_definition_index::knife;
}

struct weapon_info {
	constexpr weapon_info(const char* model, const char* icon = nullptr) :
		model(model),
		icon(icon)
	{}

	const char* model;
	const char* icon;
};

class weapon_name {
public:
	weapon_name(int _definition_index, const char* _name) {
		definition_index = _definition_index;
		name = _name;
	}

	int definition_index;
	std::string name;

	std::string _weapon_name;
	std::string skin_name;
};
