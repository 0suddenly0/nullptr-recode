#include "../hooks.h"

namespace hooks {
	void __fastcall client_command::hook(void* ecx, void* edx, const char* command) {
		auto o_client_command = engine_vhook.get_original<fn>(indexes::client_command);	
		auto new_cmd = utils::s2ws(std::string(command));
		int text_size = std::string(command).size();
		int size = strstr(command, "say ") ? 6 : strstr(command, "say_team ") ? 11 : 0;

		if (settings::misc::chat_filter_bypass && ((strstr(command, "say ") && text_size > size) || (strstr(command, "say_team ") && text_size > size))) {
			for (int i = size; i < new_cmd.size(); i++) {
				if (i % 2) new_cmd.insert(i, L"\u200B");
			}
			return o_client_command(sdk::engine_client, utils::ws2s(new_cmd).c_str());
		}
		return o_client_command(sdk::engine_client, command);
	}
}