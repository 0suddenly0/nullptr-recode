#define NOMINMAX
#include "../../sdk/structures/structures.h"
#include "misc.h"
#include "../../settings/settings.h"
#include "../notify/notify.h"
#include "../../helpers/input.h"
#include "../../helpers/math/math.h"

namespace misc {
	namespace bhop {
		void on_create_move(c_user_cmd* cmd)
		{
			if (!settings::misc::bhop::enable)
				return;

			static bool jumped_last_tick = false;
			static bool should_fake_jump = false;

			if (!jumped_last_tick && should_fake_jump) {
				should_fake_jump = false;
				cmd->buttons |= IN_JUMP;
			}
			else if (cmd->buttons & IN_JUMP) {
				if (sdk::local_player->m_flags() & FL_ONGROUND) {
					jumped_last_tick = true;
					should_fake_jump = true;
				}
				else {
					cmd->buttons &= ~IN_JUMP;
					jumped_last_tick = false;
				}
			}
			else {
				jumped_last_tick = false;
				should_fake_jump = false;
			}
		}
		void auto_strafe(c_user_cmd* cmd, qangle va) {
			if (settings::misc::bhop::strafe_type == 0) return;

			if (settings::misc::bhop::strafe_type == 1) {
				if (!sdk::local_player || !sdk::local_player->is_alive() || sdk::local_player->move_type() != MOVETYPE_WALK) return;

				bool on_ground = (sdk::local_player->m_flags() & FL_ONGROUND) && !(cmd->buttons & IN_JUMP);
				if (on_ground) return;

				static auto side = 1.0f;
				side = -side;

				auto velocity = sdk::local_player->velocity();
				velocity.z = 0.0f;

				qangle wish_angle = cmd->viewangles;

				auto speed = velocity.length();
				auto ideal_strafe = std::clamp(RAD2DEG(atan(15.f / speed)), 0.0f, 90.0f);

				if (cmd->forwardmove > 0.0f) cmd->forwardmove = 0.0f;

				static auto cl_sidespeed = sdk::cvar->find_var("cl_sidespeed");

				static float old_yaw = 0.f;
				auto yaw_delta = std::remainderf(wish_angle.yaw - old_yaw, 360.0f);
				auto abs_angle_delta = abs(yaw_delta);
				old_yaw = wish_angle.yaw;

				if (abs_angle_delta <= ideal_strafe || abs_angle_delta >= 30.0f) {
					qangle velocity_direction;
					math::vector_angles(velocity, velocity_direction);
					auto velocity_delta = std::remainderf(wish_angle.yaw - velocity_direction.yaw, 360.0f);
					auto retrack = std::clamp(RAD2DEG(atan(30.0f / speed)), 0.0f, 90.0f) * 10.f;
					if (velocity_delta <= retrack || speed <= 15.0f) {
						if (-(retrack) <= velocity_delta || speed <= 15.0f) {
							wish_angle.yaw += side * ideal_strafe;
							cmd->sidemove = cl_sidespeed->get_float() * side;
						}
						else {
							wish_angle.yaw = velocity_direction.yaw - retrack;
							cmd->sidemove = cl_sidespeed->get_float();
						}
					}
					else {
						wish_angle.yaw = velocity_direction.yaw + retrack;
						cmd->sidemove = -cl_sidespeed->get_float();
					}

					math::movement_fix(cmd, wish_angle, cmd->viewangles);
				}
				else if (yaw_delta > 0.0f) {
					cmd->sidemove = -cl_sidespeed->get_float();
				}
				else if (yaw_delta < 0.0f) {
					cmd->sidemove = cl_sidespeed->get_float();
				}
			}
			else if (settings::misc::bhop::strafe_type == 2) {
				static bool is_bhopping;
				static float calculated_direction;
				static bool in_transition;
				static float true_direction;
				static float wish_direction;
				static float step;
				static float rough_direction;

				enum directions {
					FORWARDS = 0,
					BACKWARDS = 180,
					LEFT = 90,
					RIGHT = -90
				};

				is_bhopping = cmd->buttons & IN_JUMP;
				if (!is_bhopping && sdk::local_player->m_flags() & FL_ONGROUND) {
					calculated_direction = directions::FORWARDS;
					in_transition = false;
					return;
				}

				qangle base{ };
				sdk::engine_client->get_view_angles(&base);

				auto get_rough_direction = [&](float true_direction) -> float {
					std::array< float, 4 > minimum = { directions::FORWARDS, directions::BACKWARDS, directions::LEFT, directions::RIGHT };
					float best_angle, best_delta = 181.f;

					for (size_t i = 0; i < minimum.size(); ++i) {
						float rough_direction = base.yaw + minimum.at(i);
						float delta = fabsf(math::normalize_yaw(true_direction - rough_direction));


						if (delta < best_delta) {
							best_angle = rough_direction;
							best_delta = delta;
						}
					}

					return best_angle;
				};

				true_direction = sdk::local_player->velocity().angle().y;

				if (cmd->buttons & IN_FORWARD) {
					wish_direction = base.yaw + directions::FORWARDS;
				}
				else if (cmd->buttons & IN_BACK) {
					wish_direction = base.yaw + directions::BACKWARDS;
				}
				else if (cmd->buttons & IN_MOVELEFT) {
					wish_direction = base.yaw + directions::LEFT;
				}
				else if (cmd->buttons & IN_MOVERIGHT) {
					wish_direction = base.yaw + directions::RIGHT;
				}
				else {

					cmd->buttons |= IN_FORWARD;
					wish_direction = base.yaw + directions::FORWARDS;
				}

				float speed_rotation = std::min(RAD2DEG(std::asinf(30.f / sdk::local_player->velocity().length())) * 0.5f, 45.f);
				if (in_transition) {

					float ideal_step = speed_rotation + calculated_direction;
					step = fabsf(math::normalize_yaw(calculated_direction - ideal_step));

					if (fabsf(math::normalize_yaw(wish_direction - calculated_direction)) > step) {
						float add = math::normalize_yaw(calculated_direction + step);
						float sub = math::normalize_yaw(calculated_direction - step);

						if (fabsf(math::normalize_yaw(wish_direction - add)) >= fabsf(math::normalize_yaw(wish_direction - sub))) {
							calculated_direction -= step;
						}
						else {
							calculated_direction += step;
						}
					}
					else {
						in_transition = false;
					}
				}
				else {
					rough_direction = get_rough_direction(true_direction);
					calculated_direction = rough_direction;

					if (rough_direction != wish_direction) {
						in_transition = true;
					}
				}

				cmd->forwardmove = 0.f;
				cmd->sidemove = cmd->command_number % 2 ? 450.f : -450.f;

				float direction = (cmd->command_number % 2 ? speed_rotation : -speed_rotation) + calculated_direction;

				float rotation = DEG2RAD(base.yaw - direction);

				float cos_rot = cos(rotation);
				float sin_rot = sin(rotation);

				float forwardmove = (cos_rot * cmd->forwardmove) - (sin_rot * cmd->sidemove);
				float sidemove = (sin_rot * cmd->forwardmove) + (cos_rot * cmd->sidemove);

				cmd->forwardmove = forwardmove;
				cmd->sidemove = sidemove;
			}
		}
	}

	void fake_latency(c_net_channel* net_channel) {

		if (!sdk::local_player->is_alive()) return;

		static auto old_seq = 0;

		if (old_seq == net_channel->in_sequence_nr)return;
		old_seq = net_channel->in_sequence_nr;

		net_channel->in_sequence_nr += (64 - 1) * 2 - static_cast<uint32_t>((64 - 1) * (settings::misc::fake_latency::amount / 1000.f));
	}
}