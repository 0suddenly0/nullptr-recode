#include "../hooks.h"

namespace hooks {
	void spotted::hook(const c_recv_proxy_data* data, void* ent, void* out) {
		*(int*)out = settings::visuals::ingame_radar ? 1 : data->value.m_int;
	}
}