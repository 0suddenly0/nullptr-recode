#include "../menu.h"

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
			null_gui::begin_group("fog##visuals", vec2(0, 112)); {
				null_gui::check_box("enable##fog", &settings::misc::fog::enable);
				null_gui::color_edit("##color_fog", &settings::misc::fog::clr);

				float max_min = settings::misc::fog::end_dist - 10;

				max_min = std::clamp(max_min, 0.f, settings::misc::fog::end_dist);

				null_gui::slider_float("fog start dist", &settings::misc::fog::start_dist, 0.f, max_min, "%.0f");
				null_gui::slider_float("fog end dist", &settings::misc::fog::end_dist, 0.f, 10000.f, "%.0f");
			}
			null_gui::end_group();
			break;
		}
	}
}