#include "../hooks.h"

namespace hooks {
	bool __fastcall draw_model_stats_overlay::hook(void* ecx, void* edx) {
		static auto o_draw_model_stats_overlay = draw_model_stats_overlay_vhook.get_original<fn>(indexes::cvar_get_bool);
		static const auto return_address = utils::pattern_scan(GetModuleHandleW(L"client.dll"), "85 C0 75 54 8B 0D ? ? ? ?");

		if (_ReturnAddress() == return_address)
			return true;

		return o_draw_model_stats_overlay(ecx);
	}
}