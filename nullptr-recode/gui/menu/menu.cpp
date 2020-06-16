#include "menu.h"

char* tabs[] = { "legit aim", "visuals", "misc", "skins", "profile", "configs", "scripts"};
int current_tab = 0;

namespace menu {
	void draw_bind_window() {
		input::create_binds();

		if (input::binds.size() <= 0 || !settings::misc::bind_window) return;

		null_gui::begin_window("bind's", (bool*)0, vec2(0, 0), ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse); {

			null_gui::create_columns(2); {
				null_gui::text_no_space("bind name");
				null_gui::set_column_width(-1, 130);
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

					if (cur_bind->flag & bind_info_flags_standart)
						null_gui::text_no_space(cur_bind->enable ? "enable" : "disable");
					if (cur_bind->flag & bind_info_flags_side)
						null_gui::text_no_space(cur_bind->enable ? "left" : "right");

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

		null_gui::begin_window("##nullptr", (bool)0, vec2(600, 500), ImGuiWindowFlags_MenuBar); {

			ImGui::BeginMenuBar(); {
				null_gui::deep::set_cursor_y(6 + null_gui::deep::get_scroll().y);

				null_gui::push_font(render::menu_big_font); {
					null_gui::text_no_space("nullptr | ");
				}
				null_gui::pop_font();

				null_gui::same_line();

				null_gui::deep::set_cursor_y(9 + null_gui::deep::get_scroll().y);

				null_gui::push_font(render::menu_bar_font); {
					null_gui::horizontal(current_tab, tabs, true);
				}
				null_gui::pop_font();

				ImGui::EndMenuBar();
			}

			/*null_gui::set_cursor_pos(vec2(0, 0));

			null_gui::begin_group("##body", vec2(600, 40));{
				null_gui::set_cursor_pos(vec2(8, 9));

				null_gui::push_font(render::menu_big_font);{
					null_gui::text_no_space("nullptr | ");
				}
				null_gui::pop_font();

				null_gui::same_line(0);

				null_gui::horizontal(current_tab, tabs);

				null_gui::set_cursor_pos(vec2(0, 39));

				ImGui::Separator();
			}
			null_gui::end_group();*/

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
	}
}