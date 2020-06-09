#pragma once
#include "../nullptr_gui.h"
#include "../../settings/globals.h"
#include "../../sdk/sdk.h"
#include "../../hooks/hooks.h"
#include "../../functions/notify/notify.h"

namespace menu {
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