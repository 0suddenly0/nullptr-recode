#include "../hooks.h"

namespace hooks {
	bool __fastcall fire_event_client_side::hook(void* ecx, void* edx, c_game_event* event) {
		auto o_fire_event_client_side = game_event_vhook.get_original<fn>(indexes::fire_event_client_side);

		if (!event) return o_fire_event_client_side(ecx, event);

		if (strstr(event->get_name(), "player_death")) {
			if (sdk::engine_client->get_player_for_user_id(event->get_int("attacker")) == sdk::engine_client->get_local_player()) {
				const char* icon_override = skin_changer::get_icon_override(event->get_string("weapon"));

				if (icon_override)
					event->set_string("weapon", icon_override);
			}
		}

		return o_fire_event_client_side(ecx, event);
	}
}