#include "../hooks.h"

namespace hooks {
	void __stdcall frame_stage_notify::hook(frame_stage_t stage) {
		auto o_frame_stage_notify = client_vhook.get_original<fn>(indexes::frame_stage_notify);

		if (!sdk::client_state || !sdk::local_player || !sdk::engine_client->is_connected()) {
			if (net_channel_vhook.class_base) {
				net_channel_vhook.unhook_all();
				net_channel_vhook.class_clear();
			}
		} else if (!sdk::client_state->net_channel && net_channel_vhook.class_base) {
			net_channel_vhook.unhook_all();
			net_channel_vhook.class_clear();
		}

		misc::gravity();
		misc::viewmodel();
		misc::no_smoke();
		misc::fog();
		misc::agent_changer(stage);
		skin_changer::on_frame_stage_notify(stage);

		if (stage == frame_stage_t::render_start) {
			visuals::night_mode();
			visuals::asus_props();
		}

		return o_frame_stage_notify(sdk::chl_client, stage);
	}
}