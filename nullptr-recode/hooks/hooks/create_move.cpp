#include "../hooks.h"

namespace hooks {
	void __stdcall create_move::hook(int sequence_number, float input_sample_frametime, bool active, bool& send_packet) {
		static auto o_create_move = client_vhook.get_original<fn>(indexes::create_move);

		o_create_move(sdk::chl_client, 0, sequence_number, input_sample_frametime, active);

		auto cmd = sdk::input->get_user_cmd(sequence_number);
		auto verified = sdk::input->get_verified_cmd(sequence_number);

		if (!cmd || !cmd->command_number || globals::unloading) return;

		if (!net_channel_vhook.class_base) {
			auto net_channel = sdk::client_state->net_channel;
			if (net_channel) {
				net_channel_vhook.setup(net_channel);
				net_channel_vhook.hook_index(indexes::send_datagram, send_datagram::hook);
				net_channel_vhook.hook_index(indexes::send_net_msg,  send_net_msg::hook);
			}
		}

		if (globals::show_menu)
			cmd->buttons &= ~(IN_ATTACK | IN_ATTACK2);

		misc::bhop::on_create_move(cmd);
		misc::bhop::auto_strafe(cmd, cmd->viewangles);
		
		verified->m_cmd = *cmd;
		verified->m_crc = cmd->get_checksum();
	}
}