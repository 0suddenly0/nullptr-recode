#include "config_sys.h"
#include "../../settings/settings.h"
#include "../../gui/imgui/imgui_internal.h"
#include "../../functions/notify/notify.h"
//#include "../features/changers/skin/item_definitions.h"

config::c_config* current_config = new config::c_config("cur_config");
char line_splited = '/';

namespace config {
	namespace config_utils {
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

			std::string full = utils::snprintf("%s = x-%.0f y-%.0f;", name.c_str(), x, y);

			vars.push_back(window_v{ full, utils::snprintf("%s = ", name.c_str()), wind, x, y, cur_line, cur_group });
			config.push_back(utils::snprintf("%s\n", full.c_str()));

			cur_line++;
		}
		void add_var(int& cur_line, int& cur_group, std::vector<std::string>& config, std::vector<string_v>& vars, std::string name, std::string& var) {
			add_space(cur_group, config);

			std::string full = utils::snprintf("%s = '%s';", name.c_str(), var.c_str());

			vars.push_back(string_v{ full, utils::snprintf("%s = ", name.c_str()), var, cur_line, cur_group });
			config.push_back(utils::snprintf("%s\n", full.c_str()));

			cur_line++;
		}
		void add_var(int& cur_line, int& cur_group, std::vector<std::string>& config, std::vector<int_v>& vars, std::string name, int& var) {
			add_space(cur_group, config);

			std::string full = utils::snprintf("%s = %d;", name.c_str(), var);

			vars.push_back(int_v{ full, utils::snprintf("%s = ", name.c_str()), var, cur_line, cur_group });
			config.push_back(utils::snprintf("%s\n", full.c_str()));

			cur_line++;
		}
		void add_var(int& cur_line, int& cur_group, std::vector<std::string>& config, std::vector<float_v>& vars, std::string name, float& var) {
			add_space(cur_group, config);

			std::string full = utils::snprintf("%s = %.3f;", name.c_str(), var);

			vars.push_back(float_v{ full, utils::snprintf("%s = ", name.c_str()), var, cur_line, cur_group });
			config.push_back(utils::snprintf("%s\n", full.c_str()));

			cur_line++;
		}
		void add_var(int& cur_line, int& cur_group, std::vector<std::string>& config, std::vector<bind_v>& vars, std::string name, key_bind_t& var) {
			add_space(cur_group, config);

			std::string full = utils::snprintf("%s = %d-%d-%s;" , name.c_str(), var.key_id, var.bind_type, var.enable ? "true" : "false");

			vars.push_back(bind_v{ full, utils::snprintf("%s = ", name.c_str()), var, cur_line, cur_group });
			config.push_back(utils::snprintf("%s\n", full.c_str()));

			cur_line++;
		}
		void add_var(int& cur_line, int& cur_group, std::vector<std::string>& config, std::vector<bool_v>& vars, std::string name, bool& var) {
			add_space(cur_group, config);

			std::string full = utils::snprintf("%s = %s;", name.c_str(), var ? "true" : "false");

			vars.push_back(bool_v{ full, utils::snprintf("%s = ", name.c_str()), var, cur_line, cur_group });
			config.push_back(utils::snprintf("%s\n", full.c_str()));

			cur_line++;
		}
		void add_var(int& cur_line, int& cur_group, std::vector<std::string>& config, std::vector<color_v>& vars, std::string name, color& var) {
			add_space(cur_group, config);

			std::string full = utils::snprintf("%s = %d-%d-%d-%d;", name.c_str(), var.r(), var.g(), var.b(), var.a());

			vars.push_back(color_v{ full, utils::snprintf("%s = ", name.c_str()), var, cur_line, cur_group });
			config.push_back(utils::snprintf("%s\n", full.c_str()));

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

					std::string new_t = config_utils::split_line(temp);
					new_t.erase(0, bools[i].text.length());
					new_t.erase(new_t.length() - 1, 1);

					bools[i].v = (new_t == "true" ? true : false);
				}
			}

			for (int i = 0; i < ints.size(); i++) {
				if (strstr(config.c_str(), ints[i].text.c_str())) {
					std::string temp = config;
					temp.erase(0, temp.find(ints[i].text));

					std::string new_t = config_utils::split_line(temp);
					new_t.erase(0, ints[i].text.length());
					new_t.erase(new_t.length() - 1, 1);

					ints[i].v = std::stoi(new_t);

				}
			}

			for (int i = 0; i < floats.size(); i++) {
				if (strstr(config.c_str(), floats[i].text.c_str())) {
					std::string temp = config;
					temp.erase(0, temp.find(floats[i].text));

					std::string new_t = config_utils::split_line(temp);
					new_t.erase(0, floats[i].text.length());
					new_t.erase(new_t.length() - 1, 1);

					floats[i].v = std::stof(new_t);
				}
			}

			for (int i = 0; i < strings.size(); i++) {
				if (strstr(config.c_str(), strings[i].text.c_str())) {
					std::string temp = config;
					temp.erase(0, temp.find(strings[i].text));

					std::string new_t = config_utils::split_line(temp);
					new_t.erase(0, strings[i].text.length() + 1);
					new_t.erase(new_t.length() - 2, 2);

					strings[i].v = new_t;
				}
			}

			for (int i = 0; i < colors.size(); i++) {
				if (strstr(config.c_str(), colors[i].text.c_str())) {
					std::string temp = config;
					temp.erase(0, temp.find(colors[i].text));

					std::string new_t = config_utils::split_line(temp);
					new_t.erase(0, colors[i].text.length());

					std::string color = "";

					char split = '-';
					char endl = ';';

					int col[4] = { 0, 0, 0, 0 };

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

					colors[i].v = col;
				}
			}

			for (int i = 0; i < binds.size(); i++) {
				if (strstr(config.c_str(), binds[i].text.c_str())) {
					std::string temp = config;
					temp.erase(0, temp.find(binds[i].text));

					std::string new_t = config_utils::split_line(temp);
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

					std::string new_t = config_utils::split_line(temp);
					new_t.erase(0, windows[i].text.length() + 2);

					vec2 local_pos = vec2(0, 0);

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

					null_gui::deep::set_window_pos(windows[i].window_name.c_str(), local_pos, false);
				}

			}

			return true;
		}
	}

	namespace standart {
		void setup_vars() {
			push_cur_config(&_config);
			_config.clear();

			add_group("visuals"); {
				add_group("esp"); {
					for (int i = 0; i < _countof(player_tabs); i++) {
						std::string name = player_tabs[i];
						esp_settings_t& settings_cur = settings::visuals::esp::esp_items[i];

						add_group(name); {
							add_var("[esp]-[" + name + "] enable", settings_cur.enable);
							add_var("[esp]-[" + name + "] visible only", settings_cur.only_visible);
							add_var("[esp]-[" + name + "] box", settings_cur.box);
							add_var("[esp]-[" + name + "] color visible", settings_cur.box_visible);
							add_var("[esp]-[" + name + "] color invisible", settings_cur.box_invisible);
							add_var("[esp]-[" + name + "] dormant color", settings_cur.dormant);
							add_var("[esp]-[" + name + "] name", settings_cur.name);

							add_group("skeleton"); {
								add_var("[esp]-[" + name + "]-[skeleton] enable", settings_cur.skeleton);
								add_var("[esp]-[" + name + "]-[skeleton] color visible", settings_cur.skeleton_visible);
								add_var("[esp]-[" + name + "]-[skeleton] color invisible", settings_cur.skeleton_invisible);
							}
							end_group();

							add_group("health"); {
								add_var("[esp]-[" + name + "]-[health] enable", settings_cur.health_bar);
								add_var("[esp]-[" + name + "]-[health] health in bar", settings_cur.health_in_bar);
								add_var("[esp]-[" + name + "]-[health] color based of health", settings_cur.health_based);
								add_var("[esp]-[" + name + "]-[health] bar color", settings_cur.health_bar_main);
								add_var("[esp]-[" + name + "]-[health] bar outline color", settings_cur.health_bar_outline);
								add_var("[esp]-[" + name + "]-[health] bar backgroud color", settings_cur.health_bar_background);
							}
							end_group();

							add_group("armor"); {
								add_var("[esp]-[" + name + "]-[armor] enable", settings_cur.armor_bar);
								add_var("[esp]-[" + name + "]-[armor] armor in bar", settings_cur.armor_in_bar);
								add_var("[esp]-[" + name + "]-[armor] bar color", settings_cur.armor_bar_main);
								add_var("[esp]-[" + name + "]-[armor] bar outline color", settings_cur.armor_bar_outline);
								add_var("[esp]-[" + name + "]-[armor] bar backgroud color", settings_cur.armor_bar_background);
							}
							end_group();

							add_var("[esp]-[" + name + "]-[weapon] weapon", settings_cur.weapon);

							add_group("ammo"); {
								add_var("[esp]-[" + name + "]-[weapon] enable", settings_cur.weapon_ammo);
								add_var("[esp]-[" + name + "]-[weapon] text in bar", settings_cur.weapon_ammo);
								add_var("[esp]-[" + name + "]-[weapon] bar color", settings_cur.ammo_bar_main);
								add_var("[esp]-[" + name + "]-[weapon] bar outline color", settings_cur.ammo_bar_outline);
								add_var("[esp]-[" + name + "]-[weapon] bar backgroud color", settings_cur.ammo_bar_background);
							}
							end_group();

							add_group("flags"); {
								add_var("[esp]-[" + name + "]-[flags] flashed", settings_cur.flags_flashed);
								add_var("[esp]-[" + name + "]-[flags] armor", settings_cur.flags_armor);
								add_var("[esp]-[" + name + "]-[flags] have bomb", settings_cur.flags_bomb_carrier);
								add_var("[esp]-[" + name + "]-[flags] defusing", settings_cur.flags_defusing);
								add_var("[esp]-[" + name + "]-[flags] helment", settings_cur.flags_helmet);
								add_var("[esp]-[" + name + "]-[flags] planting", settings_cur.flags_planting);
								add_var("[esp]-[" + name + "]-[flags] reloading", settings_cur.flags_reloading);
								add_var("[esp]-[" + name + "]-[flags] scoped", settings_cur.flags_scoped);
							}
							end_group();
						}
						end_group();
					}
				}
				end_group();

				add_group("chams"); {
					for (int i = 0; i < _countof(player_tabs); i++) {
						std::string name = player_tabs[i];
						chams_settings_t& settings_cur = settings::visuals::chams::player_items[i];

						add_group(name); {
							for (int layer = 0; layer < _countof(layer_tabs); layer++) {
								std::string name_layer = layer_tabs[layer];
								chams_layer_settings_t& cur_layer = settings_cur.layers[layer];
								add_group(name_layer); {
									add_var("[chams]-[" + name + "]-[" + name_layer + "] enable", cur_layer.enable);
									add_var("[chams]-[" + name + "]-[" + name_layer + "] type", cur_layer.type);
									add_var("[chams]-[" + name + "]-[" + name_layer + "] visible only", cur_layer.only_visible);
									add_var("[chams]-[" + name + "]-[" + name_layer + "] visible", cur_layer.visible);
									add_var("[chams]-[" + name + "]-[" + name_layer + "] invisible", cur_layer.invisible);
								}
								end_group();
							}
						}
						end_group();
					}

					add_group("oher chams"); {
						add_group("ragdoll"); {
							for (int i = 0; i < _countof(ragdoll_tabs); i++) {
								std::string name = ragdoll_tabs[i];
								chams_settings_t& settings_cur = settings::visuals::chams::ragdoll_items[i];

								for (int layer = 0; layer < _countof(layer_tabs); layer++) {
									std::string name_layer = layer_tabs[layer];
									chams_layer_settings_t& cur_layer = settings_cur.layers[layer];
									add_group(name_layer); {
										add_var("[ragdoll]-[" + name + "]-[" + name_layer + "] enable", cur_layer.enable);
										add_var("[ragdoll]-[" + name + "]-[" + name_layer + "] type", cur_layer.type);
										add_var("[ragdoll]-[" + name + "]-[" + name_layer + "] visible only", cur_layer.only_visible);
										add_var("[ragdoll]-[" + name + "]-[" + name_layer + "] visible", cur_layer.visible);
										add_var("[ragdoll]-[" + name + "]-[" + name_layer + "] invisible", cur_layer.invisible);
									}
									end_group();
								}
							}
						}
						end_group();

						add_group("local player"); {
							add_group("weapon"); {
								for (int i = 0; i < _countof(weapon_tabs); i++) {
									std::string name = weapon_tabs[i];
									chams_settings_t& settings_cur = settings::visuals::chams::weapon_items[i];

									add_group(name); {
										for (int layer = 0; layer < _countof(layer_tabs); layer++) {
											std::string name_layer = layer_tabs[layer];
											chams_layer_settings_t& cur_layer = settings_cur.layers[layer];
											add_group(name_layer); {
												add_var("[weapon]-[" + name + "]-[" + name_layer + "] enable", cur_layer.enable);
												add_var("[weapon]-[" + name + "]-[" + name_layer + "] type", cur_layer.type);
												add_var("[weapon]-[" + name + "]-[" + name_layer + "] visible", cur_layer.visible);
											}
											end_group();
										}
									}
									end_group();
								}
							}
							end_group();

							add_group("hands"); {
								for (int layer = 0; layer < _countof(layer_tabs); layer++) {
									std::string name_layer = layer_tabs[layer];
									chams_layer_settings_t& cur_layer = settings::visuals::chams::hands.layers[layer];
									add_group(name_layer); {
										add_var("[hands]-[" + name_layer + "] enable", cur_layer.enable);
										add_var("[hands]-[" + name_layer + "] type", cur_layer.type);
										add_var("[hands]-[" + name_layer + "] visible", cur_layer.visible);
									}
									end_group();
								}
							}
							end_group();
						}
						end_group();
					}
					end_group();

				}
				end_group();

				add_group("other"); {
					add_group("offscreen"); {
						add_var("[offscreen] enable", settings::visuals::ofc::enable);
						add_var("[offscreen] color", settings::visuals::ofc::clr);
						add_var("[offscreen] visible check", settings::visuals::ofc::visible_check);
						add_var("[offscreen] range", settings::visuals::ofc::range);
						add_var("[offscreen] size", settings::visuals::ofc::size);
					}
					end_group();

					add_var("[other] night mode", settings::visuals::night_mode);
					add_var("[other] asus props", settings::visuals::props::enable);
					add_var("[other] asus props alpha", settings::visuals::props::alpha);
					add_var("[other] bomb timer", settings::visuals::bomb_timer);
					add_var("[other] bomb timer pos", settings::visuals::bomb_timer_pos);

					add_group("damage indicator"); {
						add_var("[damage indicator] enable", settings::visuals::damage_indicator::enable);
						add_var("[damage indicator] show time", settings::visuals::damage_indicator::show_time);
						add_var("[damage indicator] speed", settings::visuals::damage_indicator::speed);
						add_var("[damage indicator] max pos y", settings::visuals::damage_indicator::max_pos_y);
						add_var("[damage indicator] offset at kill", settings::visuals::damage_indicator::offset_if_kill);
						add_var("[damage indicator] offset at hit", settings::visuals::damage_indicator::offset_hit);
						add_var("[damage indicator] range offset x", settings::visuals::damage_indicator::range_offset_x);
						add_var("[damage indicator] kill color", settings::visuals::damage_indicator::kill_color);
					}
					end_group();

					add_group("hits"); {
						add_var("[hits] hitmarker enable", settings::visuals::hitmarker::enable);
						add_var("[hits] hitmarker show time", settings::visuals::hitmarker::show_time);
						add_var("[hits] hitmarker size", settings::visuals::hitmarker::size);
						add_var("[hits] hitmarker show type", settings::visuals::hitmarker::type);
						add_var("[hits] hitsound enable", settings::visuals::hitsound::enable);
						add_var("[hits] hitsound volume", settings::visuals::hitsound::volume);
					}
					end_group();

					add_group("grenade prediction"); {
						add_var("[grenade prediction] enable", settings::visuals::grenade_prediction::enable);
						add_var("[grenade prediction] radius", settings::visuals::grenade_prediction::radius);
						add_var("[grenade prediction] radius color", settings::visuals::grenade_prediction::radius_color);
						add_var("[grenade prediction] line thickness", settings::visuals::grenade_prediction::line_thickness);
						add_var("[grenade prediction] colision box size", settings::visuals::grenade_prediction::colision_box_size);
						add_var("[grenade prediction] last colision box size", settings::visuals::grenade_prediction::main_colision_box_size);
						add_var("[grenade prediction] line color", settings::visuals::grenade_prediction::main);
						add_var("[grenade prediction] colision box color", settings::visuals::grenade_prediction::main_box);
						add_var("[grenade prediction] main colision box color", settings::visuals::grenade_prediction::end_box);
					}
					end_group();

					add_group("impacts"); {
						add_group("client"); {
							add_var("[impacts]-[client] enable", settings::visuals::impacts::client::enable);
							add_var("[impacts]-[client] show time", settings::visuals::impacts::client::show_time);
							add_var("[impacts]-[client] size", settings::visuals::impacts::client::size);
							add_var("[impacts]-[client] color", settings::visuals::impacts::client::clr);
						}
						end_group();

						add_group("server"); {
							add_var("[impacts]-[server] enable", settings::visuals::impacts::server::enable);
							add_var("[impacts]-[server] show time", settings::visuals::impacts::server::show_time);
							add_var("[impacts]-[server] size", settings::visuals::impacts::server::size);
							add_var("[impacts]-[server] color", settings::visuals::impacts::server::clr);
						}
						end_group();
					}
					end_group();

					add_var("[other] sniper crosshair", settings::visuals::sniper_crosshair);
					add_var("[other] spread circle", settings::visuals::spread_circle::enable);
					add_var("[other] spread circle color", settings::visuals::spread_circle::clr);

					add_group("dropped weapons"); {
						add_var("[dropped weapons] enable", settings::visuals::dropped_weapon::enable);
						add_var("[dropped weapons] box", settings::visuals::dropped_weapon::box);
						add_var("[dropped weapons] ammo bar", settings::visuals::dropped_weapon::ammo_bar);
						add_var("[dropped weapons] ammo in bar", settings::visuals::dropped_weapon::ammo_in_bar);
						add_var("[dropped weapons] ammo bar color", settings::visuals::dropped_weapon::bar_main );
						add_var("[dropped weapons] ammo bar bg color", settings::visuals::dropped_weapon::bar_background);
						add_var("[dropped weapons] ammo bar outline color", settings::visuals::dropped_weapon::bar_outline);
						add_var("[dropped weapons] box and text color", settings::visuals::dropped_weapon::box_color);
					}
					end_group();

					add_group("events"); {
						add_group("hurt"); {
							add_var("[events]-[hurt] enable screen", settings::visuals::logs::hurt.enable_screen);
							add_var("[events]-[hurt] enable console", settings::visuals::logs::hurt.enable_console);
							add_var("[events]-[hurt] use custom color", settings::visuals::logs::hurt.using_custom_color);
							add_var("[events]-[hurt] custom color", settings::visuals::logs::hurt.custm_color);
						}
						end_group();

						add_group("buy"); {
							add_var("[events]-[buy] enable screen", settings::visuals::logs::player_buy.enable_screen);
							add_var("[events]-[buy] enable console", settings::visuals::logs::player_buy.enable_console);
							add_var("[events]-[buy] use custom color", settings::visuals::logs::player_buy.using_custom_color);
							add_var("[events]-[buy] custom color", settings::visuals::logs::player_buy.custm_color);
						}
						end_group();

						add_group("planting"); {
							add_var("[events]-[planting] enable screen", settings::visuals::logs::planting.enable_screen);
							add_var("[events]-[planting] enable console", settings::visuals::logs::planting.enable_console);
							add_var("[events]-[planting] use custom color", settings::visuals::logs::planting.using_custom_color);
							add_var("[events]-[planting] custom color", settings::visuals::logs::planting.custm_color);
						}
						end_group();

						add_group("defusing"); {
							add_var("[events]-[defusing] enable screen", settings::visuals::logs::defusing.enable_screen);
							add_var("[events]-[defusing] enable console", settings::visuals::logs::defusing.enable_console);
							add_var("[events]-[defusing] use custom color", settings::visuals::logs::defusing.using_custom_color);
							add_var("[events]-[defusing] custom color", settings::visuals::logs::defusing.custm_color);
						}
						end_group();

						add_group("configs"); {
							add_var("[events]-[config] enable screen", settings::visuals::logs::config.enable_screen);
							add_var("[events]-[config] enable console", settings::visuals::logs::config.enable_console);
							add_var("[events]-[config] use custom color", settings::visuals::logs::config.using_custom_color);
							add_var("[events]-[config] custom color", settings::visuals::logs::config.custm_color);
						}
						end_group();

						add_group("scripts"); {
							add_var("[events]-[scripts] enable screen", settings::visuals::logs::lua.enable_screen);
							add_var("[events]-[scripts] enable console", settings::visuals::logs::lua.enable_console);
							add_var("[events]-[scripts] use custom color", settings::visuals::logs::lua.using_custom_color);
							add_var("[events]-[scripts] custom color", settings::visuals::logs::lua.custm_color);
						}
						end_group();
					}
					end_group();

					add_group("grenades"); {
						add_var("[grenades] enable", settings::visuals::grenades::enable);

						add_var("[grenades] color flash", settings::visuals::grenades::color_flash);
						add_var("[grenades] color frag", settings::visuals::grenades::color_frag);
						add_var("[grenades] color tactical", settings::visuals::grenades::color_tactical);
						add_var("[grenades] color decoy", settings::visuals::grenades::color_decoy);

						add_var("[grenades] color smoke", settings::visuals::grenades::color_smoke);
						add_var("[grenades] smoke bar", settings::visuals::grenades::smoke_bar);
						add_var("[grenades] smoke radius", settings::visuals::grenades::smoke_radius);
						add_var("[grenades] smoke timer", settings::visuals::grenades::smoke_timer);
						add_var("[grenades] smoke bar main color", settings::visuals::grenades::color_bar_smoke_main);
						add_var("[grenades] smoke bar bg color", settings::visuals::grenades::color_bar_smoke_back);
						add_var("[grenades] smoke radius color", settings::visuals::grenades::color_smoke_radius);

						add_var("[grenades] color molotov", settings::visuals::grenades::color_molotov);
						add_var("[grenades] molotov bar", settings::visuals::grenades::molotov_bar);
						add_var("[grenades] molotov radius", settings::visuals::grenades::molotov_radius);
						add_var("[grenades] molotov timer", settings::visuals::grenades::molotov_timer);
						add_var("[grenades] molotov bar main color", settings::visuals::grenades::color_bar_molotov_main);
						add_var("[grenades] molotov bar bg color", settings::visuals::grenades::color_bar_molotov_back);
						add_var("[grenades] molotov radius color", settings::visuals::grenades::color_molotov_radius);
					}
					end_group();

					add_group("fog"); {
						add_var("[fog] enable", settings::visuals::fog::enable);
						add_var("[fog] color", settings::visuals::fog::clr);
						add_var("[fog] start dist", settings::visuals::fog::start_dist);
						add_var("[fog] end dist", settings::visuals::fog::end_dist);
					}
					end_group();

					add_group("capsule hitbox"); {
						add_var("[capsule hitbox] enable", settings::visuals::hitbox::enable);
						add_var("[capsule hitbox] color", settings::visuals::hitbox::clr);
						add_var("[capsule hitbox] show time", settings::visuals::hitbox::show_time);
						add_var("[capsule hitbox] only when kill", settings::visuals::hitbox::show_only_kill);
					}
					end_group();
				}
				end_group();
			}
			end_group();

			add_group("windows"); {
				add_var("[windows] bind window show", settings::windows::bind_window_show);
				add_var("[windows] bind window pos", "binds", settings::windows::bind_window_pos.x, settings::windows::bind_window_pos.y);
				add_var("[windows] bind window alpha", settings::windows::bind_window_alpha);

				add_var("[windows] spectator list show", settings::windows::spectator_list_show);
				add_var("[windows] spectator list pos", "spectator list", settings::windows::spectator_list_pos.x, settings::windows::spectator_list_pos.y);
				add_var("[windows] spectator list alpha", settings::windows::spectator_list_alpha);
			}
			end_group();

			add_group("misc"); {
				add_group("gravity"); {
					add_var("[gravity] slow gravity", settings::misc::inverse_gravity::enable_slow);
					add_var("[gravity] inverse gravity", settings::misc::inverse_gravity::enable);
					add_var("[gravity] inverse gravity value", settings::misc::inverse_gravity::value);
				}
				end_group();

				add_group("fake latency"); {
					add_var("[fake latency] enable", settings::misc::fake_latency::enable);
					add_var("[fake latency] amount", settings::misc::fake_latency::amount);
				}
				end_group();

				add_var("[misc] watermark", settings::misc::watermark);
				add_var("[misc] obs bypass", settings::misc::obs_bypass);
				add_var("[misc] menu color", globals::menu_color);
				add_var("[misc] radar ingame", settings::visuals::ingame_radar);
				add_var("[misc] bhop", settings::misc::bhop::enable);
				add_var("[misc] auto strafer type", settings::misc::bhop::strafe_type);
				add_var("[misc] unlock inventory", settings::misc::unlock_inventory);
				add_var("[misc] auto accept", settings::misc::auto_accept);
				add_var("[misc] chat filter bypass", settings::misc::chat_filter_bypass);
				add_var("[misc] prepare revolver", settings::misc::prepare_revolver::enable);
				add_var("[misc] prepare revolver bind", settings::misc::prepare_revolver::bind);
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
					add_var("[fov] roll", settings::misc::viewmodel::roll);
					add_var("[fov] save roll side", settings::misc::viewmodel::save_roll_side);
				}
				end_group();				
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

		bool save(std::string name) {
			if (name.empty()) return false;

			setup_vars();

			return config_utils::save(_config.dir_name, _config.config, name);
		}

		bool load(std::string name) {
			return config_utils::load(_config.dir_name, name, _config.ints, _config.bools, _config.floats, _config.colors, _config.binds, _config.strings, _config.windows);
		}
	}

	namespace skins {
		void setup_vars() {
			push_cur_config(&_config);
			_config.clear();

			add_group("general"); {
				add_var("[general] show skins for selected weapon", settings::changers::skin::show_cur);
				add_var("[general] skin preview", settings::changers::skin::skin_preview);
			}
			end_group();

			add_group("skinchanger"); {
				for (auto& val : skin_changer::deeps::weapon_names) {
					add_group(val.name); {
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

			add_group("skin preview"); {
				for (int i = 0; i < skin_changer::deeps::weapon_names.size(); i++) {
					if (i > 3) {
						add_group(skin_changer::deeps::weapon_names[i].name); {
							add_var("[skin preview] [" + skin_changer::deeps::weapon_names[i].name + "] name", skin_changer::deeps::weapon_names[i]._weapon_name);
							add_var("[skin preview] [" + skin_changer::deeps::weapon_names[i].name + "] name skin", skin_changer::deeps::weapon_names[i].skin_name);
						}
						end_group();
					}
				}

				add_group("ct side"); {
					for (int i = 0; i < skin_changer::deeps::ct_knife_names.size(); i++) {
						if (i > 2) {
							add_group(skin_changer::deeps::ct_knife_names[i].name); {
								add_var("[ct skin preview]-[ct side]-[" + skin_changer::deeps::ct_knife_names[i].name + "] name", skin_changer::deeps::ct_knife_names[i]._weapon_name);
								add_var("[ct skin preview]-[ct side]-[" + skin_changer::deeps::ct_knife_names[i].name + "] name skin", skin_changer::deeps::ct_knife_names[i].skin_name);
							}
							end_group();
						}
					}

					for (int i = 0; i < skin_changer::deeps::ct_glove_names.size(); i++) {
						if (i > 2) {
							add_group(skin_changer::deeps::ct_glove_names[i].name); {
								add_var("[skin preview]-[ct side]-[" + skin_changer::deeps::ct_glove_names[i].name + "] name", skin_changer::deeps::ct_glove_names[i]._weapon_name);
								add_var("[skin preview]-[ct side]-[" + skin_changer::deeps::ct_glove_names[i].name + "] name skin", skin_changer::deeps::ct_glove_names[i].skin_name);
							}
							end_group();
						}
					}
				}
				end_group();

				add_group("t side"); {
					for (int i = 0; i < skin_changer::deeps::t_knife_names.size(); i++) {
						if (i > 2) {
							add_group(skin_changer::deeps::t_knife_names[i].name); {
								add_var("[skin preview]-[t side]-[" + skin_changer::deeps::t_knife_names[i].name + "] name", skin_changer::deeps::t_knife_names[i]._weapon_name);
								add_var("[skin preview]-[t side]-[" + skin_changer::deeps::t_knife_names[i].name + "] name skin", skin_changer::deeps::t_knife_names[i].skin_name);
							}
							end_group();
						}
					}

					for (int i = 0; i < skin_changer::deeps::t_glove_names.size(); i++) {
						if (i > 2) {
							add_group(skin_changer::deeps::t_glove_names[i].name); {
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

		bool save(std::string name) {
			if (name.empty()) return false;

			setup_vars();

			return config_utils::save(_config.dir_name, _config.config, name);
		}

		bool load(std::string name) {
			return config_utils::load(_config.dir_name, name, _config.ints, _config.bools, _config.floats, _config.colors, _config.binds, _config.strings, _config.windows);
		}
	}

	void push_cur_config(c_config* cfg) {
		current_config = cfg;
	}
	void add_var(std::string name, std::string wind, float& x, float& y) {
		config_utils::add_var(current_config->cur_line, current_config->cur_group, current_config->config, current_config->windows, name, wind, x, y);
	}
	void add_var(std::string name, std::string& var) {
		config_utils::add_var(current_config->cur_line, current_config->cur_group, current_config->config, current_config->strings, name, var);
	}
	void add_var(std::string name, int& var) {
		config_utils::add_var(current_config->cur_line, current_config->cur_group, current_config->config, current_config->ints, name, var);
	}
	void add_var(std::string name, float& var) {
		config_utils::add_var(current_config->cur_line, current_config->cur_group, current_config->config, current_config->floats, name, var);
	}
	void add_var(std::string name, key_bind_t& var) {
		config_utils::add_var(current_config->cur_line, current_config->cur_group, current_config->config, current_config->binds, name, var);
	}
	void add_var(std::string name, bool& var) {
		config_utils::add_var(current_config->cur_line, current_config->cur_group, current_config->config, current_config->bools, name, var);
	}
	void add_var(std::string name, color& var) {
		config_utils::add_var(current_config->cur_line, current_config->cur_group, current_config->config, current_config->colors, name, var);
	}
	void add_group(std::string name) {
		config_utils::add_group(current_config->cur_line, current_config->cur_group, current_config->config, name);
	}
	void end_group() {
		config_utils::end_group(current_config->cur_group);
	}
}