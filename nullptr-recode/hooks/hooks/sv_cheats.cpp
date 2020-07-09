#include "../hooks.h"

namespace hooks{
	bool __fastcall sv_cheats::hook(void* ecx, void* edx) {
		static auto o_sv_cheats = sv_cheats_vhook.get_original<fn>(indexes::cvar_get_bool);
		static const auto return_address = utils::pattern_scan(GetModuleHandleW(L"client.dll"), "85 C0 75 30 38 86");
		if (_ReturnAddress() == return_address)
			return true;

		return o_sv_cheats(ecx);
	}
}