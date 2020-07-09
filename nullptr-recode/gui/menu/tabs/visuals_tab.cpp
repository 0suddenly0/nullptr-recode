#include "../menu.h"

int test = 0;

namespace menu {
	void visuals_tab() {
		null_gui::create_columns(2);
		switch (global_visuals_tab)
		{
		case 0:
		case 1:
		case 2:
		case 3:
			null_gui::text("soon...");
			break;
		case 4:
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
				null_gui::multi_combo("show on screen##scripts log", std::vector<std::string>{ "hit", "buy", "planting", "defusing", "configs", "scripts" }, std::vector<bool*>{ &settings::visuals::logs::hurt.enable_screen, &settings::visuals::logs::player_buy.enable_screen, & settings::visuals::logs::planting.enable_screen, &settings::visuals::logs::defusing.enable_screen, &settings::visuals::logs::config.enable_screen, &settings::visuals::logs::lua.enable_screen });
				null_gui::multi_combo("show in console##scripts log", std::vector<std::string>{ "hit", "buy", "planting", "defusing", "configs", "scripts" }, std::vector<bool*>{ &settings::visuals::logs::hurt.enable_console, &settings::visuals::logs::player_buy.enable_console, &settings::visuals::logs::planting.enable_console, &settings::visuals::logs::defusing.enable_console, &settings::visuals::logs::config.enable_console, &settings::visuals::logs::lua.enable_console });
			}
			null_gui::end_group();

			null_gui::next_column();

			null_gui::begin_group("grenades", vec2(0, 239)); {
				static int selected_grenade;

				null_gui::check_box("enable##grenades", &settings::visuals::grenades::enable);
				null_gui::combo("grenade", &selected_grenade, std::vector<std::string>{ "smoke", "flahsbang", "he grenade", "decoy", "molotov"});

				switch (selected_grenade)
				{
				case 0:
					null_gui::color_edit("name##smoke", &settings::visuals::grenades::color_smoke);
					null_gui::new_line();
					null_gui::multi_combo("time display type###smoke", std::vector<std::string>{"bar", "text", "radius"}, std::vector<bool*>{ &settings::visuals::grenades::smoke_bar, &settings::visuals::grenades::smoke_timer, &settings::visuals::grenades::smoke_radius });
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
					null_gui::color_edit("name##molotov", &settings::visuals::grenades::color_molotov);
					null_gui::new_line();
					null_gui::multi_combo("time display type##molotov", std::vector<std::string>{"bar", "text", "radius"}, std::vector<bool*>{ &settings::visuals::grenades::molotov_bar, &settings::visuals::grenades::molotov_timer, &settings::visuals::grenades::molotov_radius });
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

			null_gui::begin_group("draw hitbox", vec2(0, 100)); {
				null_gui::check_box("enable##hitbox", &settings::visuals::hitbox::enable);
				null_gui::color_edit("##color hitbox", &settings::visuals::hitbox::clr);
				null_gui::slider_float("show time", &settings::visuals::hitbox::show_time, 0.f, 10.f, "%.1f");
				null_gui::check_box("show only when kill", &settings::visuals::hitbox::show_only_kill);
			}
			null_gui::end_group();
			break;
		}
	}
}