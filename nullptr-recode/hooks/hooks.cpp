#include "hooks.h"

__declspec(naked) void __fastcall create_move_hook_proxy(void* _this, int, int sequence_number, float input_sample_frametime, bool active)
{
	__asm
	{
		push ebp
		mov  ebp, esp
		push ebx; not sure if we need this
		push esp
		push dword ptr[active]
		push dword ptr[input_sample_frametime]
		push dword ptr[sequence_number]
		call hooks::create_move::hook
		pop  ebx
		pop  ebp
		retn 0Ch
	}
}

namespace hooks {

	vfunc_hook d3d9device_vhook;
	vfunc_hook panel_vhook;
	vfunc_hook surface_vhook;
	vfunc_hook client_vhook;
	vfunc_hook net_channel_vhook;
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

		client_vhook.setup(sdk::chl_client); 
		client_vhook.hook_index(indexes::create_move,        create_move_hook_proxy);
		client_vhook.hook_index(indexes::frame_stage_notify, frame_stage_notify::hook);

		client_mode_vhook.setup(sdk::client_mode);
		client_mode_vhook.hook_index(indexes::override_view, override_view::hook);
	}

	void unhook() {
		d3d9device_vhook.unhook_all();
		panel_vhook.unhook_all();
		surface_vhook.unhook_all();
		client_vhook.unhook_all();
		net_channel_vhook.unhook_all();
		client_mode_vhook.unhook_all();

		SetWindowLongPtr(sdk::game_hwnd, GWL_WNDPROC, (LONG_PTR)wndproc::o_wnd_proc);
	}
}