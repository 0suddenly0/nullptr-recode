#include "../hooks.h"
#include "../../gui/imgui/imgui_impl_win32.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace hooks {
	LRESULT __stdcall wndproc::hook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		input::process_message(uMsg, wParam, lParam);

		if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam) && globals::show_menu) return true;

		return CallWindowProc(o_wnd_proc, hWnd, uMsg, wParam, lParam);
	}
}