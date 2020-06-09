#include "../menu.h"

namespace menu {
	void misc_tab() {
		null_gui::create_columns(2);

		null_gui::begin_group("misc", vec2(0, 100)); {

			null_gui::color_edit("menu color", &globals::menu_color);
			null_gui::check_box("watermark", &settings::misc::watermark);

			null_gui::end_group();
		}
	}
}