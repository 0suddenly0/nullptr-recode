#include "../hooks.h"

namespace hooks {
	void __stdcall paint_traverse::hook(vgui::VPANEL panel, bool forceRepaint, bool allowForce) {
		static auto panelId = vgui::VPANEL{ 0 };
		static auto o_paint_traverse = panel_vhook.get_original<fn>(indexes::paint_traverse);

		o_paint_traverse(sdk::vgui_panel, panel, forceRepaint, allowForce);

		if (!panelId) {
			const auto panelName = sdk::vgui_panel->get_name(panel);
			if (!strcmp(panelName, "MatSystemTopPanel")) {
				panelId = panel;
			}
		} else if (panelId == panel){
			render::begin_render();
		}
	}
}