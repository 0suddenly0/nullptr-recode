#include "../hooks.h"
#include "../../functions/visuals/glow.h"

namespace hooks {
	int __stdcall do_post_screen_space_effects::hook(int a1) {
		auto o_do_post_screen_space_effects = client_mode_vhook.get_original<fn>(indexes::do_post_screen_space_effects);

		if (sdk::local_player) {
			glow::init();
		}

		return o_do_post_screen_space_effects(sdk::client_mode, a1);
	}
}