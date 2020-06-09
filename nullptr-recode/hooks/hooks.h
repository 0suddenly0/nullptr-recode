#pragma once
#include "vfunc.h"
#include "../sdk/sdk.h"
#include "../utils/utils.h"
#include "../settings/globals.h"
#include "../gui/nullptr_gui.h"

namespace hooks {
	extern vfunc_hook d3d9device_vhook;

	enum indexes {
		end_scene_idx = 42,
		present_idx = 17
	};

	void initialize();
	void unhook();

	namespace end_scene {
		using fn = long(__stdcall*)(IDirect3DDevice9*);
		long __stdcall hook(IDirect3DDevice9* pDevice);
	}

	namespace wndproc {
		typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
		WNDPROC oWndProc;
		LRESULT __stdcall hook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	}
}