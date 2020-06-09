#include "menu.h"

char* tabs[] = { "legit aim", "visuals", "misc", "skins", "profile", "configs", "scripts"};
const char* test_items[] = { "salam", "nizar", "A Y F", "A Y F", "A Y F", "A Y F", "A Y F", "A Y F", "A Y F", "A Y F" };
std::vector<std::string> test_items_multi_s = { "aim", "wh", "skins" };
std::vector<bool> test_items_multi_b = { true, true, true };
key_bind_t test_bind = key_bind_t{ 0, 0, false };
std::string text_string;
int test_int = 0;

namespace menu {
	void draw() {

		if (!globals::show_menu) return;

		null_gui::set_menu_color(globals::menu_color);

		null_gui::begin_window("##nullptr", (bool)0, vec2(600, 500), ImGuiWindowFlags_NoScrollbar); {

			null_gui::set_cursor_pos(vec2(0, 0));

			null_gui::begin_group("##body", vec2(600, 41));{
				null_gui::set_cursor_pos(vec2(8, 9));

				null_gui::push_font(render::menu_big_font);{
					null_gui::text_no_space("nullptr | ");
				}
				null_gui::pop_font();

				null_gui::same_line(0);

				null_gui::horizontal(test_int, tabs);

				ImGui::Separator();
			}
			null_gui::end_group();

			null_gui::create_columns(2);

			null_gui::begin_group("test salsmalekym", vec2(0, 200)); {
				null_gui::text(std::to_string(utils::get_screen_size().x).c_str());
				null_gui::text(std::to_string(utils::get_screen_size().x).c_str());
				if (null_gui::button("test button")) {
					sdk::cvar->console_dprintf("[ ");
					sdk::cvar->console_color_printf(globals::menu_color, "salam");
					sdk::cvar->console_dprintf(" ] ");
					sdk::cvar->console_color_printf(color(255, 255, 255, 255), "nullptr recod");
					sdk::cvar->console_dprintf("\n");
					notify::add("debug", "test notify standart");
					notify::add("test notify big");
				}
				null_gui::tooltip_items("test tooltip", []() {
					null_gui::check_box("salamalekym", &globals::show_menu);
					});
				null_gui::text(text_string.c_str());
				null_gui::check_box("watermark", &settings::misc::watermark);
				null_gui::key_bind("##testbind", &test_bind, true);
				null_gui::tooltip_items("##test tooltip 1", []() {
					null_gui::check_box("test checkbox##2", &globals::show_menu);
					});
				null_gui::key_bind("testbind", &test_bind);
				null_gui::int_input("test input", &test_int);
				null_gui::tooltip("test tooltip 2");
				null_gui::slider_int("test slidetint", &test_int, 0, 10);
				null_gui::color_edit("test coloreditor", &globals::menu_color);
				null_gui::text_input("test inputtext", &text_string);
				null_gui::combo("test combo", &test_int, std::vector<std::string>{"salam", "ayf"});
				null_gui::multi_combo("test combo", test_items_multi_s, &test_items_multi_b);
			}
			null_gui::end_group();

			null_gui::begin_group("test salsmalekym 2", vec2(0, 200)); {
				null_gui::text(std::to_string(utils::get_screen_size().x).c_str());
				null_gui::text(std::to_string(utils::get_screen_size().x).c_str());
				if (null_gui::button("test button")) {
					sdk::cvar->console_dprintf("[ ");
					sdk::cvar->console_color_printf(globals::menu_color, "salam");
					sdk::cvar->console_dprintf(" ] ");
					sdk::cvar->console_color_printf(color(255, 255, 255, 255), "nullptr recod");
					sdk::cvar->console_dprintf("\n");
					notify::add("debug", "test notify standart");
					notify::add("test notify big");
				}
				null_gui::tooltip_items("test tooltip", []() {
					null_gui::check_box("salamalekym", &globals::show_menu);
					});
				null_gui::text(text_string.c_str());
				null_gui::check_box("watermark", &settings::misc::watermark);
				null_gui::key_bind("##testbind", &test_bind, true);
				null_gui::tooltip_items("##test tooltip 1", []() {
					null_gui::check_box("test checkbox##2", &globals::show_menu);
					});
				null_gui::key_bind("testbind", &test_bind);
				null_gui::int_input("test input", &test_int);
				null_gui::tooltip("test tooltip 2");
				null_gui::slider_int("test slidetint", &test_int, 0, 10);
				null_gui::color_edit("test coloreditor", &globals::menu_color);
				null_gui::text_input("test inputtext", &text_string);
				null_gui::combo("test combo", &test_int, std::vector<std::string>{"salam", "ayf"});
				null_gui::multi_combo("test combo", test_items_multi_s, &test_items_multi_b);
			}
			null_gui::end_group();

			null_gui::next_column();

			null_gui::begin_group("test salsmalekym 4", vec2(0, 200)); {
				null_gui::text(std::to_string(utils::get_screen_size().x).c_str());
				null_gui::text(std::to_string(utils::get_screen_size().x).c_str());
				if (null_gui::button("test button")) {
					sdk::cvar->console_dprintf("[ ");
					sdk::cvar->console_color_printf(globals::menu_color, "salam");
					sdk::cvar->console_dprintf(" ] ");
					sdk::cvar->console_color_printf(color(255, 255, 255, 255), "nullptr recod");
					sdk::cvar->console_dprintf("\n");
					notify::add("debug", "test notify standart");
					notify::add("test notify big");
				}
				null_gui::tooltip_items("test tooltip", []() {
					null_gui::check_box("salamalekym", &globals::show_menu);
					});
				null_gui::text(text_string.c_str());
				null_gui::check_box("watermark", &settings::misc::watermark);
				null_gui::key_bind("##testbind", &test_bind, true);
				null_gui::tooltip_items("##test tooltip 1", []() {
					null_gui::check_box("test checkbox##2", &globals::show_menu);
					});
				null_gui::key_bind("testbind", &test_bind);
				null_gui::int_input("test input", &test_int);
				null_gui::tooltip("test tooltip 2");
				null_gui::slider_int("test slidetint", &test_int, 0, 10);
				null_gui::color_edit("test coloreditor", &globals::menu_color);
				null_gui::text_input("test inputtext", &text_string);
				null_gui::combo("test combo", &test_int, std::vector<std::string>{"salam", "ayf"});
				null_gui::multi_combo("test combo", test_items_multi_s, &test_items_multi_b);
			}
			null_gui::end_group();

			null_gui::begin_group("test salsmalekym 3", vec2(0, 200)); {
				null_gui::text(std::to_string(utils::get_screen_size().x).c_str());
				null_gui::text(std::to_string(utils::get_screen_size().x).c_str());
				if (null_gui::button("test button")) {
					sdk::cvar->console_dprintf("[ ");
					sdk::cvar->console_color_printf(globals::menu_color, "salam");
					sdk::cvar->console_dprintf(" ] ");
					sdk::cvar->console_color_printf(color(255, 255, 255, 255), "nullptr recod");
					sdk::cvar->console_dprintf("\n");
					notify::add("debug", "test notify standart");
					notify::add("test notify big");
				}
				null_gui::tooltip_items("test tooltip", []() {
					null_gui::check_box("salamalekym", &globals::show_menu);
					});
				null_gui::text(text_string.c_str());
				null_gui::check_box("watermark", &settings::misc::watermark);
				null_gui::key_bind("##testbind", &test_bind, true);
				null_gui::tooltip_items("##test tooltip 1", []() {
					null_gui::check_box("test checkbox##2", &globals::show_menu);
					});
				null_gui::key_bind("testbind", &test_bind);
				null_gui::int_input("test input", &test_int);
				null_gui::tooltip("test tooltip 2");
				null_gui::slider_int("test slidetint", &test_int, 0, 10);
				null_gui::color_edit("test coloreditor", &globals::menu_color);
				null_gui::text_input("test inputtext", &text_string);
				null_gui::combo("test combo", &test_int, std::vector<std::string>{"salam", "ayf"});
				null_gui::multi_combo("test combo", test_items_multi_s, &test_items_multi_b);
			}
			null_gui::end_group();

		}	
		null_gui::end_window();
	}
}