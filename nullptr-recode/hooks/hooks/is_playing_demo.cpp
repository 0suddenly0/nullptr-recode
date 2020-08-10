#include "../hooks.h"

namespace hooks {
	bool __stdcall is_playing_demo::hook() {
		if (settings::misc::reveal_money
			&& *(uintptr_t*)(_ReturnAddress()) == 0x0975C084
			&& **(uintptr_t**)(uintptr_t(_AddressOfReturnAddress()) + 4) == 0x0C75C084) {
			return true;
		}
		return false;
	}
}
