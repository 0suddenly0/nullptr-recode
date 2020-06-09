#include "hooks.h"

namespace hooks {

	vfunc_hook d3d9device_vhook;

	void initialize() {
		wndproc::oWndProc = (wndproc::WNDPROC)SetWindowLongPtr(sdk::game_hwnd, GWL_WNDPROC, (LONG_PTR)wndproc::hook);

		d3d9device_vhook.setup(sdk::d3d9device);
		d3d9device_vhook.hook_index(indexes::end_scene_idx, end_scene::hook);
	}

	void unhook() {

		d3d9device_vhook.unhook_all();

		SetWindowLongPtr(sdk::game_hwnd, GWL_WNDPROC, (LONG_PTR)wndproc::oWndProc);
	}
}