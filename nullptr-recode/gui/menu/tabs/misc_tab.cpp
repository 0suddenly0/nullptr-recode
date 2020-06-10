#include "../menu.h"

namespace menu {
	void misc_tab() {
		null_gui::create_columns(2);
		static int testr;
		null_gui::begin_group("misc", vec2(0, 300)); {

			null_gui::color_edit("menu color", &globals::menu_color);
			null_gui::check_box("watermark", &settings::misc::watermark);
			null_gui::key_bind("##watermark", &settings::misc::watermark_bind);
			null_gui::check_box("obs bypass", &settings::misc::obs_bypass);
			null_gui::check_box("bhop", &settings::misc::bhop::enable);
			null_gui::combo("auto strafe type", &settings::misc::bhop::strafe_type, std::vector<std::string>{"none", "legt", "pseudo wasd"});

			null_gui::end_group();
		}
		null_gui::begin_group("windows", vec2(0, 100)); {

			null_gui::check_box("bind window", &settings::misc::bind_window);

			null_gui::end_group();
		}
	}
}