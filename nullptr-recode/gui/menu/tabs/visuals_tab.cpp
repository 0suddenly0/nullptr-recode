#include "../menu.h"
int esp_player_tab = 0;

int chams_player_tab = 0;
std::array<int, 3> chams_player_layer;

int chams_ragdoll_tab = 0;
std::array<int, 2> chams_ragdoll_layer;

int chams_weapon_tab = 0;
std::array<int, 2> chams_weapon_layer;

int chams_hands_layer = 0;

namespace menu {
	void visuals_tab() {
		null_gui::create_columns(2);
		switch (global_visuals_tab) {
		case 0: {
			auto settings_cur = &settings::visuals::esp::esp_items[esp_player_tab];

			null_gui::begin_group("general##child", vec2(0, 101)); {
				null_gui::horizontal(esp_player_tab, player_tabs);
				null_gui::check_box("enable", &settings_cur->enable);
				null_gui::check_box("use bind", &settings::visuals::esp::using_bind);
				null_gui::key_bind("##use_bind", &settings::visuals::esp::bind);
			}
			null_gui::end_group();

			null_gui::begin_group("player", vec2(0, 0)); {
				null_gui::check_box("visible only", &settings_cur->only_visible);
				null_gui::check_box("skeleton", &settings_cur->skeleton);
				null_gui::check_box("box", &settings_cur->box);
				null_gui::check_box("name", &settings_cur->name);
				null_gui::check_box("health bar", &settings_cur->health_bar);
				null_gui::tooltip_items("##health bar tooltip", [settings_cur]() {
					null_gui::check_box("health in bar", &settings_cur->health_in_bar);
					null_gui::check_box("color healthbased", &settings_cur->health_based);
					});

				null_gui::check_box("armour bar", &settings_cur->armor_bar);

				null_gui::tooltip_items("##armour bar tooltip", [settings_cur]() {
					null_gui::check_box("armour in bar", &settings_cur->armor_in_bar);
					});

				null_gui::check_box("weapon", &settings_cur->weapon);
				null_gui::tooltip_items("##weapon bar tooltip", [settings_cur]() {
					null_gui::check_box("ammo in bar", &settings_cur->ammo_in_bar);
					null_gui::check_box("ammo bar", &settings_cur->weapon_ammo);
					});

				std::vector<std::string> flags_c = { "scoped", "flashed", "defusing", "planting", "reloading", "bomb", "armor", "helmet" };
				std::vector<bool*> flags_b = { &settings_cur->flags_scoped, &settings_cur->flags_flashed, &settings_cur->flags_defusing, &settings_cur->flags_planting, &settings_cur->flags_reloading, &settings_cur->flags_bomb_carrier, &settings_cur->flags_armor, &settings_cur->flags_helmet };
				null_gui::multi_combo("flags", flags_c, flags_b);

			}
			null_gui::end_group();

			null_gui::next_column();

			null_gui::begin_group("colors##players_visial", vec2(0, 0)); {
				null_gui::color_edit("dormant", &settings_cur->dormant);
				null_gui::new_line();
				null_gui::color_edit("visible", &settings_cur->box_visible);
				null_gui::color_edit("invisible", &settings_cur->box_invisible);
				null_gui::new_line();
				null_gui::color_edit("skeleton visible", &settings_cur->skeleton_visible);
				null_gui::color_edit("skeleton invisible", &settings_cur->skeleton_invisible);
				null_gui::new_line();
				null_gui::color_edit("health bar outline", &settings_cur->health_bar_outline);
				null_gui::color_edit("health bar background", &settings_cur->health_bar_background);
				null_gui::color_edit("health bar main", &settings_cur->health_bar_main);
				null_gui::new_line();
				null_gui::color_edit("armor bar outline", &settings_cur->armor_bar_outline);
				null_gui::color_edit("armor bar background", &settings_cur->armor_bar_background);
				null_gui::color_edit("armor bar main", &settings_cur->armor_bar_main);
				null_gui::new_line();
				null_gui::color_edit("ammo bar outline", &settings_cur->ammo_bar_outline);
				null_gui::color_edit("ammo bar background", &settings_cur->ammo_bar_background);
				null_gui::color_edit("ammo bar main", &settings_cur->ammo_bar_main);

			}
			null_gui::end_group();
		} break;
		case 1: {
			auto settings_cur = &settings::visuals::chams::player_items[chams_player_tab].layers[chams_player_layer[chams_player_tab]];

			null_gui::begin_group("general##child", vec2(0, 127)); {
				null_gui::horizontal(chams_player_tab, player_tabs);
				null_gui::horizontal(chams_player_layer[chams_player_tab], layer_tabs, tabs_type::spac_child);
				null_gui::check_box("enable", &settings_cur->enable);
				null_gui::check_box("use bind", &settings::visuals::chams::using_bind);
				null_gui::key_bind("##use_bind", &settings::visuals::chams::bind);
			}
			null_gui::end_group();

			null_gui::begin_group("settings##players visial", vec2(0, 0)); {
				null_gui::check_box("visible only", &settings_cur->only_visible);

				null_gui::combo("type", &settings_cur->type, std::vector<std::string>{ "regular", "flat", "glow", "warfame" });

				null_gui::color_edit("color visible", &settings_cur->visible);
				null_gui::color_edit("color invisible", &settings_cur->invisible);
			}
			null_gui::end_group();
		} break;
		case 2: {
			chams_layer_settings_t* settings_ragdoll_cur = &settings::visuals::chams::ragdoll_items[chams_ragdoll_tab].layers[chams_ragdoll_layer[chams_ragdoll_tab]];
			null_gui::begin_group("general##child", vec2(0, 103)); {
				null_gui::horizontal(chams_ragdoll_tab, ragdoll_tabs);
				null_gui::horizontal(chams_ragdoll_layer[chams_ragdoll_tab], layer_tabs, tabs_type::spac_child);
				null_gui::check_box("enable##ragdoll", &settings_ragdoll_cur->enable);
			}
			null_gui::end_group();

			null_gui::begin_group("settings##players visial", vec2(0, 0)); {
				null_gui::check_box("visible only##ragdoll", &settings_ragdoll_cur->only_visible);

				null_gui::combo("type##hands", &settings_ragdoll_cur->type, std::vector<std::string>{ "regular", "flat", "glow", "warfame" });

				null_gui::color_edit("color visible##ragdoll", &settings_ragdoll_cur->visible);
				null_gui::color_edit("color invisible##ragdoll", &settings_ragdoll_cur->invisible);
			}
			null_gui::end_group();

			null_gui::next_column();

			chams_layer_settings_t* settings_hands_cur = &settings::visuals::chams::hands.layers[chams_hands_layer];
			null_gui::begin_group("hands##child", vec2(0, 138)); {
				null_gui::horizontal(chams_hands_layer, layer_tabs, tabs_type::spac_child);
				null_gui::check_box("enable##hands", &settings_hands_cur->enable);

				null_gui::combo("type##hands", &settings_hands_cur->type, std::vector<std::string>{ "regular", "flat", "glow", "warfame" });

				null_gui::color_edit("color##hands", &settings_hands_cur->visible);
			}
			null_gui::end_group();

			chams_layer_settings_t* settings_weapons_cur = &settings::visuals::chams::weapon_items[chams_weapon_tab].layers[chams_weapon_layer[chams_weapon_tab]];
			null_gui::begin_group("weapons##child", vec2(0, 164)); {
				null_gui::horizontal(chams_weapon_tab, weapon_tabs);
				null_gui::horizontal(chams_weapon_layer[chams_weapon_tab], layer_tabs, tabs_type::spac_child);
				null_gui::check_box("enable##weapons", &settings_weapons_cur->enable);

				null_gui::combo("type##weapons", &settings_weapons_cur->type, std::vector<std::string>{ "regular", "flat", "glow", "warfame" });

				null_gui::color_edit("color##weapons", &settings_weapons_cur->visible);
			}
			null_gui::end_group();
		} break;
		case 3:
			null_gui::text("soon...");
			break;
		case 4: {
			null_gui::begin_group("general"); {
				null_gui::check_box("offscreen", &settings::visuals::ofc::enable);
				null_gui::tooltip_items("##offscreen", []() {
					null_gui::color_edit("color##offscreen", &settings::visuals::ofc::clr);
					null_gui::check_box("visible check", &settings::visuals::ofc::visible_check);
					null_gui::slider_float("range##offscreen", &settings::visuals::ofc::range, 0.f, 100.f, "%.1f");
					null_gui::slider_float("size##offscreen", &settings::visuals::ofc::size, 0.f, 30.f, "%.1f");
					});
				null_gui::check_box("night mode", &settings::visuals::night_mode);
				null_gui::check_box("asus props", &settings::visuals::props::enable);
				if (settings::visuals::props::enable) {
					null_gui::tooltip_items("##asus props", []() {
						null_gui::slider_int("alpha##asus props", &settings::visuals::props::alpha, 0, 255);
						if (null_gui::button("set##asus props")) {
							settings::visuals::props::request = true;
						}
						});
				}
				null_gui::check_box("bomb timer", &settings::visuals::bomb_timer);
				null_gui::combo("bomb timer position", &settings::visuals::bomb_timer_pos, std::vector<std::string>{"screen", "bomb"});
				null_gui::check_box("damage indicator", &settings::visuals::damage_indicator::enable);
				null_gui::tooltip_items("##damage indicator settings", []() {
					null_gui::slider_float("show time##damage indicator", &settings::visuals::damage_indicator::show_time, 2.f, 10.f);
					null_gui::slider_float("speed##damage indicator", &settings::visuals::damage_indicator::speed, 0.f, 1.f, "%.2f");
					null_gui::slider_float("max position y##damage indicator", &settings::visuals::damage_indicator::max_pos_y, 0.f, 200.f);
					null_gui::slider_float("offset at kill##damage indicator", &settings::visuals::damage_indicator::offset_if_kill, 0.f, 20.f);
					null_gui::slider_float("offset at hit##damage indicator", &settings::visuals::damage_indicator::offset_hit, 0.f, 10.f);
					null_gui::slider_float("range offset x##damage indicator", &settings::visuals::damage_indicator::range_offset_x, 0.f, 30.f);
					null_gui::color_edit("kill color##damage indicator", &settings::visuals::damage_indicator::kill_color, false);
					});
				null_gui::check_box("hitmarker", &settings::visuals::hitmarker::enable);
				null_gui::tooltip_items("##hitmarker settings", []() {
					null_gui::slider_float("show time##hitmarker", &settings::visuals::hitmarker::show_time, 2.f, 10.f);
					null_gui::slider_float("size##hitmarker", &settings::visuals::hitmarker::size, 2.f, 10.f);
					null_gui::combo("hitmarker type", &settings::visuals::hitmarker::type, std::vector<std::string>{"static", "dynamic"});
					});
				null_gui::check_box("hitsound", &settings::visuals::hitsound::enable);
				null_gui::slider_float("hitsound volume", &settings::visuals::hitsound::volume, 0.1f, 1.f);
				null_gui::check_box("grenade prediction", &settings::visuals::grenade_prediction::enable);
				null_gui::tooltip_items("##grenade prediction settings", []() {
					null_gui::check_box("radius##genpred", &settings::visuals::grenade_prediction::radius);
					null_gui::color_edit("##radiuscolor_genpred", &settings::visuals::grenade_prediction::radius_color);
					null_gui::slider_float("line thickness##genpred", &settings::visuals::grenade_prediction::line_thickness, 0.f, 5.f, "%.1f");
					null_gui::slider_float("colision box size##genpred", &settings::visuals::grenade_prediction::colision_box_size, 0.f, 10.f, "%.1f");
					null_gui::slider_float("main colision box size##genpred", &settings::visuals::grenade_prediction::main_colision_box_size, 0.f, 10.f, "%.1f");
					null_gui::color_edit("line color##genpred", &settings::visuals::grenade_prediction::main);
					null_gui::color_edit("colision box color##genpred", &settings::visuals::grenade_prediction::main_box);
					null_gui::color_edit("end colision box color##genpred", &settings::visuals::grenade_prediction::end_box);
					});
				null_gui::check_box("client impact", &settings::visuals::impacts::client::enable);
				null_gui::tooltip_items("##client impact settings", []() {
					null_gui::color_edit("color##client impact", &settings::visuals::impacts::client::clr);
					null_gui::slider_int("show time##client impact", &settings::visuals::impacts::client::show_time, 1, 10);
					null_gui::slider_int("size##client impact", &settings::visuals::impacts::client::size, 1, 10);
					});
				null_gui::check_box("server impact", &settings::visuals::impacts::server::enable);
				null_gui::tooltip_items("##server impact settings", []() {
					null_gui::color_edit("color##server impact", &settings::visuals::impacts::server::clr);
					null_gui::slider_int("show time##server impact", &settings::visuals::impacts::server::show_time, 1, 10);
					null_gui::slider_int("size##server impact", &settings::visuals::impacts::server::size, 1, 10);
					});
				null_gui::check_box("sniper crosshair", &settings::visuals::sniper_crosshair);
				null_gui::check_box("spread circle", &settings::visuals::spread_circle::enable);
				null_gui::color_edit("##spread circle", &settings::visuals::spread_circle::clr);
				null_gui::check_box("dropped weapons", &settings::visuals::dropped_weapon::enable);
				null_gui::tooltip_items("##dropped weapos settings", []() {
					null_gui::check_box("box##dropped weapons", &settings::visuals::dropped_weapon::box);
					null_gui::color_edit("##box dropped weapons", &settings::visuals::dropped_weapon::box_color);
					null_gui::check_box("ammo bar##dropped weapons", &settings::visuals::dropped_weapon::ammo_bar);
					null_gui::check_box("ammo in bar##dropped weapons", &settings::visuals::dropped_weapon::ammo_in_bar);
					null_gui::color_edit("ammo bar main", &settings::visuals::dropped_weapon::bar_main);
					null_gui::color_edit("ammo bar background", &settings::visuals::dropped_weapon::bar_background);
					null_gui::color_edit("ammo bar outlien", &settings::visuals::dropped_weapon::bar_outline);
					});
				null_gui::check_box("enable logs##logs", &settings::visuals::logs::enable);
				null_gui::tooltip_items("##log settings", []() {
					null_gui::check_box("use custom color for hurt", &settings::visuals::logs::hurt.using_custom_color);
					null_gui::color_edit("##color hit log", &settings::visuals::logs::hurt.custm_color);
					null_gui::check_box("use custom color for buy", &settings::visuals::logs::player_buy.using_custom_color);
					null_gui::color_edit("##color buy log", &settings::visuals::logs::player_buy.custm_color);
					null_gui::check_box("use custom color for planting", &settings::visuals::logs::planting.using_custom_color);
					null_gui::color_edit("##color planting log", &settings::visuals::logs::planting.custm_color);
					null_gui::check_box("use custom color for defusing", &settings::visuals::logs::defusing.using_custom_color);
					null_gui::color_edit("##color defusing log", &settings::visuals::logs::defusing.custm_color);
					null_gui::check_box("use custom color for configs", &settings::visuals::logs::config.using_custom_color);
					null_gui::color_edit("##color configs log", &settings::visuals::logs::config.custm_color);
					null_gui::check_box("use custom color for scripts", &settings::visuals::logs::lua.using_custom_color);
					null_gui::color_edit("##color scripts log", &settings::visuals::logs::lua.custm_color);
					});
				null_gui::multi_combo("show on screen##scripts log", std::vector<std::string>{ "hit", "buy", "planting", "defusing", "configs", "scripts" }, std::vector<bool*>{ &settings::visuals::logs::hurt.enable_screen, & settings::visuals::logs::player_buy.enable_screen, & settings::visuals::logs::planting.enable_screen, & settings::visuals::logs::defusing.enable_screen, & settings::visuals::logs::config.enable_screen, & settings::visuals::logs::lua.enable_screen });
				null_gui::multi_combo("show in console##scripts log", std::vector<std::string>{ "hit", "buy", "planting", "defusing", "configs", "scripts" }, std::vector<bool*>{ &settings::visuals::logs::hurt.enable_console, & settings::visuals::logs::player_buy.enable_console, & settings::visuals::logs::planting.enable_console, & settings::visuals::logs::defusing.enable_console, & settings::visuals::logs::config.enable_console, & settings::visuals::logs::lua.enable_console });
			}
			null_gui::end_group();

			null_gui::next_column();

			null_gui::begin_group("grenades", vec2(0, 239)); {
				static int selected_grenade;

				null_gui::check_box("enable##grenades", &settings::visuals::grenades::enable);
				null_gui::combo("grenade", &selected_grenade, std::vector<std::string>{ "smoke", "flahsbang", "he grenade", "decoy", "tactical", "molotov"});

				switch (selected_grenade)
				{
				case 0:
					null_gui::color_edit("name##smoke", &settings::visuals::grenades::color_smoke);
					null_gui::new_line();
					null_gui::multi_combo("time display type###smoke", std::vector<std::string>{"bar", "text", "radius"}, std::vector<bool*>{ &settings::visuals::grenades::smoke_bar, & settings::visuals::grenades::smoke_timer, & settings::visuals::grenades::smoke_radius });
					if (settings::visuals::grenades::smoke_bar) {
						null_gui::color_edit("bar main##smoke", &settings::visuals::grenades::color_bar_smoke_main);
						null_gui::color_edit("bar back##smoke", &settings::visuals::grenades::color_bar_smoke_back);
						null_gui::color_edit("radius##smoke", &settings::visuals::grenades::color_smoke_radius);
					}
					break;
				case 1:
					null_gui::color_edit("name##flashbang", &settings::visuals::grenades::color_flash);
					break;
				case 2:
					null_gui::color_edit("name##hegrenade", &settings::visuals::grenades::color_frag);
					break;
				case 3:
					null_gui::color_edit("name##decoy", &settings::visuals::grenades::color_decoy);
					break;
				case 4:
					null_gui::color_edit("name##tactical", &settings::visuals::grenades::color_tactical);
					break;
				case 5:
					null_gui::color_edit("name##molotov", &settings::visuals::grenades::color_molotov);
					null_gui::new_line();
					null_gui::multi_combo("time display type##molotov", std::vector<std::string>{"bar", "text", "radius"}, std::vector<bool*>{ &settings::visuals::grenades::molotov_bar, & settings::visuals::grenades::molotov_timer, & settings::visuals::grenades::molotov_radius });
					if (settings::visuals::grenades::molotov_bar) {
						null_gui::color_edit("bar main##molotov", &settings::visuals::grenades::color_bar_molotov_main);
						null_gui::color_edit("bar back##molotov", &settings::visuals::grenades::color_bar_molotov_back);
						null_gui::color_edit("radius##molotov", &settings::visuals::grenades::color_molotov_radius);
					}
					break;
				}
			}
			null_gui::end_group();

			null_gui::begin_group("fog##visuals", vec2(0, 111)); {
				null_gui::check_box("enable##fog", &settings::misc::fog::enable);
				null_gui::color_edit("##color_fog", &settings::misc::fog::clr);

				float max_min = settings::misc::fog::end_dist - 10;

				max_min = std::clamp(max_min, 0.f, settings::misc::fog::end_dist);

				null_gui::slider_float("fog start dist", &settings::misc::fog::start_dist, 0.f, max_min, "%.0f");
				null_gui::slider_float("fog end dist", &settings::misc::fog::end_dist, 0.f, 10000.f, "%.0f");
			}
			null_gui::end_group();

			null_gui::begin_group("draw hitbox", vec2(0, 105)); {
				null_gui::check_box("enable##hitbox", &settings::visuals::hitbox::enable);
				null_gui::color_edit("##color hitbox", &settings::visuals::hitbox::clr);
				null_gui::slider_float("show time", &settings::visuals::hitbox::show_time, 0.f, 10.f, "%.1f");
				null_gui::check_box("show only when kill", &settings::visuals::hitbox::show_only_kill);
			}
			null_gui::end_group();
		} break;
		}
	}
}