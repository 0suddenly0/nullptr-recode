#include "../hooks.h"
#include "../../gui/imgui/imgui_impl_win32.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace hooks {
	LRESULT __stdcall wndproc::hook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		input::process_message(uMsg, wParam, lParam);

		if (globals::show_menu && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam) == 0) return true;

		return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
	}
}