#pragma once
#include <deque>
#include "../../sdk/sdk.h"

namespace misc {
	void prepare_revolver(c_user_cmd* cmd);
	void moon_walk(c_user_cmd* cmd);
	void fast_duck(c_user_cmd* cmd);
	void block_bot(c_user_cmd* cmd);
	void edge_jump(c_user_cmd* cmd, int old_flags);
	void disable_flash_alpha();
	void no_smoke();
	void fog();

	namespace clan_tag {
		void init();

		extern int user_index;
	}

	namespace bhop {
		void on_create_move(c_user_cmd* cmd);
		void auto_strafe(c_user_cmd* cmd, qangle viewangles);
	}

	void fake_latency(c_net_channel* net_channel);
}