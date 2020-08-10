#include "../hooks.h"

namespace hooks {
	void spotted::hook(const c_recv_proxy_data* data, void* ent, void* out) {
		static auto o_spotted = spotted_vhook->get_original_function();
		*(int*)out = settings::visuals::ingame_radar ? 1 : data->value.m_int;
		o_spotted(data, ent, out);
	}
}