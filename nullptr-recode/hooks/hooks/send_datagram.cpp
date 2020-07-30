#include "../hooks.h"

namespace hooks {
	int __fastcall send_datagram::hook(void* net_channel, void*, void* datagram) {
		static auto o_send_datagram = net_channel_vhook.get_original<fn>(indexes::send_datagram);

		if (!settings::misc::fake_latency::enable || !sdk::engine_client->is_connected() || !sdk::local_player) return o_send_datagram(net_channel, datagram);

		c_net_channel* channel = (c_net_channel*)(net_channel);
		int instate = channel->in_reliable_state;
		int insequencenr = channel->in_sequence_nr;

		if (settings::misc::fake_latency::enable) misc::fake_latency(channel);

		int ret = o_send_datagram(channel, datagram);

		channel->in_reliable_state = instate;
		channel->in_sequence_nr = insequencenr;

		return ret;
	}
}