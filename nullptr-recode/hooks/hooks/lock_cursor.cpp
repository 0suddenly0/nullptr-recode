#include "../hooks.h"

namespace hooks {
	void __stdcall lock_cursor::hook() {
		static auto o_lock_cursor = surface_vhook.get_original<fn>(indexes::lock_cursor);

		if (globals::show_menu) {
			sdk::vgui_surface->unlock_cursor();
			sdk::input_system->reset_input_state();

			return;
		}
		o_lock_cursor(sdk::vgui_surface);
	}
}