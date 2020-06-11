#include "../hooks.h"

namespace hooks {
	void __stdcall frame_stage_notify::hook(frame_stage_t framestage) {
		auto o_frame_stage_notify = client_vhook.get_original<fn>(indexes::frame_stage_notify);

		if (!sdk::client_state || !sdk::local_player || !sdk::engine_client->is_connected()) {
			if (net_channel_vhook.class_base) {
				net_channel_vhook.class_clear();
			}
		} else if (!sdk::client_state->net_channel && net_channel_vhook.class_base) {
			net_channel_vhook.class_clear();
		}

		return o_frame_stage_notify(sdk::chl_client, framestage);
	}
}