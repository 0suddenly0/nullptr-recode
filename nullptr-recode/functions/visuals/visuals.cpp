#include "visuals.h"
#include "../../settings/settings.h"
#include "../../sdk/structures/structures.h"
struct spectator_t {
	std::string name;
	observer_mode_t mode;
};

namespace visuals {
	void render() {
		impact();
		grenade_prediction::paint();
		draw_watermark();
		entity_loop();
	}

	void draw_watermark() {
		if (!settings::misc::watermark) return;

		//settings
		vec2 offsets = vec2(10, 10);
		vec2 offsets_text = vec2(6, 4);
		float bar_size = 4.f;
		float rounding = 4.f;

		vec2 screen_size = utils::get_screen_size();
		std::string watermark_text = utils::snprintf("nullptr | %s | fps: %i", utils::current_date_time().c_str(), null_gui::deep::get_framerate());
		vec2 watermark_text_size = vec2(render::default_font->CalcTextSizeA(12.f, FLT_MAX, 0.0f, watermark_text.c_str()).x, render::default_font->CalcTextSizeA(12.f, FLT_MAX, 0.0f, watermark_text.c_str()).y);

		vec2 bar_start_position  = vec2(screen_size.x - offsets.x - watermark_text_size.x - offsets_text.x, offsets.y);
		vec2 bar_end_position    = vec2(screen_size.x - offsets.x                                         , offsets.y + bar_size);

		vec2 main_start_position = vec2(screen_size.x - offsets.x - watermark_text_size.x - offsets_text.x, offsets.y + bar_size);
		vec2 main_end_position   = vec2(screen_size.x - offsets.x                                         , offsets.y + watermark_text_size.y + bar_size + offsets_text.y);

		render::draw_box_filled_rounded(bar_start_position, bar_end_position, color(globals::menu_color, 255), rounding, 3);  // draw top line
		render::draw_box_filled_rounded(main_start_position, main_end_position, color(50, 50, 50, 100), rounding, 12); // draw watermark body

		render::draw_text(watermark_text, main_start_position + vec2(offsets_text.x / 2, offsets_text.y / 2), color(255,255,255,255), false);
	}

	void thirdperson() {
		static observer_mode_t bec_person = observer_mode_t::obs_in_eye;
		static bool setted_person = false;

		if (!sdk::local_player) return;

		if (settings::misc::third_person::bind.enable) {
			if (sdk::local_player->is_alive()) {
				if (!sdk::input->m_fCameraInThirdPerson) {
					sdk::input->m_fCameraInThirdPerson = true;
				}

				float dist = settings::misc::third_person::dist;

				qangle* view = sdk::local_player->get_vangles();
				trace_t tr;
				ray_t ray;

				vec3 desiredCamOffset = vec3(cos(DEG2RAD(view->yaw)) * dist,
					sin(DEG2RAD(view->yaw)) * dist,
					sin(DEG2RAD(-view->pitch)) * dist
				);

				ray.init(sdk::local_player->get_eye_pos(), (sdk::local_player->get_eye_pos() - desiredCamOffset));
				c_trace_filter traceFilter;
				traceFilter.pSkip = sdk::local_player;
				sdk::engine_trace->trace_ray(ray, MASK_SHOT, &traceFilter, &tr);

				vec3 diff = sdk::local_player->get_eye_pos() - tr.endpos;

				float distance2D = sqrt(abs(diff.x * diff.x) + abs(diff.y * diff.y));

				bool horOK = distance2D > (dist - 2.0f);
				bool vertOK = (abs(diff.z) - abs(desiredCamOffset.z) < 3.0f);

				float cameraDistance;

				if (horOK && vertOK) {
					cameraDistance = dist; 
				} else {
					if (vertOK) {
						cameraDistance = distance2D * 0.95f;
					} else {
						cameraDistance = abs(diff.z) * 0.95f;
					}
				}
				sdk::input->m_fCameraInThirdPerson = true;

				sdk::input->m_vecCameraOffset.z = cameraDistance;

				setted_person = false;
			} else {
				if (sdk::local_player->observer_target() != nullptr) {
					if (sdk::local_player->observer_mode() == observer_mode_t::obs_in_eye) {
						bec_person = sdk::local_player->observer_mode();
						setted_person = true;
					}

					if (setted_person) sdk::local_player->observer_mode() = observer_mode_t::obs_chase;
				}

				sdk::input->m_fCameraInThirdPerson = false;
			}
		} else {
			if (!sdk::local_player->is_alive()) {
				if (setted_person) sdk::local_player->observer_mode() = bec_person;
				setted_person = false;
			}
			sdk::input->m_fCameraInThirdPerson = false;
		}
	}

	void impact() {
		if (!sdk::local_player || !sdk::local_player->is_alive() || !sdk::engine_client->is_connected()) return;

		for (int i = 0; i < globals::server_impacts.size(); i++) {
			bullet_impact_t cur_impact = globals::server_impacts[i];
			sdk::debug_overlay->add_box_overlay(cur_impact.pos, vec3(-cur_impact.size, -cur_impact.size, -cur_impact.size), vec3(cur_impact.size, cur_impact.size, cur_impact.size), qangle(0, 0, 0), cur_impact.clr.color_char[0], cur_impact.clr.color_char[1], cur_impact.clr.color_char[2], cur_impact.clr.color_char[3], cur_impact.time_life);

			if (sdk::global_vars->curtime - cur_impact.time_del < 0)
				globals::server_impacts.erase(globals::server_impacts.begin() + i);
		}

		static int last_count = 0;
		auto& client_impact_list = sdk::local_player->get_client_impacts();

		for (auto i = client_impact_list.Count(); i > last_count; i--) {
			if (settings::visuals::impacts::client::enable) {
				sdk::debug_overlay->add_box_overlay(
					client_impact_list[i - 1].pos,
					vec3(-settings::visuals::impacts::client::size, -settings::visuals::impacts::client::size, -settings::visuals::impacts::client::size),
					vec3(settings::visuals::impacts::client::size, settings::visuals::impacts::client::size, settings::visuals::impacts::client::size),
					qangle(0, 0, 0),
					settings::visuals::impacts::client::clr.color_char[0],
					settings::visuals::impacts::client::clr.color_char[1],
					settings::visuals::impacts::client::clr.color_char[2],
					settings::visuals::impacts::client::clr.color_char[3],
					settings::visuals::impacts::client::show_time);
			}
		}

		if (client_impact_list.Count() != last_count)
			last_count = client_impact_list.Count();
	}

	void spectator_list() {
		int specs = 0;
		std::vector <spectator_t> spectators;

		if (sdk::engine_client->is_in_game() && sdk::engine_client->is_connected()) {
			if (sdk::local_player) {
				for (int i = 0; i < sdk::engine_client->get_max_clients(); i++) {
					c_base_player* player = c_base_player::get_player_by_index(i);
					if (!player) continue;
					if (player->health() > 0) continue;
					if (player == sdk::local_player) continue;
					if (player->is_dormant()) continue;
					if (sdk::local_player->is_alive()) {
						if (player->observer_target() != sdk::local_player)	continue;
						if (player->observer_mode() == observer_mode_t::obs_roaming) continue;
					} else {
						if (sdk::local_player->observer_mode() == observer_mode_t::obs_roaming) {
							if (player->observer_mode() != observer_mode_t::obs_roaming) continue;
						} else {
							if (player->observer_mode() == observer_mode_t::obs_roaming) continue;
							c_base_player* spectated_ent = sdk::local_player->observer_target();
							if (!spectated_ent) continue;
							if (player->observer_target() != spectated_ent)	continue;
						}
					}
					player_info_t info;
					sdk::engine_client->get_player_info(player->ent_index(), &info);
					if (info.ishltv) continue;
					if (info.fakeplayer) continue;
					if (info.szName == "" || info.szName == " ") continue;

					spectators.push_back(spectator_t{ info.szName , player->observer_mode() });
				}
			}
		}

		bool show_window = (!spectators.empty() || globals::show_menu) && settings::windows::spectator_list_show;
		null_gui::deep::set_next_window_alpha(settings::windows::spectator_list_alpha / 255.f);

		null_gui::begin_window("spectator list", (bool*)0, vec2(0, 0), ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | (globals::show_menu && show_window ? 0 : ImGuiWindowFlags_NoMove)); {
			settings::windows::bind_window_pos = null_gui::deep::get_window_pos();
			null_gui::deep::set_window_hidden(!show_window);
			null_gui::deep::set_cursor_y(22);

			null_gui::create_columns(2); {
				null_gui::text_no_space(std::string("name (" + std::to_string(spectators.size()) + ")").c_str());
				null_gui::set_column_width(-1, 150);
				null_gui::next_column();
				null_gui::text_no_space("mode");
			}
			null_gui::create_columns(1);

			null_gui::line(200.f);

			null_gui::create_columns(2); {
				for (int i = 0; i < spectators.size(); i++) {
					auto& spec = spectators[i];

					null_gui::text_no_space(spec.name.c_str());

					null_gui::next_column();

					std::string mode;

					switch (spec.mode) {
					case observer_mode_t::obs_in_eye: mode = "perspective"; break;
					case observer_mode_t::obs_chase: mode = "3rd person"; break;
					case observer_mode_t::obs_roaming: mode = "no clip"; break;
					case observer_mode_t::obs_deathcam: mode = "deathcam"; break;
					case observer_mode_t::obs_freezecam: mode = "freezecam"; break;
					case observer_mode_t::obs_fixed: mode = "fixed"; break;
					}

					null_gui::text_no_space(mode.c_str());

					null_gui::next_column();
				}
			}
			null_gui::create_columns(1);
		}
		null_gui::end_window();
	}

	void entity_loop() {
		for (int i = 1; i < sdk::entity_list->get_highest_entity_index(); ++i) {
			c_base_entity* ent = c_base_entity::get_entity_by_index(i);
			if (!ent) continue;

			if (ent->get_client_class()->class_id == class_id::c_inferno) {
				c_base_inferno* inferno = (c_base_inferno*)ent;
				if (!inferno) continue;

				render::draw_circle_3d(inferno->vec_origin(), 50, inferno->fire_count() * 10.f, color(255, 255, 255, 255));

				render::draw_text(utils::snprintf("%d : %.0f", inferno->fire_count(), ((float)inferno->fire_count() * 10.f)), vec2(100, 10), color(255, 255, 255, 255), true);
			}
		}
	}

	namespace grenade_prediction {
		void tick(int buttons) {
			if (!settings::visuals::grenade_prediction::enable) return;

			bool in_attack = (buttons & IN_ATTACK);
			bool in_attack2 = (buttons & IN_ATTACK2);

			act = (in_attack && in_attack2) ? ACT_DROP :
				(in_attack2) ? ACT_THROW :
				(in_attack) ? ACT_LOB :
				ACT_NONE;
		}

		void view() {
			if (!settings::visuals::grenade_prediction::enable) return;
			if (!sdk::local_player) return;
			auto weapon = sdk::local_player->active_weapon().get();
			if (!weapon) return;

			if ((weapon->is_grenade()) && act != ACT_NONE) {
				qangle Angles;
				sdk::engine_client->get_view_angles(&Angles);


				type = (item_definition_index)weapon->item().item_definition_index();
				simulate(Angles, sdk::local_player);

			}
		}

		void paint() {
			if (!settings::visuals::grenade_prediction::enable) return;
			if (!sdk::local_player) return;
			auto weapon = sdk::local_player->active_weapon().get();
			if (!weapon) return;

			if ((type) && path.size() > 1 && other_collisions.size() > 0 && act != ACT_NONE && weapon->is_grenade()) {
				vec2 ab, cd;
				vec3 prev = path[0];
				for (auto it = path.begin(), end = path.end(); it != end; ++it) {
					if (math::world2screen(prev, ab) && math::world2screen(*it, cd)) {
						render::draw_line(ab[0], ab[1], cd[0], cd[1], settings::visuals::grenade_prediction::main, settings::visuals::grenade_prediction::line_thickness);
					}
					prev = *it;
				}

				for (auto it = other_collisions.begin(), end = other_collisions.end() - 1; it != end; ++it) {
					draw_box(it->first, settings::visuals::grenade_prediction::main_box, settings::visuals::grenade_prediction::colision_box_size);
				}

				draw_box(other_collisions.rbegin()->first, settings::visuals::grenade_prediction::end_box, settings::visuals::grenade_prediction::main_colision_box_size);

				if (settings::visuals::grenade_prediction::radius && get_radius() != 0.f) {
					render::draw_circle_3d(other_collisions.rbegin()->first, 50, get_radius(), settings::visuals::grenade_prediction::radius_color);
				}

				std::string EntName;
				auto bestdmg = 0;

				vec3 endpos = path[path.size() - 1];
				vec3 absendpos = endpos;

				float totaladded = 0.0f;

				while (totaladded < 30.0f) {
					if (sdk::engine_trace->get_point_contents(endpos) == CONTENTS_EMPTY)
						break;

					totaladded += 2.0f;
					endpos.z += 2.0f;
				}

				c_base_combat_weapon* pWeapon = sdk::local_player->active_weapon().get();
				type = (item_definition_index)sdk::local_player->active_weapon()->item().item_definition_index();
			}
		}

		void setup(c_base_player* pl, vec3& vecSrc, vec3& vecThrow, const qangle& angEyeAngles) {
			qangle angThrow = angEyeAngles;
			float pitch = angThrow.pitch;

			if (pitch <= 90.0f) {
				if (pitch < -90.0f) {
					pitch += 360.0f;
				}
			} else {
				pitch -= 360.0f;
			}
			float a = pitch - (90.0f - fabs(pitch)) * 10.0f / 90.0f;
			angThrow.pitch = a;

			float flVel = 750.0f * 0.9f;

			static const float power[] = { 1.0f, 1.0f, 0.5f, 0.0f };
			float b = power[act];
			b = b * 0.7f;
			b = b + 0.3f;
			flVel *= b;

			vec3 vForward, vRight, vUp;
			math::angle_vectors(angThrow, vForward, vRight, vUp);

			vecSrc = pl->vec_origin();
			vecSrc += pl->view_offset();
			float off = (power[act] * 12.0f) - 12.0f;
			vecSrc.z += off;

			trace_t tr;
			vec3 vecDest = vecSrc;
			vecDest.mul_add(vecDest, vForward, 22.0f);
			trace_hull(vecSrc, vecDest, tr);

			vec3 vecBack = vForward; vecBack *= 6.0f;
			vecSrc = tr.endpos;
			vecSrc -= vecBack;

			vecThrow = pl->velocity(); vecThrow *= 1.25f;
			vecThrow.mul_add(vecThrow, vForward, flVel);
		}

		void simulate(qangle& Angles, c_base_player* pLocal) {
			vec3 vecSrc, vecThrow;
			setup(pLocal, vecSrc, vecThrow, Angles);

			float interval = sdk::global_vars->interval_per_tick;

			// Log positions 20 times per sec
			int logstep = (int)(0.05f / interval);
			int logtimer = 0;

			path.clear();
			other_collisions.clear();
			for (unsigned int i = 0; i < path.max_size() - 1; ++i) {
				if (!logtimer)
					path.push_back(vecSrc);

				int s = step(vecSrc, vecThrow, i, interval);
				if ((s & 1) || vecThrow == vec3(0, 0, 0))
					break;

				// Reset the log timer every logstep OR we bounced
				if ((s & 2) || logtimer >= logstep) logtimer = 0;
				else ++logtimer;
			}
			path.push_back(vecSrc);
		}

		int step(vec3& vecSrc, vec3& vecThrow, int tick, float interval) {
			vec3 move;
			add_gravity_move(move, vecThrow, interval, false);

			trace_t tr;
			push_entity(vecSrc, move, tr);

			int result = 0;
			if (check_detonate(vecThrow, tr, tick, interval)) {
				result |= 1;
			}

			if (tr.fraction != 1.0f) {
				result |= 2; // Collision!
				resolve_fly_collision_custom(tr, vecThrow, interval);
				qangle angles;
				math::vector_angles((tr.endpos - tr.startpos).normalized(), angles);
				other_collisions.push_back(std::make_pair(tr.endpos, angles));
			}

			if ((result & 1) || vecThrow == vec3(0, 0, 0)) {
				qangle angles;
				math::vector_angles((tr.endpos - tr.startpos).normalized(), angles);
				other_collisions.push_back(std::make_pair(tr.endpos, angles));
			}
			vecSrc = tr.endpos;
			return result;
		}

		float get_radius(item_definition_index type_locl)
		{
			switch (type_locl) {
			case item_definition_index::smokegrenade: 
				return 114.f;
			case item_definition_index::molotov:
			case item_definition_index::incgrenade:
				return 150.f;
			case item_definition_index::hegrenade:
				return 250.f;
			default:
				return 0.f;
			}
		}

		bool check_detonate(const vec3& vecThrow, const trace_t& tr, int tick, float interval) {
			firegrenade_didnt_hit = false;
			switch (type) {
			case item_definition_index::smokegrenade:
			case item_definition_index::decoy:
				if (vecThrow.length() < 0.1f) {
					int det_tick_mod = (int)(0.2f / interval);
					return !(tick % det_tick_mod);
				}
				return false;
			case item_definition_index::molotov:
			case item_definition_index::incgrenade:
				if (tr.fraction != 1.0f && tr.plane.normal.z > 0.7f)
					return true;
			case item_definition_index::flashbang:
			case item_definition_index::hegrenade:
				firegrenade_didnt_hit = (float)(tick)* interval > 1.5f && !(tick % (int)(0.2f / interval));
				return firegrenade_didnt_hit;
			default:
				return false;
			}
		}

		void trace_hull(vec3& src, vec3& end, trace_t& tr) {
			static const vec3 hull[2] = { vec3(-2.0f, -2.0f, -2.0f), vec3(2.0f, 2.0f, 2.0f) };

			c_trace_filter filter;
			filter.set_ignore_class("BaseCSGrenadeProjectile");
			filter.set_ignore_class("SmokeGrenadeProjectile");
			filter.set_ignore_class("MolotovProjectile");
			filter.set_ignore_class("DecoyProjectile");

			filter.pSkip = sdk::entity_list->get_client_entity(sdk::engine_client->get_local_player());

			ray_t ray;
			ray.init(src, end, hull[0], hull[1]);

			const unsigned int mask = 0x200400B;
			sdk::engine_trace->trace_ray(ray, mask, &filter, &tr);
		}

		void add_gravity_move(vec3& move, vec3& vel, float frametime, bool onground) {
			vec3 basevel(0.0f, 0.0f, 0.0f);

			move.x = (vel.x + basevel.x) * frametime;
			move.y = (vel.y + basevel.y) * frametime;

			if (onground) {
				move.z = (vel.z + basevel.z) * frametime;
			} else {
				// Game calls GetActualGravity( this );
				float gravity = 800.0f * 0.4f;

				float newZ = vel.z - (gravity * frametime);
				move.z = ((vel.z + newZ) / 2.0f + basevel.z) * frametime;

				vel.z = newZ;
			}
		}

		void push_entity(vec3& src, const vec3& move, trace_t& tr) {
			vec3 vecAbsEnd = src;
			vecAbsEnd += move;
			trace_hull(src, vecAbsEnd, tr);
		}

		void resolve_fly_collision_custom(trace_t& tr, vec3& vecVelocity, float interval) {
			float flSurfaceElasticity = 1.0;
			float flGrenadeElasticity = 0.45f;
			float flTotalElasticity = flGrenadeElasticity * flSurfaceElasticity;
			if (flTotalElasticity > 0.9f) flTotalElasticity = 0.9f;
			if (flTotalElasticity < 0.0f) flTotalElasticity = 0.0f;

			vec3 vecAbsVelocity;
			physics_clip_velocity(vecVelocity, tr.plane.normal, vecAbsVelocity, 2.0f);
			vecAbsVelocity *= flTotalElasticity;

			float flSpeedSqr = vecAbsVelocity.length_sqr();
			static const float flMinSpeedSqr = 20.0f * 20.0f; // 30.0f * 30.0f in CSS
			if (flSpeedSqr < flMinSpeedSqr)
				vecAbsVelocity.zero();

			if (tr.plane.normal.z > 0.7f) {
				vecVelocity = vecAbsVelocity;
				vecAbsVelocity.mul((1.0f - tr.fraction) * interval);
				push_entity(tr.endpos, vecAbsVelocity, tr);
			} else {
				vecVelocity = vecAbsVelocity;
			}
		}

		int physics_clip_velocity(const vec3& in, const vec3& normal, vec3& out, float overbounce) {
			float    backoff;
			float    change;
			float    angle;
			int        i, blocked;

			blocked = 0;

			angle = normal[2];

			if (angle > 0) {
				blocked |= 1;
			}
			if (!angle) {
				blocked |= 2;
			}

			backoff = in.dot(normal) * overbounce;

			for (i = 0; i < 3; i++) {
				change = normal[i] * backoff;
				out[i] = in[i] - change;
				if (out[i] > -0.1f && out[i] < 0.1f) {
					out[i] = 0;
				}
			}

			return blocked;
		}

		void draw_box(vec3 position, color clr, float size){
			vec2 pos;
			if (!math::world2screen(position, pos)) return;
			render::draw_box_filled(pos - size, pos + size, clr);
		}

	}
}