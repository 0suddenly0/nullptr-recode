#include "menu.h"

char* tabs[] = { "legit aim", "visuals", "misc", "skins", "profile", "configs", "scripts"};
int current_tab = 0;

char* visual_tabs[] = { "general", "chams players", "chams world", "glow", "other" };

namespace menu {
	void draw_bind_window() {
		input::create_binds();

		bool show_window = input::binds.size() > 0 && settings::windows::bind_window_show;
		null_gui::deep::set_next_window_alpha(settings::windows::bind_window_alpha / 255.f);

		null_gui::begin_window("binds", (bool*)0, vec2(0, 0), ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse); {
			settings::windows::bind_window_pos = null_gui::deep::get_window_pos();
			null_gui::deep::set_window_hidden(!show_window);
			null_gui::deep::set_cursor_y(22);

			null_gui::create_columns(2); {
				null_gui::text_no_space("bind name");
				null_gui::set_column_width(-1, 150);
				null_gui::next_column();
				null_gui::text_no_space("state");
			}
			null_gui::create_columns(1);

			null_gui::line(185.f);

			null_gui::create_columns(2); {
				for (int i = 0; i < input::binds.size(); i++) {
					auto& cur_bind = input::binds[i];

					null_gui::text_no_space(cur_bind->name.c_str());

					null_gui::next_column();

					if (cur_bind->flag & bind_info_flags_standart) null_gui::text_no_space(cur_bind->enable ? "enable" : "disable");
					if (cur_bind->flag & bind_info_flags_side)     null_gui::text_no_space(cur_bind->enable ? "left" : "right");

					null_gui::next_column();
				}
			}
			null_gui::create_columns(1);

			null_gui::end_window();
		}
	}
	void draw_main_window() {
		if (!globals::show_menu) return;

		null_gui::deep::set_menu_color(globals::menu_color);
		null_gui::deep::set_next_window_pos((utils::get_screen_size() / 2) - vec2(600 / 2, 500 / 2));

		if (current_tab == 1) null_gui::push_style_var(ImGuiStyleVar_MainBarSize, 67.f);

		vec2 main_window_pos = vec2(0, 0);
		vec2 main_window_size = vec2(0, 0);

		null_gui::begin_window("##nullptr", (bool)0, vec2(600, 500), ImGuiWindowFlags_MenuBar); {
			null_gui::deep::get_window_pos_and_size(&main_window_pos, &main_window_size);
			null_gui::begin_menu_bar(); {
				if (current_tab == 1) null_gui::pop_style_var();

				null_gui::deep::set_cursor_y(6 + null_gui::deep::get_scroll().y);

				null_gui::push_font(render::menu_big_font); {
					null_gui::text_no_space("nullptr | ");
				}
				null_gui::pop_font();

				null_gui::same_line();

				null_gui::deep::set_cursor_y(9 + null_gui::deep::get_scroll().y);

				null_gui::push_font(render::menu_bar_font); {
					null_gui::horizontal(current_tab, tabs, tabs_type::big);
				}
				null_gui::pop_font();

				if (current_tab == 1) {
					null_gui::deep::set_cursor_pos(vec2(8.f, 40 + null_gui::deep::get_scroll().y));

					null_gui::deep::set_cursor_y(46 + null_gui::deep::get_scroll().y);

					null_gui::horizontal(global_visuals_tab, visual_tabs, tabs_type::spac);
				}

				null_gui::end_menu_bar();
			}


			switch (current_tab) {
			case 0: menu::legit_tab(); break;
			case 1: menu::visuals_tab(); break;
			case 2: menu::misc_tab(); break;
			case 3: menu::skins_tab(); break;
			case 4: menu::profile_tab(); break;
			case 5: menu::configs_tab(); break;
			case 6: menu::scripts_tab(); break;
			}
		}	
		null_gui::end_window();

		auto image = skin_changer::deeps::get_preview_image();

		if (!settings::changers::skin::skin_preview || current_tab != 3 || !settings::changers::skin::show_cur || !image) return;

		null_gui::deep::set_next_window_pos(vec2(main_window_pos.x + main_window_size.x + 10, main_window_pos.y), false);

		null_gui::begin_window("##preview skins", (bool*)0, vec2(272, 212)); {
			vec2 pos = null_gui::deep::get_window_pos();
			vec2 size = null_gui::deep::get_window_size();

			vec2 min = vec2(pos.x + 8, pos.y + 3);
			vec2 max = vec2(pos.x + size.x - (8 * 2), pos.y + size.y - (3 * 2));

			null_gui::image(image, min, max);
		}
		null_gui::end_window();

	}
}