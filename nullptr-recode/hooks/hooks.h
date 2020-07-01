#pragma once
#include "vfunc.h"
#include "../utils/utils.h"
#include "../settings/settings.h"
#include "../gui/nullptr_gui.h"
#include <intrin.h>
#include "../sdk/sdk.h"
#include "../functions/misc/misc.h"
#include "../functions/misc/engine_prediction.h"

#pragma intrinsic(_ReturnAddress)  

namespace hooks {
	extern vfunc_hook d3d9device_vhook;
	extern vfunc_hook panel_vhook;
	extern vfunc_hook surface_vhook;
	extern vfunc_hook client_hook;
	extern vfunc_hook net_channel_vhook;
	extern vfunc_hook client_vhook;
	extern vfunc_hook client_mode_vhook;
	extern vfunc_hook sound_vhook;
	extern vfunc_hook engine_vhook;
	extern vfunc_hook game_coordinator_vhook;

	namespace indexes {
		int send_net_msg = 40;
		int frame_stage_notify = 37;
		int send_datagram = 46;
		int end_scene = 42;
		int reset = 16;
		int paint_traverse = 41;
		int create_move = 22;
		int lock_cursor = 67;
		int override_view = 18;
		int emit_sound = 5;
		int is_playing_demo = 82;
		int retrieve_message = 2;
		int draw_set_color = 15;
	}

	void initialize();
	void unhook();

	namespace end_scene {
		using fn = long(__stdcall*)(IDirect3DDevice9*);
		long __stdcall hook(IDirect3DDevice9* pDevice);
	}

	namespace reset {
		using fn = long(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
		long __stdcall hook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters);
	}

	namespace paint_traverse {
		using fn = void(__thiscall*)(c_panel*, vgui::VPANEL, bool, bool);
		void __stdcall hook(vgui::VPANEL panel, bool forceRepaint, bool allowForce);
	}

	namespace create_move {
		using fn = void(__fastcall*)(void*, int, int, float, bool);
		void __stdcall hook(int sequence_number, float input_sample_frametime, bool active, bool& send_packet);
	}

	namespace lock_cursor {
		using fn = void(__thiscall*)(c_surface*);
		void __stdcall hook();
	}

	namespace send_datagram {
		using fn = int(__thiscall*)(void*, void*);
		int __fastcall hook(void* net_channel, void*, void* datagram);
	}

	namespace send_net_msg {
		using fn = bool(__thiscall*)(c_net_channel * pNetChan, c_net_message & msg, bool bForceReliable, bool bVoice);
		bool __fastcall hook(c_net_channel* pNetChan, void* edx, c_net_message& msg, bool bForceReliable, bool bVoice);
	}

	namespace frame_stage_notify {
		using fn = void(__thiscall*)(c_base_client_dll*, frame_stage_t);
		void __stdcall hook(frame_stage_t stage);
	}

	namespace override_view {
		using fn = void(__thiscall*)(c_client_mode*, c_view_setup*);
		void __stdcall hook(c_view_setup* view);
	}

	namespace emit_sound {
		using fn = void(__thiscall*)(void*, c_recipient_filter&, int, int, const char*, unsigned int, const char*, float, int, float, int, int, const vec3*, const vec3*, void*, bool, float, int, int);
		void __stdcall hook(c_recipient_filter& filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char* pSample, float flVolume, int nSeed, float flAttenuation, int iFlags, int iPitch, const vec3* pOrigin, const vec3* pDirection, void* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, int unk);
	}

	namespace is_playing_demo {
		using fn = bool(__stdcall*)();
		bool __stdcall hook();
	}

	namespace retrieve_message {
		using fn = game_coordinator_results(__thiscall*)(void*, uint32_t*, void*, uint32_t, uint32_t*);
		game_coordinator_results __stdcall hook(uint32_t* punMsgType, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize);
	}

	namespace draw_set_color {
		using fn = void(__thiscall*)(c_surface*, int, int, int, int);
		void __stdcall hook(int r, int g, int b, int a);
	}

	namespace wndproc {
		typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
		WNDPROC o_wnd_proc;
		LRESULT __stdcall hook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	}
}