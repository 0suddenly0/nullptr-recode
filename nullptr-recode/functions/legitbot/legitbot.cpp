#include "legitbot.h"
#include "../autowall/autowall.h"
#include "../../helpers/helpers.h"

namespace legitbot {
	legitbot_settings_t* get_settings() {
		int group_id = -1;
		int weapon_id = -1;

		if (sdk::engine_client->is_in_game() && sdk::local_player && sdk::local_player->is_alive() && sdk::local_player->active_weapon().get()) {
			c_base_combat_weapon* weapon = sdk::local_player->active_weapon().get();
			if (!weapon->get_cs_weapon_data()) return nullptr;
			weapon_id = weapon->is_knife() ? knife : weapon->item().item_definition_index();

			auto list_weapon = std::find_if(weapon_names.begin(), weapon_names.end(), [weapon_id](const weapon_name_t& weapon) {
				return weapon.definition_index == weapon_id;
				});

			switch (weapon->get_cs_weapon_data()->weapon_type) {
			case weapon_type_knife: group_id = weapon->is_knife() ? 0 : 1; break;
			default: group_id = weapon->get_cs_weapon_data()->weapon_type + 1; break;
			}

			if (list_weapon != weapon_names.end() && !globals::show_menu && settings::legitbot::auto_current) {
				settings::legitbot::group_id = group_id;
				settings::legitbot::weapon_id = weapon_id;
				settings::legitbot::weapon_id_in_list = std::abs(std::distance(weapon_names.begin(), list_weapon));
			}
		}

		switch(settings::legitbot::settings_type) {
		case 0: return &settings::legitbot::legitbot_items_all; break;
		case 1: return &settings::legitbot::legitbot_items_groups[globals::show_menu || group_id == -1 ? settings::legitbot::group_id : group_id]; break;
		case 2: return &settings::legitbot::legitbot_items[globals::show_menu || weapon_id == -1 ? settings::legitbot::weapon_id : weapon_id]; break;
		}
	}

	bool hit_chance(qangle angles, c_base_player* player, float chance) {
		c_base_combat_weapon* weapon = sdk::local_player->active_weapon().get();

		if (!weapon) return false;
		if (sdk::cvar->find_var("weapon_accuracy_nospread")->get_int()) return true;

		const auto round_acc = [](const float accuracy) { return roundf(accuracy * 1000.f) / 1000.f; };

		if (weapon->is_knife()) return false;
		if (weapon->item().item_definition_index() == revolver)
			return weapon->get_inaccuracy() < (sdk::local_player->m_flags() & ducking ? .0020f : .0055f);

		if (sdk::local_player->m_flags() & ducking) {
			if (round_acc(weapon->get_inaccuracy()) == round_acc(weapon->is_sniper() ? weapon->get_cs_weapon_data()->inaccuracy_crouch_alt : weapon->get_cs_weapon_data()->inaccuracy_crouch)) return true;
		} else {
			if (round_acc(weapon->get_inaccuracy()) == round_acc(weapon->is_sniper() ? weapon->get_cs_weapon_data()->spread_alt : weapon->get_cs_weapon_data()->spread)) return true;
		}

		vec3 forward, right, up;
		math::angle_vectors(angles, forward, right, up);

		int hits = 0;
		int needed_hits = (int)(256.f * (chance / 100.f));

		weapon->update_accuracy_penalty();
		float weap_spread = weapon->get_spread();
		float weap_inaccuracy = weapon->get_inaccuracy();

		for (int i = 0; i < 256; i++) {
			float a = utils::random(0.f, 1.f);
			float b = utils::random(0.f, 2.f * M_PI);
			float c = utils::random(0.f, 1.f);
			float d = utils::random(0.f, 2.f * M_PI);

			float inaccuracy = a * weap_inaccuracy;
			float spread = c * weap_spread;

			vec3 spreadView((cos(b) * inaccuracy) + (cos(d) * spread), (sin(b) * inaccuracy) + (sin(d) * spread), 0.f), direction;

			direction.x = forward.x + (spreadView.x * right.x) + (spreadView.y * up.x);
			direction.y = forward.y + (spreadView.x * right.y) + (spreadView.y * up.y);
			direction.z = forward.z + (spreadView.x * right.z) + (spreadView.y * up.z);
			direction.normalized();

			qangle view_angles_spread;
			math::vector_angles(direction, up, view_angles_spread);
			view_angles_spread.normalize();

			vec3 view_forward;
			math::angle_vector(view_angles_spread, view_forward);
			view_forward.normalized();

			view_forward = sdk::local_player->get_eye_pos() + (view_forward * weapon->get_cs_weapon_data()->range);

			trace_t tr;
			ray_t ray;

			ray.init(sdk::local_player->get_eye_pos(), view_forward);
			sdk::engine_trace->clip_ray_to_entity(ray, MASK_SHOT | CONTENTS_GRATE, player, &tr);

			if (tr.hit_entity == player)
				++hits;

			if ((int)(((float)(hits) / 256.f) * 100.f) > chance) return true;
		}

		return false;
	}

	void auto_stop(c_user_cmd* cmd) {
		constexpr bool is_hexagone_godlike = true;
		if (sdk::local_player->move_type() != move_type::walk || !(sdk::local_player->m_flags() & entity_flags::on_ground) || cmd->buttons & IN_JUMP)
			return;

		vec3 hvel = sdk::local_player->velocity();
		hvel.z = 0;
		float speed = hvel.length_2d();

		if (speed < 1.f) {
			cmd->forwardmove = 0.f;
			cmd->sidemove = 0.f;
			return;
		}

		static convar* sv_accelerate = sdk::cvar->find_var("sv_accelerate");
		static convar* sv_maxspeed = sdk::cvar->find_var("sv_maxspeed");
		
		float max_accelspeed = sv_accelerate->get_float() * sdk::global_vars->interval_per_tick * sv_maxspeed->get_float();
		float wishspeed = 0.f;

		if (speed - max_accelspeed <= -1.f) wishspeed = max_accelspeed / (speed / (sv_accelerate->get_float() * sdk::global_vars->interval_per_tick));
		else wishspeed = max_accelspeed;

		qangle ndir_angle;
		math::vector_angles(hvel * -1.f, ndir_angle);
		ndir_angle.yaw -= cmd->viewangles.yaw;
		vec3 ndir_vec;
		math::angle_vector(ndir_angle, ndir_vec);

		cmd->forwardmove = ndir_vec.x * wishspeed;
		cmd->sidemove = ndir_vec.y * wishspeed;
	}

	float get_fov_to_player(qangle view_angle, qangle aim_angle) {
		qangle delta = aim_angle - view_angle;
		math::fix_angles(delta);
		return sqrtf(powf(delta.pitch, 2.0f) + powf(delta.yaw, 2.0f));
	}

	bool is_line_goes_through_smoke(vec3 start_pos, vec3 end_pos){
		static uint8_t* address = utils::pattern_scan(GetModuleHandleW(L"client.dll"), "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0");
		static auto line_goes_through_smoke_fn = (bool(*)(vec3, vec3))address;
		return line_goes_through_smoke_fn(start_pos, end_pos);
	}

	bool is_enabled(c_user_cmd* cmd) {
		if (!sdk::engine_client->is_in_game() || !sdk::local_player || !sdk::local_player->is_alive() || (settings::legitbot::using_bind && !settings::legitbot::bind.enable)) return false;

		c_base_combat_weapon* weapon = sdk::local_player->active_weapon().get();
		if (!weapon || !weapon->is_gun() || !get_settings() || (!weapon->has_bullets() && !weapon->is_knife())) return false;

		settings = *get_settings();

		if (!settings.enabled) return false;

		return cmd->buttons & IN_ATTACK || ((weapon->item().item_definition_index() == revolver || weapon->is_knife()) && cmd->buttons & IN_ATTACK2) || (settings.autofire_bind.enable);
	}

	void smooth(qangle current_angle, qangle aim_angle, qangle& angle) {
		float smooth_value = max(1.0f, settings.smooth);

		vec3 current, aim;

		math::angle_vector(current_angle, current);
		math::angle_vector(aim_angle, aim);

		const vec3 delta = aim - current;
		const vec3 smoothed = current + delta / smooth_value;

		math::vector_angles(smoothed, angle);
	}

	void rcs(qangle& angle, c_base_player* target) {
		if (!settings.rcs_enabled || shots_fired < settings.rcs_start - 1 || (!settings.rcs_x && !settings.rcs_y)) return;

		static convar* recoil_scale = sdk::cvar->find_var("weapon_recoil_scale");
		float scale = recoil_scale->get_float();

		float x = float(settings.rcs_y) / 100.f * scale;
		float y = float(settings.rcs_x) / 100.f * scale;

		qangle punch = { 0, 0, 0 };

		if (target) punch = { current_punch.pitch * x, current_punch.yaw * y, 0 };
		else if (settings.rcs_type == 0) punch = { (current_punch.pitch - last_punch.pitch) * x, (current_punch.yaw - last_punch.yaw) * y, 0 };

		if ((punch.pitch != 0.f || punch.yaw != 0.f) && current_punch.roll == 0.f) {
			angle -= punch;
			math::fix_angles(angle);
		}
	}

	bool is_silent() {
		return !(shots_fired > 0 || !settings.silent || !settings.silent_fov);
	}

	float get_fov() {
		if (is_silent()) return settings.silent_fov;
		return settings.fov;
	}

	c_base_player* get_closest_player(c_user_cmd* cmd, int& best_bone, float& best_fov, qangle& best_angles) {
		if (target && !kill_delay && settings.kill_delay > 0 && !target->is_alive()) {
			target = nullptr;
			kill_delay = true;
			kill_delay_time = int(GetTickCount()) + settings.kill_delay;
		}

		if (kill_delay) {
			if (kill_delay_time <= int(GetTickCount())) kill_delay = false;
			else return nullptr;
		}

		target = nullptr;

		std::vector<int> hitboxes;

		for (int i = 0; i < settings.hitboxes.size(); i++) {
			if (!settings.hitboxes[i]) continue;

			switch (i) {
			case 0:
			case 1:
			case 4:
				hitboxes.emplace_back(i);
				break;
			case 2:
				hitboxes.emplace_back(hitbox_upper_chest);
				break;
			case 3:
				hitboxes.emplace_back(hitbox_chest);
				break;
			case 5:
				hitboxes.emplace_back(hitbox_stomach);
				break;
			case 6:
				hitboxes.emplace_back(hitbox_pelvis);
				break;
			case 7:
				hitboxes.emplace_back(hitbox_right_forearm);
				hitboxes.emplace_back(hitbox_right_hand);
				hitboxes.emplace_back(hitbox_right_upper_arm);

				hitboxes.emplace_back(hitbox_left_forearm); 
				hitboxes.emplace_back(hitbox_left_hand);
				hitboxes.emplace_back(hitbox_left_upper_arm);
				break;
			case 8:
				hitboxes.emplace_back(hitbox_right_calf);
				hitboxes.emplace_back(hitbox_right_thigh);

				hitboxes.emplace_back(hitbox_left_thigh); 
				hitboxes.emplace_back(hitbox_left_calf);
				break;
			case 9:
				hitboxes.emplace_back(hitbox_right_foot);
				hitboxes.emplace_back(hitbox_left_foot);
				break;
			}
		}

		for (int i = 1; i <= sdk::global_vars->max_clients; i++) {
			c_base_player* player = c_base_player::get_player_by_index(i);
			if (!player || !player->is_alive() || player == sdk::local_player || !player->is_player() || player->gun_game_immunity() || player->is_dormant() || (!player->is_enemy() && settings.team_check)) continue;

			bool backtrack_enabled = legit_backtrack::data.count(player->ent_index()) > 0 && settings.aim_at_backtrack;

			for (int hitbox : hitboxes) {
				vec3 hitbox_pos = player->get_hitbox_pos(hitbox);
				qangle ang;
				math::vector_angles(hitbox_pos - sdk::local_player->get_eye_pos(), ang);
				const float fov = get_fov_to_player(cmd->viewangles + last_punch * 2.f, ang);

				float best_backtrack_fov = FLT_MAX;
				qangle best_backtrack_ang;
				bool can_backtrack = false;
				if (backtrack_enabled) {
					for (backtrack_data& record : legit_backtrack::data.at(player->ent_index())) {
						vec3 backtrack_hitbox_pos = record.get_bone_pos(hitbox);
						qangle backtrack_ang;
						math::vector_angles(backtrack_hitbox_pos - sdk::local_player->get_eye_pos(), backtrack_ang);
						const float backtrack_fov = get_fov_to_player(cmd->viewangles + last_punch * 2.f, backtrack_ang);

						if (backtrack_fov > get_fov() || sdk::local_player->can_see_player(backtrack_hitbox_pos) < 0.9f) continue;

						if (best_backtrack_fov > backtrack_fov) {
							best_backtrack_fov = backtrack_fov;
							best_backtrack_ang = backtrack_ang;

							can_backtrack = settings.backtrack_priority == 1 ? best_backtrack_fov < fov : fov > get_fov();
							target = player;
						}
					}
				}

				if ((settings.smoke_check && is_line_goes_through_smoke(sdk::local_player->get_eye_pos(), hitbox_pos)) || (fov > get_fov() && !can_backtrack)) continue;

				if (!sdk::local_player->can_see_player(player, hitbox_pos)) {
					if (!settings.autowall || can_backtrack) continue;
					int damage = autowall::get(sdk::local_player->get_eye_pos(), hitbox_pos, sdk::local_player, player).damage;
					if (damage < settings.autowall_min_damage) continue;
				}

				if (best_fov > fov) {
					best_bone = hitbox;
					best_angles = can_backtrack ? best_backtrack_ang : ang;
					best_fov = fov;
					target = player;
				}
			}
		}

		return target;
	}

	void run(c_user_cmd* cmd) {
		if (int(GetTickCount()) > last_shot_tick + 50) shots_fired = 0;

		current_punch = sdk::local_player->aim_punch_angle();

		if (!is_enabled(cmd)) {
			last_punch = { 0, 0, 0 };
			shot_delay = false;
			kill_delay = false;
			target = nullptr;
			return;
		}

		c_base_combat_weapon* weapon = sdk::local_player->active_weapon().get();
		if (!weapon) return;
		c_cs_weapon_info* weapon_data = weapon->get_cs_weapon_data();
		if (!weapon_data) return;
		if (settings.flash_check && sdk::local_player->is_flashed(50.f)) return;

		qangle angles = cmd->viewangles;
		qangle current = angles;
		float fov = FLT_MAX;

		int best_bone = -1;
		if (get_closest_player(cmd, best_bone, fov, angles)) {
			if (!settings.autofire_bind.enable && !settings.silent && !shot_delay && settings.shot_delay > 0 && !shots_fired) {
				shot_delay = true;
				shot_delay_time = int(GetTickCount()) + settings.shot_delay;
			}

			if (shot_delay && shot_delay_time <= int(GetTickCount())) shot_delay = false;
			if (shot_delay) cmd->buttons &= ~IN_ATTACK;

			if (settings.autofire_bind.enable) {
				if (hit_chance(angles, target, settings.autofire_chance)) cmd->buttons |= IN_ATTACK;
				else if(settings.auto_stop && weapon->next_primary_attack() < sdk::global_vars->curtime && sdk::local_player->next_attack() < sdk::global_vars->curtime) auto_stop(cmd);
			}
		}

		if (cmd->buttons & IN_ATTACK || cmd->buttons & IN_ATTACK2) {
			if ((cmd->buttons & IN_ATTACK) && !is_silent())
				rcs(angles, target);

			last_punch = current_punch;

			if (!is_silent())
				smooth(current, angles, angles);

			math::fix_angles(angles);

			if (utils::random(0, 100) < settings.silent_chance)
				cmd->viewangles = angles;

			if (!is_silent())
				sdk::engine_client->set_view_angles(&angles);
		}

		if (sdk::local_player->active_weapon()->is_pistol() && settings.autopistol) {
			const float server_time = sdk::local_player->tick_base() * sdk::global_vars->interval_per_tick;
			const float next_shot = sdk::local_player->active_weapon()->next_primary_attack() - server_time;
			if (next_shot > 0) cmd->buttons &= ~IN_ATTACK;
		}

		if (cmd->buttons & IN_ATTACK) {
			last_shot_tick = GetTickCount();
			shots_fired++;
		}
	}
}