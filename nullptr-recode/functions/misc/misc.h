#pragma once
#include <deque>
#include "../../sdk/sdk.h"

namespace misc {
	namespace bhop {
		void on_create_move(c_user_cmd* cmd);
		void auto_strafe(c_user_cmd* cmd, qangle viewangles);
	}
}