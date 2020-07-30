#pragma once
#include "../../settings/settings.h"
#include "../../sdk/structures/structures.h"
#include <map>
#include <deque>

class backtrack_data {
public:
	vec3 get_bone_pos(int bone) {
		vec3 ret = vec3(0, 0, 0);

		const model_t* model = player->get_model();
		if (!model) return ret;

		studiohdr_t* hdr = sdk::mdl_info->get_studiomodel(model);
		if (!hdr) return ret;

		mstudiohitboxset_t* hitboxset = hdr->get_hitbox_set(player->hitbox_set());
		mstudiobbox_t* hitbox = hitboxset->get_hitbox(bone);
		vec3 hitbox_center = (hitbox->bbmin + hitbox->bbmax) * 0.5f;
		math::vector_transform(hitbox_center, bone_matrix[hitbox->bone], ret);

		return ret;
	}

	c_base_player* player;
	float simulation_time;
	matrix3x4 bone_matrix[128];
};

namespace legit_backtrack {
	void run(c_user_cmd* cmd);

	float correct_time = 0.0f;
	float latency = 0.0f;
	float lerp_time = 0.0f;
	extern std::map<int, std::deque<backtrack_data>> data;
};