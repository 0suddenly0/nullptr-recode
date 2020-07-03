#include "../hooks.h"

namespace hooks {
	int __fastcall weapon_spread::hook(void* ecx, void* edx) {
		static auto o_weapon_spread = weapon_spread_vhook.get_original<fn>(indexes::cvar_get_bool);
		static auto return_address = utils::pattern_scan(GetModuleHandleA("client.dll"), "85 C0 0F 84 ? ? ? ? E8 ? ? ? ? 99");
		if (_ReturnAddress() == return_address 
			&& settings::visuals::sniper_crosshair 
			&& sdk::local_player
			&& sdk::local_player->active_weapon()
			&& !sdk::local_player->is_scoped())
			return 2;

		return o_weapon_spread(ecx);
	}
}