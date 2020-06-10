#include "hooks.h"

namespace hooks {

	vfunc_hook d3d9device_vhook;
	vfunc_hook panel_vhook;
	vfunc_hook surface_vhook;
	vfunc_hook client_mode_vhook;

	void initialize() {
		wndproc::o_wnd_proc = (wndproc::WNDPROC)SetWindowLongPtr(sdk::game_hwnd, GWL_WNDPROC, (LONG_PTR)wndproc::hook);

		d3d9device_vhook.setup(sdk::d3d9device);
		d3d9device_vhook.hook_index(indexes::end_scene, end_scene::hook);
		d3d9device_vhook.hook_index(indexes::reset,     reset::hook);

		panel_vhook.setup(sdk::vgui_panel);
		panel_vhook.hook_index(indexes::paint_traverse, paint_traverse::hook);

		surface_vhook.setup(sdk::vgui_surface);
		surface_vhook.hook_index(indexes::lock_cursor, lock_cursor::hook);

		client_mode_vhook.setup(sdk::client_mode);
		client_mode_vhook.hook_index(indexes::create_move, create_move::hook);
	}

	void unhook() {
		d3d9device_vhook.unhook_all();
		panel_vhook.unhook_all();
		surface_vhook.unhook_all();

		SetWindowLongPtr(sdk::game_hwnd, GWL_WNDPROC, (LONG_PTR)wndproc::o_wnd_proc);
	}
}