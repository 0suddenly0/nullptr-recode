#pragma once
#include <vector>
#include <map>
#include <xstring>

namespace skin_parser {
	struct paint_kit {
		int id;
		int rarity;
		std::string name;
		std::string name_short;
		std::vector<std::string> weaponName;
		auto operator < (const paint_kit& other) const -> bool
		{
			return name < other.name;
		}
	};
	extern bool skins_parsed;
	extern std::vector<paint_kit> skins;
	extern std::vector<paint_kit> gloves;
	extern std::vector<paint_kit> stickers;
	void initialize_kits();
}