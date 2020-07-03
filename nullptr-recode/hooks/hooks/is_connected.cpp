#include "../hooks.h"

namespace hooks {
	bool __fastcall is_connected::hook(void* ecx, void* edx) {
		static auto o_is_connected = engine_vhook.get_original<fn>(indexes::is_connected);
		static auto return_address = utils::pattern_scan(GetModuleHandleW(L"client.dll"), "84 C0 75 04 B0 01 5F");

		if (_ReturnAddress() == return_address && settings::misc::unlock_inventory) {
			return false;
		}

		return o_is_connected(ecx);
	}
}