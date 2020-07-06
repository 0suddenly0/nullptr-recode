#include "visuals.h"
#include "../../settings/settings.h"
#include "../../sdk/structures/structures.h"

std::map<int, std::string> fixed_names = {
	//pistols
	{ item_definition_index::usp_silencer, "usp" },
	{ item_definition_index::glock, "glock" },
	{ item_definition_index::hkp2000, "p2000" },
	{ item_definition_index::elite, "elite" },
	{ item_definition_index::p250, "p250" },
	{ item_definition_index::fiveseven, "five-seven" },
	{ item_definition_index::cz75a, "cz75a" },
	{ item_definition_index::tec9, "tec9" },
	{ item_definition_index::deagle, "deagle" },
	{ item_definition_index::revolver, "rovelver" },
	////////////////////////////////////////////////

	//pp
	{ item_definition_index::mp9, "mp9" },
	{ item_definition_index::mac10, "mac10" },
	{ item_definition_index::mp7, "mp7" },
	{ item_definition_index::mp5, "mp5-sd" },
	{ item_definition_index::ump45, "ump45" },
	{ item_definition_index::p90, "p90" },
	{ item_definition_index::bizon, "bizon" },
	////////////////////////////////////////////////

	//rifles
	{ item_definition_index::famas, "famas" },
	{ item_definition_index::m4a1_silencer, "m4a1-s" },
	{ item_definition_index::m4a1, "m4a1" },
	{ item_definition_index::ssg08, "ssg08" },
	{ item_definition_index::aug, "aug" },
	{ item_definition_index::awp, "awp" },
	{ item_definition_index::scar20, "scar20" },
	{ item_definition_index::galilar, "galil" },
	{ item_definition_index::ak47, "ak 47" },
	{ item_definition_index::sg556, "sg553" },
	{ item_definition_index::g3sg1, "g3sg1" },
	////////////////////////////////////////////////

	//have
	{ item_definition_index::nova, "nova" },
	{ item_definition_index::xm1014, "xm1014" },
	{ item_definition_index::sawedoff, "sawedoff" },
	{ item_definition_index::m249, "m249" },
	{ item_definition_index::negev, "negev" },
	{ item_definition_index::mag7, "mag7" },
	////////////////////////////////////////////////

	//granades
	{ item_definition_index::flashbang, "flash" },
	{ item_definition_index::smokegrenade, "smoke" },
	{ item_definition_index::molotov, "molotov" },
	{ item_definition_index::incgrenade, "inc" },
	{ item_definition_index::decoy, "decoy" },
	{ item_definition_index::hegrenade, "hae" },
	////////////////////////////////////////////////

	//other
	{ item_definition_index::c4, "c4" },
	{ item_definition_index::knife, "knife" },
	{ item_definition_index::knife_gg, "knife" },
	{ item_definition_index::knife_bayonet, "knife" },
	{ item_definition_index::knife_butterfly, "knife" },
	{ item_definition_index::knife_falchion, "knife" },
	{ item_definition_index::knife_flip, "knife" },
	{ item_definition_index::knife_ghost, "knife" },
	{ item_definition_index::knife_gut, "knife" },
	{ item_definition_index::knife_gypsy_jack, "knife" },
	{ item_definition_index::knife_karambit, "knife" },
	{ item_definition_index::knife_m9_bayonet, "knife" },
	{ item_definition_index::knife_navaja, "knife" },
	{ item_definition_index::knife_push, "knife" },
	{ item_definition_index::knife_stiletto, "knife" },
	{ item_definition_index::knife_survival_bowie, "knife" },
	{ item_definition_index::knife_t, "knife" },
	{ item_definition_index::knife_tactical, "knife" },
	{ item_definition_index::knife_talon, "knife" },
	{ item_definition_index::knife_ursus, "knife" },
	{ item_definition_index::knife_widowmaker, "knife" },
	{ item_definition_index::taser, "zeus" }
	////////////////////////////////////////////////
};

const char* clean_item_name(const char* name) {
	if (name[0] == 'C')
		name++;

	auto start = strstr(name, "Weapon");
	if (start != nullptr)
		name = start + 6;

	return name;
};

struct spectator_t {
	std::string name;
	observer_mode_t mode;
};

float player_alpha[65];
float player_alpha_offscreen[65];
float last_view_time[1024];
float last_view_time_offscreen[1024];

RECT get_bbox(c_base_entity* ent) {
	RECT rect{};
	auto collideable = ent->get_collideable();
	if (!collideable) return rect;

	auto min = collideable->obb_mins();
	auto max = collideable->obb_maxs();

	const matrix3x4& trans = ent->coordinate_frame();


	vec3 points[] = {
		vec3(min.x, min.y, min.z),
		vec3(min.x, max.y, min.z),
		vec3(max.x, max.y, min.z),
		vec3(max.x, min.y, min.z),
		vec3(max.x, max.y, max.z),
		vec3(min.x, max.y, max.z),
		vec3(min.x, min.y, max.z),
		vec3(max.x, min.y, max.z)
	};

	vec3 pointsTransformed[8];
	for (int i = 0; i < 8; i++) {
		math::vector_transform(points[i], trans, pointsTransformed[i]);
	}

	vec2 screen_points[8] = {};

	for (int i = 0; i < 8; i++) {
		if (!math::world2screen(pointsTransformed[i], screen_points[i]))
			return rect;
	}

	auto left = screen_points[0].x;
	auto top = screen_points[0].y;
	auto right = screen_points[0].x;
	auto bottom = screen_points[0].y;

	for (int i = 1; i < 8; i++) {
		if (left > screen_points[i].x)
			left = screen_points[i].x;
		if (top < screen_points[i].y)
			top = screen_points[i].y;
		if (right < screen_points[i].x)
			right = screen_points[i].x;
		if (bottom > screen_points[i].y)
			bottom = screen_points[i].y;
	}
	return RECT{ (long)left, (long)top, (long)right, (long)bottom };
}

class esp_player {
public:
	static int get_player_alpha_offscreen(int index, int max = 255)
	{
		int i = player_alpha_offscreen[index];
		i = std::clamp(i, 0, max);
		return i;
	}

	static int get_player_alpha(int alpha, int index)
	{
		int i = alpha - player_alpha[index];
		i = std::clamp(i, 0, 255);
		return i;
	}

	bool begin(c_base_player* _player) {
		if (!_player->is_alive()) return false;

		player = _player;
		is_localplayer = sdk::local_player->ent_index() == _player->ent_index();
		is_enemy = sdk::local_player->team_num() != _player->team_num();
		is_visible = sdk::local_player->can_see_player(_player, hitbox_chest);

		if (sdk::local_player->observer_target() == player && sdk::local_player->observer_mode() != observer_mode_t::obs_chase && !settings::misc::third_person::bind.enable) {
			return false;
		}

		if (is_enemy) {
			current_settings = settings::visuals::esp::esp_items[esp_types::enemies];
		} else if (!is_enemy && !is_localplayer) {
			current_settings = settings::visuals::esp::esp_items[esp_types::teammates];
		} else if (is_localplayer) {
			if (settings::misc::third_person::bind.enable) current_settings = settings::visuals::esp::esp_items[esp_types::local_player];
			else return false;
		}

		if (!current_settings.enable) return false;

		clr = is_visible ? current_settings.box_visible : current_settings.box_invisible;

		auto head = _player->get_hitbox_pos(hitbox_head);
		auto origin = _player->vec_origin();

		head.z += 6;

		if (!math::world2screen(head, head_pos) ||
			!math::world2screen(origin, feet_pos)) {
			return false;
		}

		auto h = fabs(head_pos.y - feet_pos.y);
		auto w = h / 2.f;

		bbox.left = (long)(feet_pos.x - w * 0.45f);
		bbox.right = (long)(bbox.left + w);
		bbox.bottom = (long)(feet_pos.y);
		bbox.top = (long)(head_pos.y);
		return true;
	}

	c_base_player* player;
	bool          is_enemy;
	bool          is_visible;
	bool          is_localplayer;
	color         clr;
	vec2          head_pos;
	vec2          feet_pos;
	RECT          bbox;
	esp_settings_t current_settings;
};



namespace visuals {
	void render() {
		bomb_indicator();
		grenades();
		spread_circle();
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

	void spread_circle() {
		if (!sdk::engine_client->is_in_game() || !sdk::local_player->is_alive() || !settings::visuals::spread_circle::enable) return;
		auto weapon = sdk::local_player->active_weapon();
		if (!weapon) return;

		float spreed = weapon->get_inaccuracy() * 800;
		int w, h;
		int centre_w, centre_h;
		sdk::engine_client->get_screen_size(w, h);
		centre_w = w / 2;
		centre_h = h / 2;

		render::draw_circle_filled(centre_w, centre_h, spreed, 40, settings::visuals::spread_circle::clr);
	}

	void grenade_names(c_base_entity* entity) {
		if (!sdk::local_player)
			return;

		std::string name;
		color clr;

		const model_t* model = entity->get_model();

		if (entity->get_client_class()) {
			if (model) {
				studiohdr_t* hdr = sdk::mdl_info->get_studiomodel(model);
				if (hdr) {
					std::string hdrName = hdr->szName;
					if (hdrName.find("thrown") != std::string::npos || hdrName.find("dropped") != std::string::npos) {
						if (hdrName.find("flashbang") != std::string::npos) {
							name = "flash";
							clr = settings::visuals::grenades::color_flash;
						} else if (hdrName.find("fraggrenade") != std::string::npos) {
							name = "frag";
							clr = settings::visuals::grenades::color_frag;
						} else if (hdrName.find("molotov") != std::string::npos) {
							name = "molotov";
							clr = settings::visuals::grenades::color_molotov;
						} else if (hdrName.find("incendiarygrenade") != std::string::npos) {
							name = "incendiary";
							clr = settings::visuals::grenades::color_molotov;
						} else if (hdrName.find("decoy") != std::string::npos) {
							name = "decoy";
							clr = settings::visuals::grenades::color_decoy;
						} else if (hdrName.find("smoke") != std::string::npos) {
							name = "smoke";
							clr = settings::visuals::grenades::color_smoke;
						}
					}
				}

				if (!name.empty()) {
					vec2 pos;
					if (math::world2screen(entity->vec_origin(), pos))
					{
						render::draw_text(name, pos, clr, true, true);
					}
				}
			}
		}
	}

	void draw_capsule_hitbox(c_base_player* player, bool dead) {
		if (!settings::visuals::hitbox::enable || !player || (settings::visuals::hitbox::show_only_kill && !dead)) return;
		matrix3x4 boneMatrix_actual[MAXSTUDIOBONES];
		if (!player->setup_bones(boneMatrix_actual, 128, 0x00000100 | 0x200, sdk::global_vars->curtime))return;

		studiohdr_t* studioHdr = sdk::mdl_info->get_studiomodel(player->get_model());
		if (studioHdr) {
			mstudiohitboxset_t* set = studioHdr->get_hitbox_set(player->hitbox_set());
			if (set) {
				for (int i = 0; i < set->numhitboxes; i++) {
					mstudiobbox_t* hitbox = set->get_hitbox(i);
					if (hitbox) {
						if (hitbox->m_flRadius != -1.0f) {
							vec3 min_actual = vec3(0, 0, 0);
							vec3 max_actual = vec3(0, 0, 0);

							math::vector_transform(hitbox->bbmin, boneMatrix_actual[hitbox->bone], min_actual);
							math::vector_transform(hitbox->bbmax, boneMatrix_actual[hitbox->bone], max_actual);

							color clr = settings::visuals::hitbox::clr;
							sdk::debug_overlay->add_capsule_overlay(min_actual, max_actual, hitbox->m_flRadius, clr.color_char[0], clr.color_char[1], clr.color_char[2], clr.color_char[3], settings::visuals::hitbox::show_time);
						}
					}
				}
			}
		}
	}

	void grenades() {
		if (!settings::visuals::grenades::enable) return;

		for (int i = 0; i < sdk::entity_list->get_highest_entity_index(); i++) {
			c_base_entity* entity = (c_base_entity*)(sdk::entity_list->get_client_entity(i));
			if (entity && entity != sdk::local_player) {
				grenade_names(entity);
			}
		}

		for (int i = 0; i < globals::smoke_info.size(); i++) {
			grenade_info_t cur_smoke = globals::smoke_info[i];
			vec2 position;

			if (cur_smoke.time_to_expire - sdk::global_vars->curtime < 0 || cur_smoke.time_to_expire - sdk::global_vars->curtime > 20) {
				globals::smoke_info.erase(globals::smoke_info.begin() + i);
				continue;
			}

			if (math::world2screen(cur_smoke.position, position)) {
				std::string life_time = utils::snprintf("%.1f", cur_smoke.time_to_expire - sdk::global_vars->curtime);

				if (settings::visuals::grenades::smoke_bar) {
					float box_w = (float)fabs((position.x - 30) - (position.x + 30));
					auto width = (((box_w * (cur_smoke.time_to_expire - sdk::global_vars->curtime)) / 18.f));

					render::draw_box_filled(position.x - 30, position.y + 15, position.x + 30, position.y + 18.f, settings::visuals::grenades::color_bar_smoke_back);
					render::draw_box_filled(position.x - 30, position.y + 15, (position.x - 30) + (int)width, position.y + 18.f, settings::visuals::grenades::color_bar_smoke_main);
					if (settings::visuals::grenades::smoke_timer) {
						render::draw_text(life_time, vec2(((position.x - 30) + (int)width), position.y + 20.f), settings::visuals::grenades::color_smoke, true, true);
					}
				} else if (settings::visuals::grenades::smoke_timer) {
					render::draw_text(life_time, vec2(position.x, position.y + 10), settings::visuals::grenades::color_smoke, true, true);
				}
			}

			if (settings::visuals::grenades::smoke_radius) {
				render::draw_circle_3d(cur_smoke.position, 50, grenade_prediction::get_radius(smokegrenade), settings::visuals::grenades::color_smoke_radius);
			}
		}

		for (int i = 0; i < globals::molotov_info.size(); i++) {
			grenade_info_t cur_molotov = globals::molotov_info[i];
			vec2 position;
			if (cur_molotov.time_to_expire - sdk::global_vars->curtime < 0 || cur_molotov.time_to_expire - sdk::global_vars->curtime > 8) {
				globals::molotov_info.erase(globals::molotov_info.begin() + i);
				continue;
			}

			if (math::world2screen(cur_molotov.position, position)) {
				std::string life_time = utils::snprintf("%.1f", (float)cur_molotov.time_to_expire - sdk::global_vars->curtime);

				ImVec2 time_size = ImGui::CalcTextSize(life_time.c_str());
				render::draw_text("molotov", position, settings::visuals::grenades::color_molotov, true, true);

				if (settings::visuals::grenades::molotov_bar) {
					float box_w = (float)fabs((position.x - 30) - (position.x + 30));
					auto width = (((box_w * (cur_molotov.time_to_expire - sdk::global_vars->curtime)) / 7.f));

					render::draw_box_filled(position.x - 30, position.y + 15.f, position.x + 30, position.y + 18.f, settings::visuals::grenades::color_bar_molotov_back);
					render::draw_box_filled(position.x - 30, position.y + 15.f, (position.x - 30) + (int)width, position.y + 18.f, settings::visuals::grenades::color_bar_molotov_main);

					if (settings::visuals::grenades::molotov_timer)
					{
						render::draw_text(life_time, vec2(((position.x - 30) + (int)width), position.y + 20.f), settings::visuals::grenades::color_molotov, true, true);
					}
				} else if (settings::visuals::grenades::molotov_timer) {
					render::draw_text(life_time, vec2(position.x, position.y + 10), settings::visuals::grenades::color_molotov, true, true);
				}
			}
		}
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

		c_planted_c4* bomb = c_planted_c4::get_bomb();
		if (!bomb) return;
		float bomb_timer = bomb->c4_blow() - sdk::global_vars->curtime;
		if (bomb_timer < 0) return;

		vec2 window_pos = vec2(0, y - 430);
		static vec2 window_size(96, 15);// 3 item - 35 | 2 item - 25 | 1 item - 15

		if (settings::visuals::bomb_timer_pos == 1) {
			if (!c_planted_c4::get_bomb_player()) return;
			if (!math::world2screen(c_planted_c4::get_bomb_player()->get_abs_origin(), window_pos)) return;

			window_pos.x -= window_size.x / 2;
			window_pos.y -= window_size.y / 2;
		}

		render::draw_box_filled(window_pos, window_pos + window_size, color(0, 0, 0, 100));
		render::draw_box_filled(window_pos, window_pos + vec2(2.f, window_size.y), globals::menu_color);
		render::draw_text(bomb->bomb_defused() ? "defused" : utils::snprintf("bomb: %s", bomb_timer >= 0 ? utils::snprintf("%.3f", bomb_timer).c_str() : "0"), vec2(window_pos.x + 4, window_pos.y + 2), color(255, 255, 255, 255), false);
	
		float time = c_planted_c4::get_defuse_time(bomb);
		float bomb_damage = c_planted_c4::get_bomb_damage();

		if (bomb->bomb_defused()) {
			window_size.y = 15;
			return;
		}

		if (c_planted_c4::get_defuse_time(bomb) > 0) {
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
		} else if (c_planted_c4::get_defuse_time(bomb) <= 0) {
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

	void offscreen(c_base_player* player) {
		if (!settings::visuals::ofc::enable || !sdk::local_player) return;
		if (!player || !player->is_player() || player->life_state() != life_state::alive || (player->team_num() == sdk::local_player->team_num())) return;
		if (!c_base_player::get_spectating_player() || player == c_base_player::get_spectating_player()) return;

		qangle viewangles;
		int width, height;
		sdk::engine_client->get_view_angles(&viewangles);
		sdk::engine_client->get_screen_size(width, height);

		vec2 screen_center = vec2(width * .5f, height * .5f);

		const float angle_yaw_rad = DEG2RAD(viewangles.yaw - math::calc_angle(c_base_player::get_spectating_player()->get_eye_pos(), player->get_hitbox_pos(hitbox_pelvis)).yaw - 90);
		const float new_point_x = screen_center.x + ((((width - (settings::visuals::ofc::size * 3)) * 0.5f) * (settings::visuals::ofc::range / 100.0f)) * cos(angle_yaw_rad)) + (int)(6.0f * (((float)settings::visuals::ofc::size - 4.f) / 16.0f));
		const float new_point_y = screen_center.y + ((((height - (settings::visuals::ofc::size * 3)) * 0.5f) * (settings::visuals::ofc::range / 100.0f)) * sin(angle_yaw_rad));

		std::array<vec2, 3> points{
			vec2(new_point_x - settings::visuals::ofc::size, new_point_y - settings::visuals::ofc::size),
			vec2(new_point_x + settings::visuals::ofc::size, new_point_y),
			vec2(new_point_x - settings::visuals::ofc::size, new_point_y + settings::visuals::ofc::size)
		};

		math::rotate_triangle(points, viewangles.yaw - math::calc_angle(c_base_player::get_spectating_player()->get_eye_pos(), player->get_hitbox_pos(hitbox_pelvis)).yaw - 90);
		render::draw_triangle_filled(points, color(settings::visuals::ofc::clr, esp_player::get_player_alpha_offscreen(player->ent_index(), 100)));
		render::draw_triangle(points, color(settings::visuals::ofc::clr, esp_player::get_player_alpha_offscreen(player->ent_index())));
	}

	void draw_weapons(c_base_combat_weapon* ent) {
		if (ent->owner_entity().is_valid()) return;
		auto bbox = get_bbox(ent);
		if (bbox.right == 0 || bbox.bottom == 0) return;

		if (settings::visuals::dropped_weapon::box) {
			render::draw_box(vec2(bbox.left - 1, bbox.top + 1), vec2(bbox.right + 1, bbox.bottom - 1), color(0, 0, 0, 255));
			render::draw_box(vec2(bbox.left, bbox.top), vec2(bbox.right, bbox.bottom), settings::visuals::dropped_weapon::box_color);
			render::draw_box(vec2(bbox.left + 1, bbox.top - 1), vec2(bbox.right - 1, bbox.bottom + 1), color(0, 0, 0, 255));
		}

		std::string name = fixed_names[ent->get_item_definition_index()] == "" ? clean_item_name(ent->get_client_class()->network_name) : fixed_names[ent->get_item_definition_index()];
		if (name.empty()) return;

		float box_w = (float)fabs(bbox.right - bbox.left);
		float offsett = 3.f;
		if (settings::visuals::dropped_weapon::ammo_bar) {
			if (ent->is_gun()) {
				float width = (((box_w * std::clamp(ent->clip1(), 0, ent->get_cs_weapondata()->max_clip1)) / ent->get_cs_weapondata()->max_clip1));

				render::draw_box(bbox.left - 1.f, bbox.top + 3.f, bbox.right + 1.f, bbox.top + 7.f, settings::visuals::dropped_weapon::bar_outline);
				render::draw_box_filled(bbox.left, bbox.top + 4, bbox.right, bbox.top + 6, settings::visuals::dropped_weapon::bar_background);
				render::draw_box_filled(bbox.left, bbox.top + 4, bbox.left + (int)width, bbox.top + 6, settings::visuals::dropped_weapon::bar_main);
				if (ent->clip1() != ent->get_cs_weapondata()->max_clip1 && ent->clip1() > 0 && settings::visuals::dropped_weapon::ammo_in_bar) {
					render::draw_text(std::to_string(ent->clip1()), bbox.left + width, bbox.top + 5.f, color(255, 255, 255, 255), true, true, true);
				}
				offsett += 4.f;
			}
		}

		render::draw_text(name, (bbox.left + box_w * 0.5f), bbox.top + offsett, color(255, 255, 255, 255), true, true, false);
	}

	void entity_loop() {
		for (int i = 1; i < sdk::entity_list->get_highest_entity_index(); ++i) {
			c_base_entity* ent = c_base_entity::get_entity_by_index(i);
			if (!ent) continue;

			if (ent->is_player()) {
				c_base_player* cur_player = (c_base_player*)ent;
				if (!cur_player->is_dormant()) {
					last_view_time_offscreen[cur_player->ent_index()] = sdk::global_vars->curtime;
				}
				if (!settings::visuals::ofc::visible_check) {
					if (cur_player->is_dormant() && sdk::global_vars->curtime - last_view_time_offscreen[cur_player->ent_index()] > 2) {
						player_alpha_offscreen[cur_player->ent_index()] = utils::lerp(player_alpha_offscreen[cur_player->ent_index()], 0.f, 0.15f);
					} else if (!cur_player->is_dormant()) {
						player_alpha_offscreen[cur_player->ent_index()] = utils::lerp(player_alpha_offscreen[cur_player->ent_index()], 255.f, 0.02f);
					}
				}
				if (settings::visuals::ofc::visible_check) {
					vec2 pos;
					bool visible = math::world2screen_offscreen(cur_player->get_hitbox_pos(hitbox_pelvis), pos) && settings::visuals::ofc::visible_check;
					if (visible || cur_player->is_dormant()) {
						player_alpha_offscreen[cur_player->ent_index()] = utils::lerp(player_alpha_offscreen[cur_player->ent_index()], 0.f, 0.15f);
					} else if (!visible && !cur_player->is_dormant()) {
						player_alpha_offscreen[cur_player->ent_index()] = utils::lerp(player_alpha_offscreen[cur_player->ent_index()], 255.f, 0.02f);
					}
				}
				offscreen(cur_player);

				esp_player player = esp_player();
				if (player.begin(cur_player)) {
				}
			}
			else if (settings::visuals::dropped_weapon::enable && ent->is_weapon())
				draw_weapons((c_base_combat_weapon*)(ent));
			else if (ent->get_client_class()->class_id == class_id::c_inferno && settings::visuals::grenades::enable && settings::visuals::grenades::molotov_radius) {
				c_base_inferno* inferno = (c_base_inferno*)ent;
				if (!inferno) continue;

				render::draw_circle_3d(inferno->vec_origin(), 50, inferno->fire_count() * 10.f, settings::visuals::grenades::color_molotov_radius);
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