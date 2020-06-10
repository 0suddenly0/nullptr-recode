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
				} else {
					cmd->buttons &= ~IN_JUMP;
					jumped_last_tick = false;
				}
			} else {
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
			} else if (settings::misc::bhop::strafe_type == 2) {
				static bool is_bhopping;
				static float calculated_direction;
				static bool in_transition;
				static float true_direction;
				static float wish_direction;
				static float step;
				static float rough_direction;

				enum directions
				{
					FORWARDS = 0,
					BACKWARDS = 180,
					LEFT = 90,
					RIGHT = -90
				};

				// Reset direction when player is not strafing
				is_bhopping = cmd->buttons & IN_JUMP;
				if (!is_bhopping && sdk::local_player->m_flags() & FL_ONGROUND)
				{
					calculated_direction = directions::FORWARDS;
					in_transition = false;
					return;
				}

				// Get true view angles
				qangle base{ };
				sdk::engine_client->get_view_angles(&base);

				// Calculate the rough direction closest to the player's true direction
				auto get_rough_direction = [&](float true_direction) -> float
				{
					// Make array with our four rough directions
					std::array< float, 4 > minimum = { directions::FORWARDS, directions::BACKWARDS, directions::LEFT, directions::RIGHT };
					float best_angle, best_delta = 181.f;

					// Loop through our rough directions and find which one is closest to our true direction
					for (size_t i = 0; i < minimum.size(); ++i)
					{
						float rough_direction = base.yaw + minimum.at(i);
						float delta = fabsf(math::normalize_yaw(true_direction - rough_direction));

						// Only the smallest delta wins out
						if (delta < best_delta)
						{
							best_angle = rough_direction;
							best_delta = delta;
						}
					}

					return best_angle;
				};

				// Get true direction based on player velocity
				true_direction = sdk::local_player->velocity().angle().y;

				// Detect wish direction based on movement keypresses
				if (cmd->buttons & IN_FORWARD)
				{
					wish_direction = base.yaw + directions::FORWARDS;
				}
				else if (cmd->buttons & IN_BACK)
				{
					wish_direction = base.yaw + directions::BACKWARDS;
				}
				else if (cmd->buttons & IN_MOVELEFT)
				{
					wish_direction = base.yaw + directions::LEFT;
				}
				else if (cmd->buttons & IN_MOVERIGHT)
				{
					wish_direction = base.yaw + directions::RIGHT;
				}
				else
				{
					// Reset to forward when not pressing a movement key, then fix anti-aim strafing by setting IN_FORWARD
					cmd->buttons |= IN_FORWARD;
					wish_direction = base.yaw + directions::FORWARDS;
				}
				// Calculate the ideal strafe rotation based on player speed (c) navewindre
				float speed_rotation = std::min(RAD2DEG(std::asinf(30.f / sdk::local_player->velocity().length())) * 0.5f, 45.f);
				if (in_transition)
				{
					// Get value to rotate by via calculated speed rotation
					float ideal_step = speed_rotation + calculated_direction;
					step = fabsf(math::normalize_yaw(calculated_direction - ideal_step)); // 15.f is a good alternative, but here is where you would do your "speed" slider value for the autostrafer

					// Check when the calculated direction arrives close to the wish direction
					if (fabsf(math::normalize_yaw(wish_direction - calculated_direction)) > step)
					{
						float add = math::normalize_yaw(calculated_direction + step);
						float sub = math::normalize_yaw(calculated_direction - step);

						// Step in direction that gets us closer to our wish direction
						if (fabsf(math::normalize_yaw(wish_direction - add)) >= fabsf(math::normalize_yaw(wish_direction - sub)))
						{
							calculated_direction -= step;
						}
						else
						{
							calculated_direction += step;
						}
					}
					else
					{
						// Stop transitioning when we meet our wish direction
						in_transition = false;
					}
				}
				else
				{
					// Get rough direction and setup calculated direction only when not transitioning
					rough_direction = get_rough_direction(true_direction);
					calculated_direction = rough_direction;

					// When we have a difference between our current (rough) direction and our wish direction, then transition
					if (rough_direction != wish_direction)
					{
						in_transition = true;
					}
				}

				// Set movement up to be rotated
				cmd->forwardmove = 0.f;
				cmd->sidemove = cmd->command_number % 2 ? 450.f : -450.f;

				// Calculate ideal rotation based on our newly calculated direction
				float direction = (cmd->command_number % 2 ? speed_rotation : -speed_rotation) + calculated_direction;

				// Rotate our direction based on our new, defininite direction
				float rotation = DEG2RAD(base.yaw - direction);

				float cos_rot = cos(rotation);
				float sin_rot = sin(rotation);

				float forwardmove = (cos_rot * cmd->forwardmove) - (sin_rot * cmd->sidemove);
				float sidemove = (sin_rot * cmd->forwardmove) + (cos_rot * cmd->sidemove);

				// Apply newly rotated movement
				cmd->forwardmove = forwardmove;
				cmd->sidemove = sidemove;
			}
		}
	}
}