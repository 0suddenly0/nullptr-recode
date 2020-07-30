#pragma once
#include "../../sdk/sdk.h"
#include "../../sdk/structures/structures.h"
#include "../../helpers/helpers.h"

struct return_info_t {
	int damage = -1;
	int hitgroup = -1;
	int walls = 4;
	bool did_penetrate_wall = false;
	float thickness = 1.f;

	c_client_entity* ent = nullptr;
	vec3 end = vec3();
};

struct fbdata_t {
	vec3 start = vec3();
	vec3 end = vec3();
	vec3 pos = vec3();
	vec3 dir = vec3();

	c_base_trace_filter* filter = nullptr;
	trace_t trace;

	float thickness = 1.f;
	float damage = 1.f;
	int walls = 4;
};

namespace autowall {
	float get_estimated_point_damage(vec3 point);
	return_info_t get(vec3 start, vec3 end, c_base_player* from_ent = nullptr, c_base_player* to_ent = nullptr, int hitgroup = -1);

	void clip_trace_to_player(vec3& start, vec3& end, c_base_player* ent, unsigned int mask, c_base_trace_filter* filter, trace_t* trace);
	void scale_damage(c_base_player* ent, c_cs_weapon_info* inf, int& hitgroup, float& damage);
	bool handle_bullet_penetration(c_cs_weapon_info* inf, fbdata_t& bullet);
	bool trace_to_exit(trace_t* enter_trace, vec3& start, vec3& dir, trace_t* exit_trace);
	bool is_breakable(c_base_player* e);
}
