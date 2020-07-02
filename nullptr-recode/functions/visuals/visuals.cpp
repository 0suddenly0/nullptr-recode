#include "visuals.h"
#include "../../settings/settings.h"
#include "../../sdk/structures/structures.h"
struct spectator_t {
	std::string name;
	observer_mode_t mode;
};

float get_defuse_time(c_planted_c4* bomb) {
	static float defuse_time = -1;
	if (!bomb) return 0;
	if (!bomb->bomb_defuser()) defuse_time = -1;
	else if (defuse_time == -1) defuse_time = sdk::global_vars->curtime + bomb->defuse_length();
	if (defuse_time > -1 && bomb->bomb_defuser()) return defuse_time - sdk::global_vars->curtime;
	return 0;
}

c_base_player* get_bomb_player() {
	for (int i = 1; i <= sdk::entity_list->get_highest_entity_index(); ++i) {
		c_base_player* entity = c_base_player::get_player_by_index(i);
		if (!entity || entity->is_player() || entity->is_dormant() || entity == sdk::local_player)
			continue;

		if (entity->is_planted_c4())
			return entity;
	}

	return nullptr;
}

c_planted_c4* get_bomb() {
	c_base_entity* entity;
	for (int i = 1; i <= sdk::entity_list->get_max_entities(); ++i) {
		entity = c_base_entity::get_entity_by_index(i);
		if (entity && !entity->is_dormant() && entity->is_planted_c4())
			return (c_planted_c4*)(entity);
	}

	return nullptr;
}

c_base_player* get_damage_player() {
	if (sdk::local_player->is_alive()) {
		return sdk::local_player;
	} else {
		if (sdk::local_player->observer_mode() == observer_mode_t::obs_roaming) {
			return nullptr;
		} else {
			c_base_player* spectated_ent = sdk::local_player->observer_target();
			if (!spectated_ent) return nullptr;
			return spectated_ent;
		}
	}

	return sdk::local_player;
}

float get_bomb_damage() {
	c_base_player* bomb = get_bomb_player();

	if (!bomb)
		return 0.f;

	float armor = get_damage_player()->armor_value();
	float distance = get_damage_player()->get_eye_pos().dist_to(bomb->get_abs_origin());

	float a = 450.7f;
	float b = 75.68f;
	float c = 789.2f;
	float d = ((distance - b) / c);
	float damage = a * exp(-d * d);

	float dmg = damage;

	if (armor > 0)
	{
		float _new = dmg * 0.5f;
		float armor = (dmg - _new) * 0.5f;

		if (armor > (float)(armor))
		{
			armor = float(armor) * (1.f / 0.5f);
			_new = dmg - armor;
		}

		damage = _new;
	}
	return damage;
}

namespace visuals {
	void render() {
		bomb_indicator();
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
		vec2 watermark_text_size = render::get_text_size(watermark_text);	

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
		c_utl_vector<client_hit_verify_t>& client_impact_list = sdk::local_player->get_client_impacts();

		for (int i = client_impact_list.Count(); i > last_count; i--) {
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
					spectator_t& spec = spectators[i];

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

	void night_mode() {
		static bool night_mode_done = false;

		if (!sdk::engine_client->is_in_game() || !sdk::local_player) {
			night_mode_done = false;
			return;
		}

		static std::string mapname = sdk::engine_client->get_level_name_short();
		if (mapname != sdk::engine_client->get_level_name_short()) {
			night_mode_done = false;
			mapname = sdk::engine_client->get_level_name_short();
		}

		static bool old_value = settings::visuals::night_mode;
		if (old_value != settings::visuals::night_mode || globals::unloading)
			night_mode_done = false;

		if (!night_mode_done) {
			static const auto load_named_sky = (void(__fastcall*)(const char*))(utils::pattern_scan(GetModuleHandleW(L"engine.dll"), "55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45"));
			static convar* sv_skyname = sdk::cvar->find_var("sv_skyname");
			static convar* r_3dsky = sdk::cvar->find_var("r_3dsky");

			load_named_sky(settings::visuals::night_mode && !globals::unloading ? "sky_csgo_night02" : sv_skyname->get_string());
			r_3dsky->set_value(!settings::visuals::night_mode);
			for (material_handle_t i = sdk::mat_system->first_material(); i != sdk::mat_system->invalid_material(); i = sdk::mat_system->next_material(i)) {
				c_material* material = sdk::mat_system->get_material(i);
				if (!material) continue;

				if (strstr(material->get_texture_group_name(), "World")) {
					if (settings::visuals::night_mode && !globals::unloading) material->color_modulate(0.18, 0.18, 0.15);
					else material->color_modulate(1.0f, 1.0f, 1.0f);
				}
			}
			night_mode_done = true;
		}

		old_value = settings::visuals::night_mode;
	}

	void asus_props() {
		if (!sdk::engine_client->is_in_game() || !sdk::local_player) return;

		static convar* r_drawstaticprops = sdk::cvar->find_var("r_drawstaticprops");
		if (settings::visuals::props::enable && !globals::unloading) {
			r_drawstaticprops->m_fnChangeCallbacks.m_Size = 0; 
			r_drawstaticprops->set_value(3);
		} else {
			r_drawstaticprops->m_fnChangeCallbacks.m_Size = 0;
			r_drawstaticprops->set_value(1);
		}

		if (settings::visuals::props::request || globals::unloading) {
			for (material_handle_t i = sdk::mat_system->first_material(); i != sdk::mat_system->invalid_material(); i = sdk::mat_system->next_material(i)) {
				c_material* material = sdk::mat_system->get_material(i);
				if (!material) continue;

				if (strstr(material->get_texture_group_name(), "StaticProp")) {
					if (settings::visuals::props::request && !globals::unloading) material->alpha_modulate(settings::visuals::props::alpha / 255.f);
					else material->alpha_modulate(1.0f);
				}
			}
			settings::visuals::props::request = false;
		}
	}

	void bomb_indicator() {
		if (!sdk::engine_client->is_connected() || !sdk::local_player || !settings::visuals::bomb_timer) return;
		int x, y;
		sdk::engine_client->get_screen_size(x, y);

		c_planted_c4* bomb = get_bomb();
		if (!bomb) return;
		float bomb_timer = bomb->c4_blow() - sdk::global_vars->curtime;
		if (bomb_timer < 0) return;

		vec2 window_pos = vec2(0, y - 430);
		static vec2 window_size(96, 15);// 3 item - 35 | 2 item - 25 | 1 item - 15

		if (settings::visuals::bomb_timer_pos == 1) {
			if (!get_bomb_player()) return;
			if (!math::world2screen(get_bomb_player()->get_abs_origin(), window_pos)) return;

			window_pos.x -= window_size.x / 2;
			window_pos.y -= window_size.y / 2;
		}

		render::draw_box_filled(window_pos, window_pos + window_size, color(0, 0, 0, 100));
		render::draw_box_filled(window_pos, window_pos + vec2(2.f, window_size.y), globals::menu_color);
		render::draw_text(bomb->bomb_defused() ? "defused" : utils::snprintf("bomb: %s", bomb_timer >= 0 ? utils::snprintf("%.3f", bomb_timer).c_str() : "0"), vec2(window_pos.x + 4, window_pos.y + 2), color(255, 255, 255, 255), false);
	
		float time = get_defuse_time(bomb);
		float bomb_damage = get_bomb_damage();

		if (bomb->bomb_defused()) {
			window_size.y = 15;
			return;
		}

		if (get_defuse_time(bomb) > 0) {
			bool def_kits = bomb->bomb_defuser()->has_defuser_kit();

			std::vector<render::multicolor_t> items = {
				render::multicolor_t{"defuse: ", color(255, 255, 255, 255)},
				render::multicolor_t{utils::snprintf("%.3f", time), bomb_timer < time ? color(255, 50, 50, 255) : color(50, 255, 50, 255)}
			};
			render::draw_text_multicolor(items, vec2(window_pos.x + 4, window_pos.y + 12), false);

			if (bomb_damage > 1 && bomb_timer >= 0) {
				render::draw_text(utils::snprintf("damage: %s", bomb_damage < 100 ? utils::snprintf("%.0f", bomb_damage).c_str() : "you dead"), vec2(window_pos.x + 4, window_pos.y + 22), color(255, 255, 255, 255), false);
				window_size.y = 35;
			} else {
				window_size.y = 25;
			}

			float box_w = (float)fabs(0 - window_size.x);
			float width;

			if (def_kits) {
				width = (((box_w * time) / 5.f));
			} else {
				width = (((box_w * time) / 10.f));
			}

			render::draw_box_filled(window_pos + vec2(0.f, window_size.y + 2.f), window_pos + vec2(width, window_size.y + 4.f), color(50, 50, 255, 255));
		} else if (get_defuse_time(bomb) <= 0) {
			if (bomb_damage > 1 && bomb_timer >= 0) {
				render::draw_text(utils::snprintf("damage: %s", bomb_damage < 100 ? utils::snprintf("%.0f", bomb_damage).c_str() : "you dead"), vec2(window_pos.x + 4, window_pos.y + 12), color(255, 255, 255, 255), false);
				window_size.y = 25;
			} else {
				window_size.y = 15;
			}
		}

		float box_w = (float)fabs(0 - window_size.x);
		float width = (((box_w * bomb_timer) / sdk::cvar->find_var("mp_c4timer")->get_float()));
		render::draw_box_filled(window_pos + vec2(0.f, window_size.y), window_pos + vec2(width, window_size.y + 2), color(255, 100, 100, 255));
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

			act = (in_attack && in_attack2) ? act_drop :
				(in_attack2) ? act_throw :
				(in_attack) ? act_lob :
				act_none;
		}

		void view() {
			if (!settings::visuals::grenade_prediction::enable) return;
			if (!sdk::local_player) return;
			auto weapon = sdk::local_player->active_weapon().get();
			if (!weapon) return;

			if ((weapon->is_grenade()) && act != act_none) {
				qangle Angles;
				sdk::engine_client->get_view_angles(&Angles);


				type = (item_definition_index)weapon->item().item_definition_index();
				simulate(Angles, sdk::local_player);

			}
		}

		void paint() {
			if (!settings::visuals::grenade_prediction::enable) return;
			if (!sdk::local_player) return;
			c_base_combat_weapon* weapon = sdk::local_player->active_weapon().get();
			if (!weapon) return;

			if ((type) && path.size() > 1 && other_collisions.size() > 0 && act != act_none && weapon->is_grenade()) {
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
				int bestdmg = 0;

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