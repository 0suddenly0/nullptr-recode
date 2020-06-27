#include "../hooks.h"

namespace hooks {
	bool __fastcall send_net_msg::hook(c_net_channel* pNetChan, void* edx, c_net_message& msg, bool bForceReliable, bool bVoice) {
		static auto o_send_net_msg = net_channel_vhook.get_original<fn>(indexes::send_net_msg);

		if (sdk::local_player && sdk::engine_client->is_connected()) {
			if (msg.get_type() == 14) return true;
			if (msg.get_group() == 9) bVoice = true;
		}

		return o_send_net_msg(pNetChan, msg, bForceReliable, bVoice);
	}
}