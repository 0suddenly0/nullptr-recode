#include "../menu.h"

namespace menu {
	void legit_tab() {
		null_gui::create_columns(2);

		null_gui::begin_group("select weapon", vec2(0, 138)); {
			null_gui::horizontal(settings::legitbot::settings_type, legitbot_groups);

			switch (settings::legitbot::settings_type) {
			case 0:
				null_gui::new_line(55.f);
				break;
			case 1:
				null_gui::combo("groups##combo", &settings::legitbot::group_id, weapon_groups);
				null_gui::check_box("auto current", &settings::legitbot::auto_current);
				break;
			case 2:
				if (null_gui::combo("weapons##combo", &settings::legitbot::weapon_id_in_list, [](void* data, int32_t idx, const char** out_text) {
					std::vector<weapon_name_t>* vec = (std::vector<weapon_name_t>*)(data);
					*out_text = vec->at(idx).name;
					return true;
					}, (void*)(&weapon_names), weapon_names.size())) {
					settings::legitbot::weapon_id = weapon_names[settings::legitbot::weapon_id_in_list].definition_index;
				}
				null_gui::check_box("auto current", &settings::legitbot::auto_current);
				break;
			}

			null_gui::check_box("use bind", &settings::legitbot::using_bind);
			null_gui::key_bind("##legitbot bind", &settings::legitbot::bind);
		}
		null_gui::end_group();

		null_gui::begin_group("general", vec2(0, 0)); {
			null_gui::check_box("enabled", &legitbot_settings->enabled);
			null_gui::check_box("silent", &legitbot_settings->silent);
			null_gui::check_box("auto-pistol", &legitbot_settings->autopistol);

			null_gui::multi_combo("checks", std::vector<std::string>{ "team", "smoke", "flash" }, std::vector<bool*>{ &legitbot_settings->team_check, & legitbot_settings->smoke_check, & legitbot_settings->flash_check });

			null_gui::check_box("autowall", &legitbot_settings->autowall);
			if (legitbot_settings->autowall) {
				null_gui::slider_int("min damage", &legitbot_settings->autowall_min_damage, 0, 100);
			}
			null_gui::check_box("autofire", &legitbot_settings->autofire);
			null_gui::key_bind("##autofirebind", &legitbot_settings->autofire_bind);
			if (legitbot_settings->autofire) {
				null_gui::slider_int("chance##autofire", &legitbot_settings->autofire_chance, 0, 100, "%d%%");
			}
		}
		null_gui::end_group();

		null_gui::next_column();

		null_gui::begin_group("current settings", vec2(0, 244)); {
			null_gui::multi_combo("hitboxes", hitboxes_string, &legitbot_settings->hitboxes);
			if (legitbot_settings->silent) {
				null_gui::slider_float("silent fov", &legitbot_settings->silent_fov, 0.f, 20.f, "%.2f");
				null_gui::slider_int("silent chance", &legitbot_settings->silent_chance, 0, 100, "%d%%");
			}
			null_gui::slider_float("standart fov", &legitbot_settings->fov, 0.f, 20.f, "%.2f");
			null_gui::slider_float("smooth##smooth", &legitbot_settings->smooth, 1.f, 20.f, "%.2f");
			if (!legitbot_settings->silent)
				null_gui::slider_int("shot delay", &legitbot_settings->shot_delay, 0, 1000);
			null_gui::slider_int("kill delay", &legitbot_settings->kill_delay, 0, 1000);

			null_gui::slider_int("backtrack ticks [?]", &legitbot_settings->backtrack_ticks, 0, 12, "%d ticks");
			null_gui::tooltip_on_click("##backtrack settings", []() {
				null_gui::check_box("aim at backtrack", &legitbot_settings->aim_at_backtrack);
				null_gui::combo("priority for backtrack", &legitbot_settings->backtrack_priority, std::vector<std::string>{"low", "high"});
				});
		}
		null_gui::end_group();

		null_gui::begin_group("rcs", vec2(0, 178)); {
			null_gui::check_box("enabled##rcs", &legitbot_settings->rcs_enabled);

			null_gui::combo("type##rcs", &legitbot_settings->rcs_type, std::vector<std::string>{ "standalone", "aim" });

			null_gui::slider_int("start##start", &legitbot_settings->rcs_start, 1, 30);
			null_gui::slider_int("x", &legitbot_settings->rcs_x, 0, 100);
			null_gui::slider_int("y", &legitbot_settings->rcs_y, 0, 100);
		}
		null_gui::end_group();
	}
}