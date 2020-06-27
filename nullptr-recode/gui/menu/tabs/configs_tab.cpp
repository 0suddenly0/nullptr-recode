#include "../menu.h"
#include "../../../helpers/config_sys/config_sys.h"

static bool is_configs_loaded = false;
static bool is_configs_loaded_skin = false;
static std::string current_config;
static std::string current_config_skin;
static bool load_request_skin = false;
static bool load_request = false;
static bool save_request_skin = false;
static bool save_request = false;
static bool del_request_skin = false;
static bool del_request = false;

namespace file_sys = std::experimental::filesystem;

int w = 0;
int h = 0;

std::vector<std::string> get_configs(std::string cfg_dir) {
	std::vector<std::string> items = {};

	std::string path(utils::snprintf("C:\\nullptr\\%s\\", cfg_dir.c_str()).c_str());
	if (!file_sys::is_directory(path))
		file_sys::create_directories(path);

	for (auto& p : file_sys::directory_iterator(path)) {
		if (strstr(p.path().string().substr(path.length()).c_str(), ".null")) {
			std::string s = p.path().string().substr(path.length());

			items.push_back(s.substr(0, s.length() - 5));
		}
	}

	return items;
}

bool check_config_load(bool skin) {
	sdk::engine_client->get_screen_size(w, h);

	null_gui::deep::set_next_window_pos(vec2((w / 2) - (164 / 2), ((h / 2) - (54 / 2))));
	null_gui::deep::set_next_window_focus();

	null_gui::begin_window("are u sure?", nullptr, vec2(164, 54), ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse); {
		if (null_gui::button("yes", false, vec2(70, 0), false)) {
			if (!skin) {
				if (!current_config.empty())
				{
					if (config::standart::load(current_config))
						notify::add("config", utils::snprintf("%s loaded", current_config.c_str()), settings::visuals::logs::config);
					menu::temp_clantag = settings::misc::clantag::clantag.c_str();
					settings::misc::clantag::clantag_visible = menu::temp_clantag;
				}

				load_request = false;
			} else {
				if (!current_config_skin.empty())
				{
					if(config::skins::load(current_config_skin)) 
						notify::add("config", utils::snprintf("%s loaded", current_config_skin.c_str()), settings::visuals::logs::config);
				}

				load_request_skin = false;
			}

			null_gui::end_window();
			return true;
		}

		null_gui::same_line();

		if (null_gui::button("no", false, vec2(70, 0), false)) {
			if (!skin) load_request = false;
			else load_request_skin = false;

			null_gui::end_window();
			return false;
		}

		null_gui::end_window();
	}
}

bool check_config_save(bool skin) {
	sdk::engine_client->get_screen_size(w, h);

	null_gui::deep::set_next_window_pos(vec2((w / 2) - (164 / 2), ((h / 2) - (54 / 2))));
	null_gui::deep::set_next_window_focus();

	null_gui::begin_window("are u sure?", nullptr, vec2(164, 54), ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse); {
		if (null_gui::button("yes", false, vec2(70, 0), false)) {
			if (!skin) {
				if (!current_config.empty()) {
					if(config::standart::save(current_config))
						notify::add("config", utils::snprintf("%s saved", current_config.c_str()), settings::visuals::logs::config);
				}

				save_request = false;
			} else {
				if (!current_config_skin.empty())
				{
					if(config::skins::save(current_config_skin))
						notify::add("config", utils::snprintf("%s saved", current_config_skin.c_str()), settings::visuals::logs::config);
				}

				save_request_skin = false;
			}

			null_gui::end_window();
			return true;
		}

		null_gui::same_line();

		if (null_gui::button("no", false, vec2(70, 0), false)) {
			if (!skin) save_request = false;
			else save_request_skin = false;

			null_gui::end_window();
			return false;
		}

		null_gui::end_window();
	}
}

bool check_config_del(bool skin) {
	sdk::engine_client->get_screen_size(w, h);

	null_gui::deep::set_next_window_pos(vec2((w / 2) - (164 / 2), ((h / 2) - (54 / 2))));
	null_gui::deep::set_next_window_focus();

	null_gui::begin_window("are u sure?", nullptr, vec2(164, 54), ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse); {
		if (null_gui::button("yes", false, vec2(70, 0), false)) {
			if (!skin) {
				if (!current_config.empty()) {
					if(file_sys::remove("C:\\nullptr\\standart\\" + current_config + ".null"))
						notify::add("config", utils::snprintf("%s deleted", current_config.c_str()), settings::visuals::logs::config);
					current_config.clear();
					is_configs_loaded = false;
				}

				del_request = false;
			} else {
				if (!current_config_skin.empty()) {
					if (file_sys::remove("C:\\nullptr\\skins\\" + current_config_skin + ".null"))
						notify::add("config", utils::snprintf("%s deleted", current_config_skin.c_str()), settings::visuals::logs::config);
					current_config_skin.clear();
					is_configs_loaded_skin = false;
				}

				del_request_skin = false;
			}

			null_gui::end_window();
			return true;
		}

		null_gui::same_line();

		if (null_gui::button("no", false, vec2(70, 0), false)) {
			if (!skin) del_request = false;
			else del_request_skin = false;

			null_gui::end_window();
			return false;
		}

		null_gui::end_window();
	}
}

namespace menu {
	void configs_tab() {
		bool can_open = !load_request && !load_request_skin && !save_request && !save_request_skin && !del_request && !del_request_skin;

		null_gui::create_columns(2);
		null_gui::begin_group("configs##configs"); {
			static std::vector<std::string> configs;

			if (!is_configs_loaded) {
				is_configs_loaded = true;
				configs = get_configs("standart");
			}

			static std::string config_name;

			null_gui::text_input("config name", &config_name);
			if (null_gui::button("create", true)) {
				current_config = config_name;

				if(config::standart::save(current_config))
					notify::add("config", utils::snprintf("%s created", current_config.c_str()), settings::visuals::logs::config);
				is_configs_loaded = false;
			}


			if(null_gui::begin_list_box("configs##config_list", vec2(0, 278))) {
				for (std::string& cur_config : configs) {
					if (null_gui::selectable(cur_config.c_str(), cur_config == current_config)) {
						current_config = cur_config;
					}

					if (cur_config == current_config) {
						null_gui::tooltip_on_click("##config rename", [can_open, &cur_config]() {
							static std::string config_rename;
							null_gui::text_input("new name", &config_rename);

							if (null_gui::button("rename", false, vec2(91, 0))) {
								if (!cur_config.empty()) {
									std::string old_name = "C:\\nullptr\\standart\\" + cur_config + ".null";
									std::string new_name = "C:\\nullptr\\standart\\" + config_rename + ".null";

									if(rename(old_name.c_str(), new_name.c_str()) == 0)
										notify::add("config", utils::snprintf("%s renamed to %s", cur_config.c_str(), config_rename.c_str()), settings::visuals::logs::config);

									cur_config = config_rename;

									is_configs_loaded = false;
								}
							}

							null_gui::same_line();

							if (null_gui::button("delete", false, vec2(91, 0))) {
								if (can_open) del_request = true;
							}
							});
					}
				}
				null_gui::end_list_box();
			}

			if (null_gui::button("refresh", true)) 
				is_configs_loaded = false;

			if (null_gui::button("load", false, vec2(111, 0))) {
				if (can_open) load_request = true;
			}

			null_gui::same_line();

			if (null_gui::button("save", false, vec2(111, 0))) {
				if (can_open) save_request = true;
			}

			if (load_request) check_config_load(false);
			if (save_request) check_config_save(false);
			if (del_request) check_config_del(false);
		}
		null_gui::end_group();

		null_gui::next_column();

		null_gui::begin_group("config skins##configs"); {
			static std::vector<std::string> configs;

			if (!is_configs_loaded_skin) {
				is_configs_loaded_skin = true;
				configs = get_configs("skins");
			}

			static std::string config_name;

			null_gui::text_input("config name##skins", &config_name);
			if (null_gui::button("create##skins", true)) {
				current_config_skin = config_name;

				if(config::skins::save(current_config_skin))
					notify::add("config", utils::snprintf("%s created", current_config_skin.c_str()), settings::visuals::logs::config);
				is_configs_loaded_skin = false;
			}

			null_gui::begin_list_box("configs##skins", vec2(0, 278)); {
				for (auto& cur_config : configs) {
					if (null_gui::selectable(cur_config.c_str(), cur_config == current_config_skin)) {
						current_config_skin = cur_config;
					}

					if (cur_config == current_config_skin) {
						null_gui::tooltip_on_click("##config rename", [can_open, &cur_config]() {
							static std::string config_rename;
							null_gui::text_input("new name", &config_rename);

							if (null_gui::button("rename", false, vec2(91, 0))) {
								if (!cur_config.empty()) {
									std::string old_name = "C:\\nullptr\\skins\\" + cur_config + ".null";
									std::string new_name = "C:\\nullptr\\skins\\" + config_rename + ".null";

									if (rename(old_name.c_str(), new_name.c_str()) == 0)
										notify::add("config", utils::snprintf("%s renamed to %s", cur_config.c_str(), config_rename.c_str()), settings::visuals::logs::config);

									cur_config = config_rename;

									is_configs_loaded_skin = false;
								}
							}

							null_gui::same_line();

							if (null_gui::button("delete", false, vec2(91, 0))) {
								if (can_open) del_request_skin = true;
							}
							});
					}
				}
			}
			null_gui::end_list_box();

			if (null_gui::button("refresh##skins", true))
				is_configs_loaded_skin = false;

			if (null_gui::button("load##skins", false, vec2(111, 0))) {
				if (can_open) load_request_skin = true;
			}

			null_gui::same_line();

			if (null_gui::button("save##skins", false, vec2(111, 0))) {
				if (can_open) save_request_skin = true;
			}

			if (load_request_skin) check_config_load(true);
			if (save_request_skin) check_config_save(true);
			if (del_request_skin) check_config_del(true);
		}
		null_gui::end_group();
	}
}