#include "../../sdk/structures/structures.h"
#include "../../settings/settings.h"
#include "../notify/notify.h"
#include "../../helpers/input.h"
#include "../../helpers/math/math.h"
#include "events.h"

std::vector<bullet_impact_t> impacts;

class c_event : public c_game_event_listener {
private:
	virtual void fire_game_event(c_game_event* event) {
		if (strstr(event->get_name(), "player_hurt")) {
			int userid_int = event->get_int("userid");
			int attacker_int = event->get_int("attacker");
			if (!userid_int || !attacker_int) return;

			int userid_id = sdk::engine_client->get_player_for_user_id(userid_int);
			int attacker_id = sdk::engine_client->get_player_for_user_id(attacker_int);

			c_base_player* userid = (c_base_player*)sdk::entity_list->get_client_entity(userid_id);
			c_base_player* attacker = (c_base_player*)sdk::entity_list->get_client_entity(attacker_id);

			player_info_t userid_info, attacker_info;
			if (!sdk::engine_client->get_player_info(userid_id, &userid_info) || !sdk::engine_client->get_player_info(attacker_id, &attacker_info)) return;

			color clr = notify::get_log_color(settings::visuals::logs::hurt);
			bool can_show = false;
			std::vector<render::multicolor_t> items;

			if (attacker == sdk::local_player && userid != sdk::local_player) {
				visuals::draw_capsule_hitbox(userid, event->get_int("health") == 0);

				if (settings::visuals::hitsound::enable)
					sdk::engine_client->execute_client_cmd(utils::snprintf("playvol buttons\\arena_switch_press_02.wav %.1f", settings::visuals::hitsound::volume).c_str());

				vec3 hit_pos = userid->get_hitbox_pos(event_manager::get_event_bone[event->get_int("hitgroup") == 0 ? hitbox_stomach : event->get_int("hitgroup")]);
				bullet_impact_t best_impact;
				int impact_id = 0;
				best_impact.pos = hit_pos;
				float best_impact_distance = 50;

				for (int i = 0; i < impacts.size(); i++) {
					auto impact = impacts[i];
					float distance = impact.pos.dist_to(hit_pos);
					if (distance < best_impact_distance) {
						impact_id = i;
						best_impact_distance = distance;
						best_impact = impact;
					}
				}

				damage_indicator_t indicator;
				indicator.hit_pos = best_impact.pos;
				indicator.time_del = sdk::global_vars->curtime + settings::visuals::damage_indicator::show_time;
				indicator.damage = event->get_int("dmg_health");
				indicator.killed = event->get_int("health") == 0;
				indicator.init = false;
				if(settings::visuals::damage_indicator::enable)
					globals::damage_indicators.push_back(indicator);

				if(event->get_int("hitgroup") != 0)
					globals::hitmarkers.push_back(best_impact);
				globals::hitmarker_last.time_del = sdk::global_vars->curtime + settings::visuals::hitmarker::show_time;

				if(impact_id != 0) impacts.erase(impacts.begin() + impact_id);

				items = {
					render::multicolor_t{ "you hurt ", color(255, 255, 255, 255) },
					render::multicolor_t{ userid_info.szName, clr },
					render::multicolor_t{ " in the ", color(255, 255, 255, 255) },
					render::multicolor_t{ event_manager::hitbox_to_string[event->get_int("hitgroup")], clr },
					render::multicolor_t{ " for ", color(255, 255, 255, 255) },
					render::multicolor_t{ event->get_int("dmg_health"), clr },
					render::multicolor_t{ " (", color(255, 255, 255, 255) },
					render::multicolor_t{ event->get_int("health"), clr },
					render::multicolor_t{ " health remaining)", color(255, 255, 255, 255) }
				};
				can_show = true;
			} else if (userid == sdk::local_player && attacker != sdk::local_player) {
				items = {
					render::multicolor_t{ "you took ", color(255, 255, 255, 255) },
					render::multicolor_t{ event->get_int("dmg_health"), clr },
					render::multicolor_t{ " damage from ", color(255, 255, 255, 255) },
					render::multicolor_t{ attacker_info.szName, clr },
					render::multicolor_t{ " in the ", color(255, 255, 255, 255) },
					render::multicolor_t{ event_manager::hitbox_to_string[event->get_int("hitgroup")], clr },
					render::multicolor_t{ " (", color(255, 255, 255, 255) },
					render::multicolor_t{ event->get_int("health"), clr },
					render::multicolor_t{ " health remaining)", color(255, 255, 255, 255) }
				};
				can_show = true;
			}

			if (can_show) {
				notify::add("damage", items, settings::visuals::logs::hurt);
			}
		}

		if (strstr(event->get_name(), "item_purchase")) {
			int userid = event->get_int("userid");
			if (!userid) return;

			int userid_id = sdk::engine_client->get_player_for_user_id(userid);

			player_info_t userid_info;
			if (!sdk::engine_client->get_player_info(userid_id, &userid_info)) return;
			c_base_player* player = (c_base_player*)(sdk::entity_list->get_client_entity(userid_id));
			if (!sdk::local_player || !player || player->team_num() == sdk::local_player->team_num()) return;

			color clr = notify::get_log_color(settings::visuals::logs::player_buy);
			std::string weapon_name = event_manager::fixed_weapon_names_by_name[event->get_string("weapon")] != "" ? event_manager::fixed_weapon_names_by_name[event->get_string("weapon")] : event->get_string("weapon");
			std::vector<render::multicolor_t> items = {
				render::multicolor_t{ userid_info.szName, clr },
				render::multicolor_t{ " purchased a(n) ", color(255, 255, 255, 255) },
				render::multicolor_t{ weapon_name, clr }
			};

			notify::add("buy", items, settings::visuals::logs::player_buy);
		}

		if (strstr(event->get_name(), "bomb_beginplant")) {
			int userid = event->get_int("userid");
			if (!userid) return;

			int userid_id = sdk::engine_client->get_player_for_user_id(userid);

			player_info_t userid_info;
			if (!sdk::engine_client->get_player_info(userid_id, &userid_info)) return;
			c_base_player* player = (c_base_player*)(sdk::entity_list->get_client_entity(userid_id));
			if (!player || player == sdk::local_player) return;

			float dist_to_b = player->vec_origin().dist_to((*sdk::player_resource)->b_side_center());
			float dist_to_a = player->vec_origin().dist_to((*sdk::player_resource)->a_side_center());

			std::string side = dist_to_a < dist_to_b ? "A" : "B";
			color clr = notify::get_log_color(settings::visuals::logs::planting);
			std::vector<render::multicolor_t> items = {
				render::multicolor_t{ userid_info.szName, clr },
				render::multicolor_t{ " has begin planting the bomb at site ", color(255, 255, 255, 255) },
				render::multicolor_t{ side, clr }
			};

			notify::add("info", items, settings::visuals::logs::planting);
		}

		if (strstr(event->get_name(), "bomb_planted")) {
			auto userid = event->get_int("userid");
			if (!userid) return;

			auto userid_id = sdk::engine_client->get_player_for_user_id(userid);

			player_info_t userid_info;
			if (!sdk::engine_client->get_player_info(userid_id, &userid_info)) return;
			c_base_player* player = (c_base_player*)(sdk::entity_list->get_client_entity(userid_id));
			if (!player || player == sdk::local_player) return;

			float dist_to_b = player->vec_origin().dist_to((*sdk::player_resource)->b_side_center());
			float dist_to_a = player->vec_origin().dist_to((*sdk::player_resource)->a_side_center());

			std::string side = dist_to_a < dist_to_b ? "A" : "B";
			color clr = notify::get_log_color(settings::visuals::logs::planting);
			std::vector<render::multicolor_t> items = {
				render::multicolor_t{ userid_info.szName, clr },
				render::multicolor_t{ " has planted bomb at site ", color(255, 255, 255, 255) },
				render::multicolor_t{ side, clr }
			};

			notify::add("info", items, settings::visuals::logs::planting);
		}

		if (strstr(event->get_name(), "bomb_begindefuse")) {
			auto userid = event->get_int("userid");
			if (!userid) return;

			auto userid_id = sdk::engine_client->get_player_for_user_id(userid);

			player_info_t userid_info;
			if (!sdk::engine_client->get_player_info(userid_id, &userid_info)) return;
			auto player = (c_base_player*)(sdk::entity_list->get_client_entity(userid_id));
			if (!player || player == sdk::local_player) return;

			std::string kit = event->get_bool("haskit") ? " has began defusing the bomb with defuse kit" : " has began defusing the bomb without defuse kit";
			color clr = notify::get_log_color(settings::visuals::logs::defusing);
			std::vector<render::multicolor_t> items = {
				render::multicolor_t{ userid_info.szName, clr },
				render::multicolor_t{ kit, color(255, 255, 255, 255) },
			};

			notify::add("info", items, settings::visuals::logs::defusing);
		}

		if (strstr(event->get_name(), "bomb_abortdefuse")) {
			auto userid = event->get_int("userid");
			if (!userid) return;

			auto userid_id = sdk::engine_client->get_player_for_user_id(userid);

			player_info_t userid_info;
			if (!sdk::engine_client->get_player_info(userid_id, &userid_info)) return;
			auto player = (c_base_player*)(sdk::entity_list->get_client_entity(userid_id));
			if (!player || player == sdk::local_player) return;

			color clr = notify::get_log_color(settings::visuals::logs::defusing);
			std::vector<render::multicolor_t> items = {
				render::multicolor_t{ userid_info.szName, clr },
				render::multicolor_t{ " has abort defusing", color(255, 255, 255, 255) },
			};

			notify::add("info", items, settings::visuals::logs::defusing);
		}

		if (strstr(event->get_name(), "bullet_impact")) {
			c_base_player* player = (c_base_player*)sdk::entity_list->get_client_entity(sdk::engine_client->get_player_for_user_id(event->get_int("userid")));

			vec3 position(event->get_float("x"), event->get_float("y"), event->get_float("z"));
			if (player && player->is_player()) {
				if (player == sdk::local_player) {
					if (settings::visuals::impacts::server::enable) {
						vec3 size(settings::visuals::impacts::server::size, settings::visuals::impacts::server::size, settings::visuals::impacts::server::size);
						sdk::debug_overlay->add_box_overlay(position, size, size * -1, qangle(0, 0, 0), settings::visuals::impacts::server::clr, settings::visuals::impacts::server::show_time);
					}

					bullet_impact_t imp;
					imp.pos = position;
					imp.time_del = sdk::global_vars->curtime + settings::visuals::hitmarker::show_time;
					impacts.push_back(imp);
				}
			}
		}

		if (strstr(event->get_name(), "inferno_startburn"))  {
			vec3 position(event->get_float("x"), event->get_float("y"), event->get_float("z"));
			globals::molotov_info.emplace_back(grenade_info_t{ position, sdk::global_vars->curtime + 7.f });
		}

		if (strstr(event->get_name(), "smokegrenade_detonate")) {
			vec3 position(event->get_float("x"), event->get_float("y"), event->get_float("z"));
			globals::smoke_info.emplace_back(grenade_info_t{ position, sdk::global_vars->curtime + 18.f });
		}

		if (strstr(event->get_name(), "round_start") || strstr(event->get_name(), "round_prestart") || strstr(event->get_name(), "round_freeze_end")) {
			globals::molotov_info.clear();
			globals::smoke_info.clear();
			globals::damage_indicators.clear();
			globals::hitmarkers.clear();
			impacts.clear();
		}
	}

	virtual int  get_event_debug_id(void) {
		return EVENT_DEBUG_ID_INIT;
	}
}; c_event event;

namespace event_manager {
	void initialization() {
		sdk::game_events->add_listener(&event, "player_hurt", false);
		sdk::game_events->add_listener(&event, "player_death", false);
		sdk::game_events->add_listener(&event, "bullet_impact", false);
		sdk::game_events->add_listener(&event, "inferno_startburn", false);
		sdk::game_events->add_listener(&event, "round_end", false);
		sdk::game_events->add_listener(&event, "round_start", false);
		sdk::game_events->add_listener(&event, "smokegrenade_detonate", false);
		sdk::game_events->add_listener(&event, "round_prestart", false);
		sdk::game_events->add_listener(&event, "round_freeze_end", false);
		sdk::game_events->add_listener(&event, "item_purchase", false);
		sdk::game_events->add_listener(&event, "bomb_beginplant", false);
		sdk::game_events->add_listener(&event, "bomb_planted", false);
		sdk::game_events->add_listener(&event, "bomb_begindefuse", false);
		sdk::game_events->add_listener(&event, "bomb_abortdefuse", false);
	}
	void shutdown() {
		sdk::game_events->remove_listener(&event);
	}

	std::map<int, int> get_event_bone = {
		{hitbox_event::hit_head, hitboxes::hitbox_head},
		{hitbox_event::hit_left_foot, hitboxes::hitbox_right_thigh},
		{hitbox_event::hit_right_foot, hitboxes::hitbox_left_thigh},
		{hitbox_event::hit_chest, hitboxes::hitbox_upper_chest},
		{hitbox_event::hit_lower_chest, hitboxes::hitbox_lower_chest},
		{hitbox_event::hit_left_hand, hitboxes::hitbox_right_forearm},
		{hitbox_event::hit_right_hand, hitboxes::hitbox_left_forearm},
		{hitbox_event::hit_neck, hitboxes::hitbox_neck},
	};

	std::map<int, std::string> hitbox_to_string = {
		{hitbox_event::hit_head, "head"},
		{hitbox_event::hit_left_foot, "left leg"},
		{hitbox_event::hit_right_foot, "right leg"},
		{hitbox_event::hit_chest, "chest"},
		{hitbox_event::hit_lower_chest, "lower chest"},
		{hitbox_event::hit_left_hand, "left arm"},
		{hitbox_event::hit_right_hand, "right arm"},
		{hitbox_event::hit_neck, "neck"},
	};

	std::map<std::string, std::string> fixed_weapon_names_by_name = {
		{ "weapon_taser", "zeus" },
		{ "item_kevlar", "kevlar" },
		{ "item_defuser", "defuse kit" },
		{ "item_assaultsuit", "full armor" },

		{ "weapon_p250", "p250" },
		{ "weapon_tec9", "tec-9" },
		{ "weapon_cz75a", "cz75a" },
		{ "weapon_glock", "glock" },
		{ "weapon_elite", "elite" },
		{ "weapon_deagle", "deagle" },
		{ "weapon_hkp2000", "p2000" },
		{ "weapon_usp_silencer", "usp" },
		{ "weapon_revolver", "rovelver" },
		{ "weapon_fiveseven", "five-seven" },

		{ "weapon_mp9", "mp-9" },
		{ "weapon_mac10", "mac-10" },
		{ "weapon_mp7", "mp-7" },
		{ "weapon_mp5sd", "mp5-sd" },
		{ "weapon_ump45", "ump-45" },
		{ "weapon_p90", "p90" },
		{ "weapon_bizon", "bizon" },

		{ "weapon_famas", "famas" },
		{ "weapon_m4a1_silencer", "m4a1-s" },
		{ "weapon_m4a1", "m4a1" },
		{ "weapon_ssg08", "ssg08" },
		{ "weapon_aug", "aug" },
		{ "weapon_awp", "awp" },
		{ "weapon_scar20", "scar20" },
		{ "weapon_galilar", "galil" },
		{ "weapon_ak47", "ak 47" },
		{ "weapon_sg556", "sg553" },
		{ "weapon_g3sg1", "g3sg1" },

		{ "weapon_nova", "nova" },
		{ "weapon_xm1014", "xm1014" },
		{ "weapon_sawedoff", "sawedoff" },
		{ "weapon_m249", "m249" },
		{ "weapon_negev", "negev" },
		{ "weapon_mag7", "mag7" },

		{ "weapon_flashbang", "flash" },
		{ "weapon_smokegrenade", "smoke" },
		{ "weapon_molotov", "molotov" },
		{ "weapon_incgrenade", "inc" },
		{ "weapon_decoy", "decoy" },
		{ "weapon_hegrenade", "hae" },
	};
}