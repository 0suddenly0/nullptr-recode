#pragma once
#include "../helpers/input.h"

namespace settings {
	namespace misc {
		bool bind_window = false;

		bool watermark = true;
		key_bind_t watermark_bind = key_bind_t{ 0, 0, false };
	}
}