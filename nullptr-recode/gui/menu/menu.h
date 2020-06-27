#pragma once
#include "../../functions/changers/skin changer/item_definitions.h"
#include "../../functions/changers/skin changer/kit_parser.h"
#include "../../functions/changers/skin changer/skins.h"
#include "../nullptr_gui.h"
#include "../../settings/settings.h"
#include "../../sdk/sdk.h"
#include "../../functions/notify/notify.h"
#include "../../sdk/structures/structures.h"
#include <experimental/filesystem>

namespace menu {
	int definition_vector_index = 0;
	std::string temp_clantag = "";
	int global_visuals_tab = 0;

	void draw_bind_window();
	void draw_main_window();

	void legit_tab();
	void visuals_tab();
	void misc_tab();
	void skins_tab();
	void profile_tab();
	void configs_tab();
	void scripts_tab();
}