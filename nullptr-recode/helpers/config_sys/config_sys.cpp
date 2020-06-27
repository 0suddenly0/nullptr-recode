#include "config_sys.h"
#include "../../settings/settings.h"
#include "../../gui/imgui/imgui_internal.h"
#include "../../functions/notify/notify.h"
//#include "../features/changers/skin/item_definitions.h"

char line_splited = '/';

namespace config {
	namespace c_utils {
		std::string split_line(std::string temp) {
			std::string new_t = "";

			for (int g = 0; g < temp.length(); g++) {
				if (temp[g] != line_splited) {
					new_t += temp[g];
				} else {
					return new_t;
				}
			}
		}

		void add_var(int& cur_line, int& cur_group, std::vector<std::string>& config, std::vector<window_v>& vars, std::string name, std::string wind, float& x, float& y) {
			add_space(cur_group, config);

			std::string full = name + " = x-" + std::to_string((int)x) + " y-" + std::to_string((int)y) + ";";

			vars.push_back(window_v{ full, name + " = ", wind, x, y, cur_line, cur_group });
			config.push_back(name + " = x-" + std::to_string((int)x) + " y-" + std::to_string((int)y) + ";" + "\n");

			cur_line++;
		}
		void add_var(int& cur_line, int& cur_group, std::vector<std::string>& config, std::vector<string_v>& vars, std::string name, std::string& var) {
			add_space(cur_group, config);

			std::string full = name + " = '" + var + "';";

			vars.push_back(string_v{ full, name + " = ", var, cur_line, cur_group });
			config.push_back(name + " = '" + var + "';" + "\n");

			cur_line++;
		}
		void add_var(int& cur_line, int& cur_group, std::vector<std::string>& config, std::vector<int_v>& vars, std::string name, int& var) {
			add_space(cur_group, config);

			std::string full = name + " = " + std::to_string(var) + ";";

			vars.push_back(int_v{ full, name + " = ", var, cur_line, cur_group });
			config.push_back(name + " = " + std::to_string(var) + ";" + "\n");

			cur_line++;
		}
		void add_var(int& cur_line, int& cur_group, std::vector<std::string>& config, std::vector<float_v>& vars, std::string name, float& var) {
			add_space(cur_group, config);

			std::string full = name + " = " + std::to_string(var) + ";";

			vars.push_back(float_v{ full, name + " = ", var, cur_line, cur_group });
			config.push_back(name + " = " + std::to_string(var) + ";" + "\n");

			cur_line++;
		}
		void add_var(int& cur_line, int& cur_group, std::vector<std::string>& config, std::vector<bind_v>& vars, std::string name, key_bind_t& var) {
			add_space(cur_group, config);

			std::string full = name + " = " + std::to_string(var.key_id) + "-" + std::to_string(var.bind_type) + "-" + (var.enable ? "true" : "false") + ";";

			vars.push_back(bind_v{ full, name + " = ", var, cur_line, cur_group });
			config.push_back(name + " = " + std::to_string(var.key_id) + "-" + std::to_string(var.bind_type) + "-" + (var.enable ? "true" : "false") + ";" + "\n");

			cur_line++;
		}
		void add_var(int& cur_line, int& cur_group, std::vector<std::string>& config, std::vector<bool_v>& vars, std::string name, bool& var) {
			add_space(cur_group, config);

			std::string full = name + " = " + (var ? "true" : "false") + ";";

			vars.push_back(bool_v{ full, name + " = ", var, cur_line, cur_group });
			config.push_back(name + " = " + (var ? "true" : "false") + ";" + "\n");

			cur_line++;
		}
		void add_var(int& cur_line, int& cur_group, std::vector<std::string>& config, std::vector<color_v>& vars, std::string name, color& var) {
			add_space(cur_group, config);

			std::string full = utils::snprintf("%s = %i-%i-%i", name.c_str(), var.color_char[0], var.color_char[1], var.color_char[2], var.color_char[3]);

			vars.push_back(color_v{ full, utils::snprintf("%s = ", name.c_str()), var, cur_line, cur_group });
			config.push_back(utils::snprintf("%s;\n", full.c_str()));

			cur_line++;
		}

		void add_space(int& cur_group, std::vector<std::string>& config) {
			for (int i = 0; i < cur_group; i++) {
				std::string text = "| ";

				if (i == cur_group - 1) text = "|- ";
				else text = "|  ";

				config.push_back(text);
			}
		}

		void add_group(int& cur_line, int& cur_group, std::vector<std::string>& config, std::string name) {
			add_space(cur_group, config);

			config.push_back("[ " + name + " ]" + "\n");

			cur_group++;
			cur_line++;
		}

		void end_group(int& cur_group) {
			cur_group--;
		}

		bool save(std::string path, std::vector<std::string>& config, std::string name) {
			if (name.empty()) return false;

			CreateDirectoryA((LPCSTR)std::string("C:\\nullptr\\" + path + "\\").c_str(), NULL);

			std::ofstream out;
			out.open("C:\\nullptr\\" + path + "\\" + name + ".null");
			if (out.is_open()) {
				for (int i = 0; i < config.size(); i++) {
					out << config[i];
				}
			}
			out.close();

			return true;
		}

		bool load(std::string path, std::string name, std::vector<int_v>& ints, std::vector<bool_v>& bools, std::vector<float_v>& floats, std::vector<color_v>& colors, std::vector<bind_v>& binds, std::vector<string_v>& strings, std::vector<window_v>& windows) {
			if (name.empty()) return false;

			CreateDirectoryA((LPCSTR)std::string("C:\\nullptr\\" + path + "\\").c_str(), NULL);

			std::string line;

			std::string config = "";

			std::ifstream in("C:\\nullptr\\" + path + "\\" + name + ".null");

			if (in.is_open()) {
				while (getline(in, line)) {
					config += line;
					config += line_splited;
				}
			}
			in.close();


			for (int i = 0; i < bools.size(); i++) {
				if (strstr(config.c_str(), bools[i].text.c_str())) {
					std::string temp = config;
					temp.erase(0, temp.find(bools[i].text));

					std::string new_t = c_utils::split_line(temp);
					new_t.erase(0, bools[i].text.length());
					new_t.erase(new_t.length() - 1, 1);

					bools[i].v = (new_t == "true" ? true : false);
				}
			}

			for (int i = 0; i < ints.size(); i++) {
				if (strstr(config.c_str(), ints[i].text.c_str())) {
					std::string temp = config;
					temp.erase(0, temp.find(ints[i].text));

					std::string new_t = c_utils::split_line(temp);
					new_t.erase(0, ints[i].text.length());
					new_t.erase(new_t.length() - 1, 1);

					ints[i].v = std::stoi(new_t);

				}
			}

			for (int i = 0; i < floats.size(); i++) {
				if (strstr(config.c_str(), floats[i].text.c_str())) {
					std::string temp = config;
					temp.erase(0, temp.find(floats[i].text));

					std::string new_t = c_utils::split_line(temp);
					new_t.erase(0, floats[i].text.length());
					new_t.erase(new_t.length() - 1, 1);

					floats[i].v = std::stof(new_t);
				}
			}

			for (int i = 0; i < strings.size(); i++) {
				if (strstr(config.c_str(), strings[i].text.c_str())) {
					std::string temp = config;
					temp.erase(0, temp.find(strings[i].text));

					std::string new_t = c_utils::split_line(temp);
					new_t.erase(0, strings[i].text.length() + 1);
					new_t.erase(new_t.length() - 2, 2);

					strings[i].v = new_t;
				}
			}

			for (int i = 0; i < colors.size(); i++) {
				if (strstr(config.c_str(), colors[i].text.c_str())) {
					std::string temp = config;
					temp.erase(0, temp.find(colors[i].text));

					std::string new_t = c_utils::split_line(temp);
					new_t.erase(0, colors[i].text.length());

					std::string color = "";

					char split = '-';
					char endl = ';';

					float col[4] = { 0, 0, 0, 0 };

					int h = 0;

					for (int g = 0; g < new_t.length(); g++) {

						if (new_t[g] != split && new_t[g] != endl) {
							color += new_t[g];
						} else {
							col[h] = std::stof(color);
							color = "";
							h++;
						}
					}

					colors[i].v.color_char[0] = col[0];
					colors[i].v.color_char[1] = col[1];
					colors[i].v.color_char[2] = col[2];
					colors[i].v.color_char[3] = col[3];

					colors[i].v.color_float[0] = col[0] / 255.f;
					colors[i].v.color_float[1] = col[1] / 255.f;
					colors[i].v.color_float[2] = col[2] / 255.f;
					colors[i].v.color_float[3] = col[3] / 255.f;
				}
			}

			for (int i = 0; i < binds.size(); i++) {
				if (strstr(config.c_str(), binds[i].text.c_str())) {
					std::string temp = config;
					temp.erase(0, temp.find(binds[i].text));

					std::string new_t = c_utils::split_line(temp);
					new_t.erase(0, binds[i].text.length());

					std::string value = "";

					int h = 0;

					char split = '-';
					char endl = ';';

					for (int g = 0; g < new_t.length(); g++) {
						if (new_t[g] != split && new_t[g] != endl) {
							value += new_t[g];
						} else {
							switch (h) {
							case 0: binds[i].v.key_id = std::stoi(value);
							case 1: binds[i].v.bind_type = std::stoi(value);
							case 2: binds[i].v.enable = (value == "true" ? true : false);
							}

							value = "";
							h++;
						}
					}
				}
			}

			for (int i = 0; i < windows.size(); i++) {
				if (strstr(config.c_str(), windows[i].text.c_str())) {
					std::string temp = config;
					temp.erase(0, temp.find(windows[i].text));

					std::string new_t = c_utils::split_line(temp);
					new_t.erase(0, windows[i].text.length() + 2);

					ImVec2 local_pos = ImVec2(0, 0);

					char split = ' ';
					char endl = ';';

					std::string value = "";

					for (int g = 0; g < new_t.length(); g++) {
						if (new_t[g] != split && new_t[g] != endl) {
							value += new_t[g];
						} else if (new_t[g] == split) {
							local_pos.x = std::stof(value);
							g += 2;

							value = "";
						} else if (new_t[g] == endl) {
							local_pos.y = std::stof(value);

							value = "";
						}
					}

					ImGui::SetWindowPos(windows[i].window_name.c_str(), local_pos);
				}

			}

			return true;
		}
	}

	namespace standart {
		void clear() {
			config.clear();

			ints.clear();
			bools.clear();
			binds.clear();
			floats.clear();
			colors.clear();
			strings.clear();
			windows.clear();

			cur_line = 0;
			cur_group = 0;
		}

		void setup_set() {
			clear();

			add_group("visuals"); {
				add_group("general"); {
					add_group("misc"); {
						add_var("[sniper crosshair] enable", settings::misc::sniper_crosshair);

						add_group("fog"); {

						}
						end_group();
					}
					end_group();
				}
				end_group();
			}
			end_group();

			add_group("windows"); {
				add_var("[windows] bind window show", settings::windows::bind_window_show);
				add_var("[windows] bind window pos", "bind's", settings::windows::bind_window_pos.x, settings::windows::bind_window_pos.y);
				add_var("[windows] bind window alpha", settings::windows::bind_window_alpha);
			}
			end_group();

			add_group("misc"); {
				add_group("gravity"); {
					add_var("[misc]-[gravity] slow gravity", settings::misc::inverse_gravity::enable_slow);
					add_var("[misc]-[gravity] inverse gravity", settings::misc::inverse_gravity::enable);
					add_var("[misc]-[gravity] inverse gravity value", settings::misc::inverse_gravity::value);
				}
				end_group();

				add_var("[misc] menu color", globals::menu_color);
				add_var("[misc] radar ingame", settings::visuals::ingame_radar);
				add_var("[misc] bhop", settings::misc::bhop::enable);
				add_var("[misc] auto strafer type", settings::misc::bhop::strafe_type);
				add_var("[misc] block bot", settings::misc::block_bot::enable);
				add_var("[misc] block bot bind", settings::misc::block_bot::bind);
				add_var("[misc] clantag enable", settings::misc::clantag::enable);
				add_var("[misc] clantag type", settings::misc::clantag::clantag_type);
				add_var("[misc] clantag animation type", settings::misc::clantag::animation_type);
				add_var("[misc] clantag custom type", settings::misc::clantag::custom_type);
				add_var("[misc] clantag speed", settings::misc::clantag::speed);
				add_var("[misc] clantag check on empty", settings::misc::clantag::check_empty);
				add_var("[misc] custom clantag clantag", settings::misc::clantag::clantag);
				add_group("disable"); {
					add_var("[disable] flash", settings::misc::disable_flash);
					add_var("[disable] flash alpha", settings::misc::flash_alpha);
					add_var("[disable] smoke", settings::misc::disable_smoke);
					add_var("[disable] zoom", settings::misc::disable_zoom);
					add_var("[disable] zoom border", settings::misc::disable_zoom_border);
				}
				end_group();

				add_var("[misc] edge jump", settings::misc::edge_jump::enable);
				add_var("[misc] auto duck", settings::misc::edge_jump::auto_duck);
				add_var("[misc] edge jump key", settings::misc::edge_jump::bind);
				add_var("[misc] fast duck", settings::misc::fast_duck);
				add_var("[misc] moon walk", settings::misc::moon_walk);
				add_var("[misc] reveal money", settings::misc::reveal_money);
				add_var("[misc] reveal rank", settings::misc::reveal_rank);
				add_var("[misc] third person", settings::misc::third_person::enable);
				add_var("[misc] third person dist", settings::misc::third_person::dist);
				add_var("[misc] third person key", settings::misc::third_person::bind);

				add_group("fov"); {
					add_var("[fov] override", settings::misc::viewmodel::override);
					add_var("[fov] viewmodel", settings::misc::viewmodel::viewmodel);
					add_var("[fov] x", settings::misc::viewmodel::fov_x);
					add_var("[fov] y", settings::misc::viewmodel::fov_y);
					add_var("[fov] z", settings::misc::viewmodel::fov_z);
				}
				end_group();

				add_var("[misc] watermark", settings::misc::watermark);
			}
			end_group();

			add_group("profile changer"); {
				for (auto& [id, val] : mode_names) {
					add_group(val); {
						add_var("[profile changer]-[" + val + "] rank", settings::changers::profile::profile_items[id].rank_id);
						add_var("[profile changer]-[" + val + "] wins", settings::changers::profile::profile_items[id].wins);
					}
					end_group();
				}
				add_var("[profile changer] private rank", settings::changers::profile::private_id);
				add_var("[profile changer] private xp", settings::changers::profile::private_xp);

				add_group("comments"); {
					add_var("[profile changer]-[comments] teacher", settings::changers::profile::teacher);
					add_var("[profile changer]-[comments] friendly", settings::changers::profile::friendly);
					add_var("[profile changer]-[comments] leader", settings::changers::profile::leader);
				}
				end_group();

				add_group("ban"); {
					add_var("[profile changer]-[ban] type", settings::changers::profile::ban_type);
					add_var("[profile changer]-[ban] time", settings::changers::profile::ban_time);
					add_var("[profile changer]-[ban] time type", settings::changers::profile::ban_time_type);
				}
				end_group();

			}
			end_group();

		}

		void add_var(std::string name, std::string wind, float& x, float& y) {
			c_utils::add_var(cur_line, cur_group, config, windows, name, wind, x, y);
		}
		void add_var(std::string name, std::string& var) {
			c_utils::add_var(cur_line, cur_group, config, strings, name, var);
		}
		void add_var(std::string name, int& var) {
			c_utils::add_var(cur_line, cur_group, config, ints, name, var);
		}
		void add_var(std::string name, float& var) {
			c_utils::add_var(cur_line, cur_group, config, floats, name, var);
		}
		void add_var(std::string name, key_bind_t& var) {
			c_utils::add_var(cur_line, cur_group, config, binds, name, var);
		}
		void add_var(std::string name, bool& var) {
			c_utils::add_var(cur_line, cur_group, config, bools, name, var);
		}

		void add_var(std::string name, color& var) {
			c_utils::add_var(cur_line, cur_group, config, colors, name, var);
		}

		void add_space() {
			c_utils::add_space(cur_group, config);
		}

		void add_group(std::string name) {
			c_utils::add_group(cur_line, cur_group, config, name);
		}

		void end_group() {
			c_utils::end_group(cur_group);
		}

		bool save(std::string name) {
			if (name.empty()) return false;

			setup_set();

			return c_utils::save("standart", config, name);
		}

		bool load(std::string name) {
			return c_utils::load("standart", name, ints, bools, floats, colors, binds, strings, windows);
		}
	}

	namespace skins
	{
		void clear()
		{
			config.clear();

			ints.clear();
			bools.clear();
			binds.clear();
			floats.clear();
			colors.clear();
			strings.clear();
			windows.clear();

			cur_line = 0;
			cur_group = 0;
		}

		void setup_set()
		{
			clear();

			add_group("general");
			{
				add_var("[general] show skins for selected weapon", settings::changers::skin::show_cur);
				add_var("[general] skin preview", settings::changers::skin::skin_preview);
			}
			end_group();

			add_group("skinchanger");
			{
				for (auto& val : skin_changer::deeps::weapon_names)
				{
					add_group(val.name);
					{
						auto& settings_cur = settings::changers::skin::m_items[val.definition_index];

						add_var("[" + val.name + "] paint kit", settings_cur.paint_kit_index);
						add_var("[" + val.name + "] paint kit index", settings_cur.paint_kit_vector_index);
						add_var("[" + val.name + "] selected model idx", settings_cur.definition_override_index);
						add_var("[" + val.name + "] selected model", settings_cur.definition_override_vector_index);
					}
					end_group();
				}
			}
			end_group();

			add_group("skin preview");
			{
				for (int i = 0; i < skin_changer::deeps::weapon_names.size(); i++)
				{
					if (i > 3)
					{
						add_group(skin_changer::deeps::weapon_names[i].name);
						{
							add_var("[skin preview] [" + skin_changer::deeps::weapon_names[i].name + "] name", skin_changer::deeps::weapon_names[i]._weapon_name);
							add_var("[skin preview] [" + skin_changer::deeps::weapon_names[i].name + "] name skin", skin_changer::deeps::weapon_names[i].skin_name);
						}
						end_group();
					}
				}

				add_group("ct side");
				{
					for (int i = 0; i < skin_changer::deeps::ct_knife_names.size(); i++)
					{
						if (i > 2)
						{
							add_group(skin_changer::deeps::ct_knife_names[i].name);
							{
								add_var("[ct skin preview]-[ct side]-[" + skin_changer::deeps::ct_knife_names[i].name + "] name", skin_changer::deeps::ct_knife_names[i]._weapon_name);
								add_var("[ct skin preview]-[ct side]-[" + skin_changer::deeps::ct_knife_names[i].name + "] name skin", skin_changer::deeps::ct_knife_names[i].skin_name);
							}
							end_group();
						}
					}

					for (int i = 0; i < skin_changer::deeps::ct_glove_names.size(); i++)
					{
						if (i > 2)
						{
							add_group(skin_changer::deeps::ct_glove_names[i].name);
							{
								add_var("[skin preview]-[ct side]-[" + skin_changer::deeps::ct_glove_names[i].name + "] name", skin_changer::deeps::ct_glove_names[i]._weapon_name);
								add_var("[skin preview]-[ct side]-[" + skin_changer::deeps::ct_glove_names[i].name + "] name skin", skin_changer::deeps::ct_glove_names[i].skin_name);
							}
							end_group();
						}
					}
				}
				end_group();

				add_group("t side");
				{
					for (int i = 0; i < skin_changer::deeps::t_knife_names.size(); i++)
					{
						if (i > 2)
						{
							add_group(skin_changer::deeps::t_knife_names[i].name);
							{
								add_var("[skin preview]-[t side]-[" + skin_changer::deeps::t_knife_names[i].name + "] name", skin_changer::deeps::t_knife_names[i]._weapon_name);
								add_var("[skin preview]-[t side]-[" + skin_changer::deeps::t_knife_names[i].name + "] name skin", skin_changer::deeps::t_knife_names[i].skin_name);
							}
							end_group();
						}
					}

					for (int i = 0; i < skin_changer::deeps::t_glove_names.size(); i++)
					{
						if (i > 2)
						{
							add_group(skin_changer::deeps::t_glove_names[i].name);
							{
								add_var("[skin preview]-[t side]-[" + skin_changer::deeps::t_glove_names[i].name + "] name", skin_changer::deeps::t_glove_names[i]._weapon_name);
								add_var("[skin preview]-[t side]-[" + skin_changer::deeps::t_glove_names[i].name + "] name skin", skin_changer::deeps::t_glove_names[i].skin_name);
							}
							end_group();
						}
					}
				}
				end_group();
			}
			end_group();

		}
		void add_var(std::string name, std::string wind, float& x, float& y)
		{
			c_utils::add_var(cur_line, cur_group, config, windows, name, wind, x, y);
		}
		void add_var(std::string name, std::string& var)
		{
			c_utils::add_var(cur_line, cur_group, config, strings, name, var);
		}
		void add_var(std::string name, int& var)
		{
			c_utils::add_var(cur_line, cur_group, config, ints, name, var);
		}
		void add_var(std::string name, float& var)
		{
			c_utils::add_var(cur_line, cur_group, config, floats, name, var);
		}
		void add_var(std::string name, key_bind_t& var)
		{
			c_utils::add_var(cur_line, cur_group, config, binds, name, var);
		}
		void add_var(std::string name, bool& var)
		{
			c_utils::add_var(cur_line, cur_group, config, bools, name, var);
		}

		void add_var(std::string name, color& var)
		{
			c_utils::add_var(cur_line, cur_group, config, colors, name, var);
		}

		void add_space()
		{
			c_utils::add_space(cur_group, config);
		}

		void add_group(std::string name)
		{
			c_utils::add_group(cur_line, cur_group, config, name);
		}

		void end_group()
		{
			c_utils::end_group(cur_group);
		}

		bool save(std::string name)
		{
			if (name.empty()) return false;

			setup_set();

			return c_utils::save("skins", config, name);
		}

		bool load(std::string name)
		{
			return c_utils::load("skins", name, ints, bools, floats, colors, binds, strings, windows);
		}
	}
}