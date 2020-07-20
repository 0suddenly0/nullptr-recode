#include "../menu.h"

static int selected_tab_skins = 0;
char* skinchanger_tabs[] = { "general", "paint kit" };

bool this_knife(int idx) {
	return idx == item_definition_index::knife_t || idx == item_definition_index::knife;
}
bool this_glove(int idx) {
	return idx == item_definition_index::glove_ct_side || idx == item_definition_index::glove_t_side;
}
bool this_ct(int idx) {
	return idx == item_definition_index::glove_ct_side || idx == item_definition_index::knife;
}
bool this_t(int idx) {
	return idx == item_definition_index::glove_t_side || idx == item_definition_index::knife_t;
}


namespace menu {
	void skins_tab() {
		auto& entries = settings::changers::skin::m_items;
		null_gui::create_columns(2);
		null_gui::begin_group("weapon select##skin window"); {
			null_gui::begin_list_box("weapons##sjinstab", vec2(0, 370)); {
				for (int i = 0; i < skin_changer::deeps::weapon_names.size(); i++) {
					null_gui::push_style_color(ImGuiCol_Text, color(153, 153, 153, 255));
					switch (i) {
					case 0: null_gui::text_no_space("knfe"); break;
					case 2: null_gui::text_no_space("glove"); break;
					case 4: null_gui::text_no_space("pistols"); break;
					case 14: null_gui::text_no_space("semi-rifle"); break;
					case 21: null_gui::text_no_space("rifle"); break;
					case 28: null_gui::text_no_space("sniper-rifle"); break;
					case 32: null_gui::text_no_space("machingun"); break;
					case 34: null_gui::text_no_space("shotgun"); break;
					}
					null_gui::pop_style_color();

					if (null_gui::selectable(skin_changer::deeps::weapon_names[i].name.c_str(), definition_vector_index == i)) {
						definition_vector_index = i;
					}
				}
			}
			null_gui::end_list_box();

			static float next_enb_time = 0;
			float time_to_next_up = sdk::global_vars->curtime;

			if (next_enb_time - sdk::global_vars->curtime < 2.f) time_to_next_up = math::clamp(next_enb_time - sdk::global_vars->curtime, 0.f, 1.f);
			else next_enb_time = sdk::global_vars->curtime;

			if (null_gui::button(utils::snprintf("update (%.1f)", time_to_next_up).c_str(), true)) {
				if (next_enb_time < sdk::global_vars->curtime) {
					skin_changer::update();
					next_enb_time = sdk::global_vars->curtime + 1.f;
				}
			}
		}
		null_gui::end_group();

		null_gui::next_column();

		null_gui::begin_group("skin select##skin tab"); {
			auto& selected_entry = entries[skin_changer::deeps::weapon_names[definition_vector_index].definition_index];
			auto& satatt = settings::changers::skin::statrack_items[skin_changer::deeps::weapon_names[definition_vector_index].definition_index];
			selected_entry.definition_index = skin_changer::deeps::weapon_names[definition_vector_index].definition_index;
			selected_entry.definition_vector_index = definition_vector_index;

			null_gui::horizontal(selected_tab_skins, skinchanger_tabs);

			std::vector<weapon_name>& cur_knifes = this_ct(selected_entry.definition_index) ? skin_changer::deeps::ct_knife_names : skin_changer::deeps::t_knife_names;
			std::vector<weapon_name>& cur_gloves = this_ct(selected_entry.definition_index) ? skin_changer::deeps::ct_glove_names : skin_changer::deeps::t_glove_names;

			if (!this_knife(selected_entry.definition_index) && !this_glove(selected_entry.definition_index))
				skin_changer::deeps::weapon_names.at(definition_vector_index)._weapon_name = skin_changer::deeps::weapon_names_preview[definition_vector_index].name;
			else if (this_glove(selected_entry.definition_index))
				cur_gloves.at(selected_entry.definition_override_vector_index)._weapon_name = skin_changer::deeps::glove_names_preview.at(selected_entry.definition_override_vector_index).name;
			else if (this_knife(selected_entry.definition_index))
				cur_knifes.at(selected_entry.definition_override_vector_index)._weapon_name = skin_changer::deeps::knife_names_preview.at(selected_entry.definition_override_vector_index).name;

			if (selected_tab_skins == 0) {
				null_gui::check_box("skin preview", &settings::changers::skin::skin_preview);
				null_gui::check_box("stattrak##enable", &selected_entry.stat_trak);
				null_gui::int_input("seed", &selected_entry.seed);
				null_gui::int_input("stattrak", &satatt.statrack_new.counter);
				null_gui::slider_float("wear", &selected_entry.wear, FLT_MIN, 1.f, "%.10f", 5);

				if (this_knife(selected_entry.definition_index)) {
					null_gui::begin_list_box("knife##list_kinfes", vec2(0, 204)); {
						for (int i = 0; i < cur_knifes.size(); i++) {
							if (null_gui::selectable(cur_knifes[i].name.c_str(), selected_entry.definition_override_vector_index == i)) {
								selected_entry.definition_override_vector_index = i;
							}
						}

						selected_entry.definition_override_index = cur_knifes.at(selected_entry.definition_override_vector_index).definition_index;
					}
					null_gui::end_list_box();
				} else if (this_glove(selected_entry.definition_index)) {
					null_gui::begin_list_box("glove##list_gloves", vec2(0, 204)); {
						for (int i = 0; i < cur_gloves.size(); i++) {
							if (null_gui::selectable(cur_gloves[i].name.c_str(), selected_entry.definition_override_vector_index == i)) {
								selected_entry.definition_override_vector_index = i;
							}
						}

						selected_entry.definition_override_index = cur_gloves.at(selected_entry.definition_override_vector_index).definition_index;
					}
					null_gui::end_list_box();
				} else {
					selected_entry.definition_override_vector_index = 0;
				}
			} else if (selected_tab_skins == 1) {
				if (skin_parser::skins_parsed) {
					static std::string filter;

					int cur_weapidx = 0;
					if (!this_glove(selected_entry.definition_index) && !this_knife(selected_entry.definition_index))
						cur_weapidx = skin_changer::deeps::weapon_names[definition_vector_index].definition_index;
					else if (this_glove(selected_entry.definition_index))
						cur_weapidx = cur_gloves.at(selected_entry.definition_override_vector_index).definition_index;
					else if (this_knife(selected_entry.definition_index))
						cur_weapidx = cur_knifes.at(selected_entry.definition_override_vector_index).definition_index;


					null_gui::text_input("name filter [?]", &filter);
					null_gui::tooltip_on_click("##skinfilter", []() {
						null_gui::check_box("show skins for selected weapon", &settings::changers::skin::show_cur);
						});


					auto cs_weapon_name = get_cs_weapon_name(cur_weapidx);

					null_gui::begin_list_box("skins##list", vec2(0, 325)); {
						if (selected_entry.definition_index != item_definition_index::glove_t_side && selected_entry.definition_index != item_definition_index::glove_ct_side) {
							if (null_gui::selectable(" - ", selected_entry.paint_kit_index == -1)) {
								selected_entry.paint_kit_vector_index = -1;
								selected_entry.paint_kit_index = -1;

								if (!this_knife(selected_entry.definition_index)) skin_changer::deeps::weapon_names.at(definition_vector_index).skin_name = "empty";
								else cur_knifes.at(selected_entry.definition_override_vector_index).skin_name = "empty";
							}

							for (int i = 0; i < skin_parser::skins.size(); i++) {
								for (auto names : skin_parser::skins[i].weaponName) {
									std::string name = skin_parser::skins[i].name;

									if (settings::changers::skin::show_cur) {
										if (names != cs_weapon_name)continue;
									}

									if (name.find(filter) != name.npos) {
										null_gui::push_style_color(ImGuiCol_Text, skin_changer::get_color_ratiry(this_knife(selected_entry.definition_index) && settings::changers::skin::show_cur ? 6 : skin_parser::skins[i].rarity));
										if (null_gui::selectable(utils::snprintf("%s##id_%i", name.c_str(), i).c_str(), selected_entry.paint_kit_vector_index == i)) {
											selected_entry.paint_kit_vector_index = i;
											selected_entry.paint_kit_index = skin_parser::skins[selected_entry.paint_kit_vector_index].id;

											if (!this_knife(selected_entry.definition_index)) skin_changer::deeps::weapon_names.at(definition_vector_index).skin_name = skin_parser::skins[i].name_short;
											else cur_knifes.at(selected_entry.definition_override_vector_index).skin_name = skin_parser::skins[i].name_short;
										}
										null_gui::pop_style_color();
									}
								}
							}
						} else {

							if (null_gui::selectable(" - ", selected_entry.paint_kit_index == -1)) {
								selected_entry.paint_kit_vector_index = -1;
								selected_entry.paint_kit_index = -1;

								cur_gloves.at(definition_vector_index).skin_name = "empty";
							}

							for (int i = 0; i < skin_parser::gloves.size(); i++) {
								for (auto names : skin_parser::gloves[i].weaponName) {
									std::string name = skin_parser::gloves[i].name;

									if (settings::changers::skin::show_cur) {
										if (names != cs_weapon_name) continue;
									}

									if (name.find(filter) != name.npos) {
										null_gui::push_style_color(ImGuiCol_Text, skin_changer::get_color_ratiry(6));
										if (null_gui::selectable(utils::snprintf("%s##id_%i", name.c_str(), i).c_str(), selected_entry.paint_kit_vector_index == i)) {
											selected_entry.paint_kit_vector_index = i;
											selected_entry.paint_kit_index = skin_parser::gloves[selected_entry.paint_kit_vector_index].id;

											cur_gloves.at(selected_entry.definition_override_vector_index).skin_name = skin_parser::gloves[selected_entry.paint_kit_vector_index].name_short;
										}
										null_gui::pop_style_color();
									}
								}
							}
						}
					}
					null_gui::end_list_box();
				} else {
					null_gui::text("skins parsing, wait...");
				}
			}
		}
		null_gui::end_group();
	}
}