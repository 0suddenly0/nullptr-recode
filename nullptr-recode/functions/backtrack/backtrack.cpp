#include "backtrack.h"
#include "../legitbot/legitbot.h"
#include "../../helpers/helpers.h"

namespace legit_backtrack {
	std::map<int, std::deque<backtrack_data>> data = { };

	void run(c_user_cmd* cmd) {
		if (!sdk::engine_client->is_in_game() || !sdk::local_player || !sdk::local_player->is_alive()) {
			data.clear();
			return;
		}

		c_base_combat_weapon* weapon = sdk::local_player->active_weapon().get();
		if (!weapon || !weapon->is_gun()) {
			data.clear();
			return;
		}

		if (!legitbot::get_settings() || !legitbot::get_settings()->enabled || legitbot::get_settings()->backtrack_ticks == 0 || (settings::legitbot::using_bind && !settings::legitbot::bind.enable)) {
			data.clear();
			return;
		}

		static convar* sv_maxunlag = sdk::cvar->find_var("sv_maxunlag");
		static convar* sv_minupdaterate = sdk::cvar->find_var("sv_minupdaterate");
		static convar* sv_maxupdaterate = sdk::cvar->find_var("sv_maxupdaterate");

		static convar* sv_client_min_interp_ratio = sdk::cvar->find_var("sv_client_min_interp_ratio");
		static convar* sv_client_max_interp_ratio = sdk::cvar->find_var("sv_client_max_interp_ratio");

		static convar* cl_interp_ratio = sdk::cvar->find_var("cl_interp_ratio");
		static convar* cl_interp = sdk::cvar->find_var("cl_interp");
		static convar* cl_updaterate = sdk::cvar->find_var("cl_updaterate");

		float updaterate = cl_updaterate->get_float();

		float minupdaterate = sv_minupdaterate->get_float();
		float maxupdaterate = sv_maxupdaterate->get_float();

		float min_interp = sv_client_min_interp_ratio->get_float();
		float max_interp = sv_client_max_interp_ratio->get_float();

		float flLerpAmount = cl_interp->get_float();
		float flLerpRatio = cl_interp_ratio->get_float();
		flLerpRatio = math::clamp(flLerpRatio, min_interp, max_interp);

		float updateRate = 0.f;

		updateRate = math::clamp(updaterate, minupdaterate, maxupdaterate);
		lerp_time = std::fmaxf(flLerpAmount, flLerpRatio / updateRate);
		latency = sdk::engine_client->get_net_channel_info()->get_latency(FLOW_OUTGOING) + sdk::engine_client->get_net_channel_info()->get_latency(FLOW_INCOMING);
		correct_time = latency + lerp_time;

		c_base_player* player;
		for (int i = 1; i <= sdk::global_vars->max_clients; ++i) {
			player = c_base_player::get_player_by_index(i);

			if (player->is_not_target() || (legitbot::get_settings()->team_check && player->team_num() == sdk::local_player->team_num())) {
				if (data.count(i) > 0) data.erase(i);
				continue;
			}

			std::deque<backtrack_data>& cur_data = data[i];
			if (!cur_data.empty()) {
				backtrack_data& front = cur_data.front();
				if (front.simulation_time == player->simulation_time())
					continue;

				while (!cur_data.empty()) {
					backtrack_data& back = cur_data.back();
					float deltaTime = correct_time - (sdk::global_vars->curtime - back.simulation_time);
					if (std::fabsf(deltaTime) <= TICKS_TO_TIME(legitbot::get_settings()->backtrack_ticks))
						break;

					cur_data.pop_back();
				}
			}

			const model_t* model = player->get_model();
			if (!model) continue;

			studiohdr_t* hdr = sdk::mdl_info->get_studiomodel(model);
			if (!hdr) continue;

			backtrack_data this_data;
			this_data.player = player;
			this_data.simulation_time = player->simulation_time();

			*(vec3*)((uintptr_t)player + 0xA0) = player->vec_origin();
			*(int*)((uintptr_t)player + 0xA68) = 0;
			*(int*)((uintptr_t)player + 0xA30) = 0;
			player->invalidate_bone_cache();
			player->setup_bones(this_data.bone_matrix, 128, BONE_USED_BY_ANYTHING, sdk::global_vars->curtime);

			float deltaTime = correct_time - (sdk::global_vars->curtime - this_data.simulation_time);
			if (!(std::fabsf(deltaTime) > TICKS_TO_TIME(legitbot::get_settings()->backtrack_ticks))) {
				data[i].push_front(this_data);
			}

		}

		qangle angles;
		int tick_count = -1;
		float best_fov = 180.0f;
		for (auto& node : data) {
			auto& cur_data = node.second;
			if (cur_data.empty())
				continue;

			for (backtrack_data& this_data : cur_data) {
				float deltaTime = correct_time - (sdk::global_vars->curtime - this_data.simulation_time);
				if (std::fabsf(deltaTime) > TICKS_TO_TIME(legitbot::get_settings()->backtrack_ticks))
					continue;

				math::vector_angles(this_data.get_bone_pos(hitbox_head) - sdk::local_player->get_eye_pos(), angles);
				math::fix_angles(angles);
				float fov = legitbot::get_fov_to_player(cmd->viewangles, angles);
				if (best_fov > fov) {
					best_fov = fov;
					tick_count = TIME_TO_TICKS(this_data.simulation_time + lerp_time);
				}
			}
		}

		if (tick_count != -1)
			cmd->tick_count = tick_count;
	}
}

