#pragma once
#include "../helpers/input.h"

namespace settings {
	namespace misc {
		bool bind_window = false;

		bool obs_bypass = false;

		bool watermark = true;
		key_bind_t watermark_bind = key_bind_t{ 0, 0, false };

		namespace bhop {
			bool enable = false;
			int strafe_type = 0;
		}
	}
}