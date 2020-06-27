#include "../hooks.h"

namespace hooks {
	void __stdcall paint_traverse::hook(vgui::VPANEL panel, bool forceRepaint, bool allowForce) {
		static auto panelId = vgui::VPANEL{ 0 };
		static auto o_paint_traverse = panel_vhook.get_original<fn>(indexes::paint_traverse);

		if (settings::misc::disable_zoom_border && !strcmp("HudZoom", sdk::vgui_panel->get_name(panel)) && sdk::local_player->is_alive()) 
			return;

		o_paint_traverse(sdk::vgui_panel, panel, forceRepaint, allowForce);

		if (!panelId) {
			const auto panelName = sdk::vgui_panel->get_name(panel);
			if (!strcmp(panelName, "MatSystemTopPanel")) {
				panelId = panel;
			}
		} else if (panelId == panel){
			if (sdk::local_player && IN_SCORE && settings::misc::reveal_rank) 
				sdk::chl_client->dispatch_user_message(50, 0, 0, nullptr);
			render::begin_render();
		}
	}
}