#pragma once
#include "vfunc.h"
#include "../utils/utils.h"
#include "../settings/globals.h"
#include "../gui/nullptr_gui.h"
#include <intrin.h>

#include "../functions/misc/misc.h"

#pragma intrinsic(_ReturnAddress)  

namespace hooks {
	extern vfunc_hook d3d9device_vhook;
	extern vfunc_hook panel_vhook;
	extern vfunc_hook surface_vhook;
	extern vfunc_hook client_mode_vhook;

	namespace indexes {
		int end_scene = 42;
		int reset = 16;
		int paint_traverse = 41;
		int create_move = 24;
		int lock_cursor = 67;
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
		using fn = bool(__thiscall*)(c_client_mode*, float, c_user_cmd*);
		bool __stdcall hook(float input_sample_frametime, c_user_cmd* cmd);
	}

	namespace lock_cursor {
		using fn = void(__thiscall*)(c_surface*);
		void __stdcall hook();
	}

	namespace wndproc {
		typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
		WNDPROC o_wnd_proc;
		LRESULT __stdcall hook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	}
}