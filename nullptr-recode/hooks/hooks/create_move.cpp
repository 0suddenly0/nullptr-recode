#include "../hooks.h"

namespace hooks {
	bool __stdcall create_move::hook(float input_sample_frametime, c_user_cmd* cmd) {
		bool* send_packet = reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(_AddressOfReturnAddress()) + 0x14);
		auto oCreateMove = client_mode_vhook.get_original<fn>(indexes::create_move);
		bool result = oCreateMove(sdk::client_mode, input_sample_frametime, cmd);

		if (!cmd || !cmd->command_number || globals::unloading) return result;

		if (globals::show_menu)
			cmd->buttons &= ~(IN_ATTACK | IN_ATTACK2);

		misc::bhop::on_create_move(cmd);
		misc::bhop::auto_strafe(cmd, cmd->viewangles);
		
		return false;
	}
}