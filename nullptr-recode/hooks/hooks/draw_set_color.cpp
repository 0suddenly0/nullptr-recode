#include "../hooks.h"

namespace hooks {
	void __stdcall draw_set_color::hook(int r, int g, int b, int a) {
		static auto o_draw_set_color = surface_vhook.get_original<fn>(indexes::draw_set_color);

		static uint8_t* return_to_scope_arc = utils::pattern_scan(GetModuleHandleW(L"client.dll"), "6A 00 FF 50 3C 8B 0D ? ? ? ? FF B7 ? ? ? ?") + 0x5;
		static uint8_t* return_to_scope_lens = utils::pattern_scan(GetModuleHandleW(L"client.dll"), "FF 50 3C 8B 4C 24 20") + 0x3;

		if ((_ReturnAddress() == return_to_scope_arc || _ReturnAddress() == return_to_scope_lens) && settings::misc::disable_zoom_border)
			a = 0;

		return o_draw_set_color(sdk::vgui_surface, r, g, b, a);
	}
}