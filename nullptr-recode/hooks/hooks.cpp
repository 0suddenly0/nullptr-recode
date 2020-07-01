#include "hooks.h"
#include "../functions/changers/profile changer/profile_changer.h"
#include "../functions/misc/events.h"

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
	vfunc_hook sound_vhook;
	vfunc_hook engine_vhook;
	vfunc_hook game_coordinator_vhook; 

	void initialize() {
		wndproc::o_wnd_proc = (wndproc::WNDPROC)SetWindowLongPtr(sdk::game_hwnd, GWL_WNDPROC, (LONG_PTR)wndproc::hook);

		d3d9device_vhook.setup(sdk::d3d9device);
		d3d9device_vhook.hook_index(indexes::end_scene, end_scene::hook);
		d3d9device_vhook.hook_index(indexes::reset, reset::hook);

		panel_vhook.setup(sdk::vgui_panel);
		panel_vhook.hook_index(indexes::paint_traverse, paint_traverse::hook);

		surface_vhook.setup(sdk::vgui_surface);
		surface_vhook.hook_index(indexes::lock_cursor, lock_cursor::hook);

		client_vhook.setup(sdk::chl_client);
		client_vhook.hook_index(indexes::create_move, create_move_hook_proxy);
		client_vhook.hook_index(indexes::frame_stage_notify, frame_stage_notify::hook);

		client_mode_vhook.setup(sdk::client_mode);
		client_mode_vhook.hook_index(indexes::override_view, override_view::hook);

		sound_vhook.setup(sdk::engine_sound);
		sound_vhook.hook_index(indexes::emit_sound, emit_sound::hook);

		engine_vhook.setup(sdk::engine_client);
		engine_vhook.hook_index(indexes::is_playing_demo, is_playing_demo::hook);

		game_coordinator_vhook.setup(sdk::game_coordinator);
		game_coordinator_vhook.hook_index(indexes::retrieve_message, retrieve_message::hook);

		event_manager::initialization();
	}

	void unhook() {
		net_channel_vhook.unhook_all();
		d3d9device_vhook.unhook_all();
		panel_vhook.unhook_all();
		surface_vhook.unhook_all();
		client_vhook.unhook_all();
		client_mode_vhook.unhook_all();
		sound_vhook.unhook_all();
		engine_vhook.unhook_all();
		game_coordinator_vhook.unhook_all();

		event_manager::shutdown();
		profile_changer::send_update_messages();

		SetWindowLongPtr(sdk::game_hwnd, GWL_WNDPROC, (LONG_PTR)wndproc::o_wnd_proc);
	}
}
