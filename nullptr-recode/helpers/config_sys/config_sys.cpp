#include <numeric>
#include "config_sys.h"
#include "../../settings/settings.h"
#include "../../gui/imgui/imgui_internal.h"
#include "../../functions/notify/notify.h"

config::c_config* cur_config = new config::c_config("temp");

namespace config {
    namespace config_utils {
        std::string del_last_simbols(std::string* string, int num) {
            return string->erase(string->length() - num, string->length());
        }

        std::string del_first_simbols(std::string* string, int num) {
            return string->erase(0, num);
        }

        std::string del_last_simbols(std::string string, int num) {
            return string.erase(string.length() - num, string.length());
        }

        std::string del_first_simbols(std::string string, int num) {
            return string.erase(0, num);
        }

        void load_group(std::vector<std::string>& groups_list, std::string line) {
            del_first_simbols(&line, line.find("[")); // del space "|  " and "|- "

            if (line != "[ end ]") groups_list.push_back(line);
            else groups_list.pop_back();
        }

        std::string get_var_name(std::string line) {
            std::string ret = line;
            del_first_simbols(&ret, ret.find("|-") + 3); // del space "|  " and "|- "
            return del_last_simbols(ret, ret.length() - ret.find(" = ")); // del " = value;"
        }

        std::string get_var_value(std::string line) {
            std::string ret = line;
            del_last_simbols(&ret, 1); // del ";"
            del_first_simbols(&ret, ret.find("|-") + 3); // del space "|  " and "|- "
            return del_first_simbols(ret, ret.find(" = ") + 3); // del "var name = "
        }

        line_t get_line_type(std::string line) {
            if (line.back() == ']') {
                return line_t::group;
            } else if (line.back() == ';') {
                return line_t::var;
            }
        }
    }

    namespace standart {
        void setup() {
            _config.clear();
            push_config(&_config);

			std::function<void(legitbot_settings_t& cur_settings)> save_legit = [](legitbot_settings_t& cur_settings) {
				add_var("enable", cur_settings.enabled);
				add_var("silent", cur_settings.silent);
				add_var("autopistol", cur_settings.autopistol);
				add_group("checks"); {
					add_var("team", cur_settings.team_check);
					add_var("smoke", cur_settings.smoke_check);
					add_var("flash", cur_settings.flash_check);
				}
				end_group();
				add_group("autowall"); {
					add_var("enable", cur_settings.autowall);
					add_var("min damage", cur_settings.autowall_min_damage);
				}
				end_group();
				add_group("autofire"); {
					add_var("enable", cur_settings.autofire);
					add_var("bind", cur_settings.autofire_bind);
					add_var("chance", cur_settings.autofire_chance);
				}
				end_group();
				add_group("hitboxes"); {
					for (int i = 0; i < hitboxes_string.size(); i++) {
						add_var(hitboxes_string[i], cur_settings.hitboxes[i]);
					}
				}
				end_group();
				add_var("silent fov", cur_settings.silent_fov);
				add_var("silent chance", cur_settings.silent_chance);
				add_var("standart fov", cur_settings.fov);
				add_var("smooth", cur_settings.smooth);
				add_var("shot delay", cur_settings.shot_delay);
				add_var("kill delay", cur_settings.kill_delay);
				add_group("backtrack"); {
					add_var("ticks", cur_settings.backtrack_ticks);
					add_var("aim at backtrack", cur_settings.aim_at_backtrack);
					add_var("priority", cur_settings.backtrack_priority);
				}
				end_group();
				add_group("rcs"); {
					add_var("enable", cur_settings.rcs_enabled);
					add_var("start", cur_settings.rcs_start);
					add_var("type", cur_settings.rcs_type);
					add_var("x", cur_settings.rcs_x);
					add_var("y", cur_settings.rcs_y);
				}
				end_group();
			};

			add_group("legitbot"); {
				add_group("general"); {
					add_var("selected tab", settings::legitbot::settings_type);
					add_var("auto current", settings::legitbot::auto_current);
					add_var("using bind", settings::legitbot::using_bind);
					add_var("bind", settings::legitbot::bind);
				}
				end_group();

				add_group("settings groups"); {
					add_group("all"); {
						save_legit(settings::legitbot::legitbot_items_all);
					}
					end_group();

					add_group("groups"); {
						for (int i = 0; i < weapon_groups.size(); i++) {
							add_group(weapon_groups.at(i)); {
								save_legit(settings::legitbot::legitbot_items_groups[i]);
							}
							end_group();
						}
					}
					end_group();

					add_group("on weapon"); {
						for (int i = 0; i < weapon_names.size(); i++) {
							add_group(weapon_names.at(i).name); {
								save_legit(settings::legitbot::legitbot_items[i]);
							}
							end_group();
						}
					}
					end_group();
				}
				end_group();
			}
			end_group();

			add_group("visuals"); {
				add_group("esp"); {
					add_var("using bind", settings::visuals::esp::using_bind);
					add_var("bind", settings::visuals::esp::bind);

					for (int i = 0; i < _countof(player_tabs); i++) {
						std::string name = player_tabs[i];
						esp_settings_t& settings_cur = settings::visuals::esp::esp_items[i];

						add_group(name); {
							add_var("enable", settings_cur.enable);
							add_var("visible only", settings_cur.only_visible);
							add_var("box", settings_cur.box);
							add_var("color visible", settings_cur.box_visible);
							add_var("color invisible", settings_cur.box_invisible);
							add_var("dormant color", settings_cur.dormant);
							add_var("name", settings_cur.name);

							add_group("skeleton"); {
								add_var("enable", settings_cur.skeleton);
								add_var("color visible", settings_cur.skeleton_visible);
								add_var("color invisible", settings_cur.skeleton_invisible);
							}
							end_group();

							add_group("health"); {
								add_var("enable", settings_cur.health_bar);
								add_var("health in bar", settings_cur.health_in_bar);
								add_var("color based of health", settings_cur.health_based);
								add_var("bar color", settings_cur.health_bar_main);
								add_var("bar outline color", settings_cur.health_bar_outline);
								add_var("bar backgroud color", settings_cur.health_bar_background);
							}
							end_group();

							add_group("armor"); {
								add_var("enable", settings_cur.armor_bar);
								add_var("armor in bar", settings_cur.armor_in_bar);
								add_var("bar color", settings_cur.armor_bar_main);
								add_var("bar outline color", settings_cur.armor_bar_outline);
								add_var("bar backgroud color", settings_cur.armor_bar_background);
							}
							end_group();

							add_var("weapon", settings_cur.weapon);

							add_group("ammo"); {
								add_var("enable", settings_cur.weapon_ammo);
								add_var("text in bar", settings_cur.ammo_in_bar);
								add_var("bar color", settings_cur.ammo_bar_main);
								add_var("bar outline color", settings_cur.ammo_bar_outline);
								add_var("bar backgroud color", settings_cur.ammo_bar_background);
							}
							end_group();

							add_group("flags"); {
								add_var("flashed", settings_cur.flags_flashed);
								add_var("armor", settings_cur.flags_armor);
								add_var("have bomb", settings_cur.flags_bomb_carrier);
								add_var("defusing", settings_cur.flags_defusing);
								add_var("helment", settings_cur.flags_helmet);
								add_var("planting", settings_cur.flags_planting);
								add_var("reloading", settings_cur.flags_reloading);
								add_var("scoped", settings_cur.flags_scoped);
							}
							end_group();
						}
						end_group();
					}
				}
				end_group();

				add_group("chams"); {
					add_var("using bind", settings::visuals::chams::using_bind);
					add_var("bind", settings::visuals::chams::bind);

					for (int i = 0; i < _countof(player_tabs); i++) {
						std::string name = player_tabs[i];
						chams_settings_t& settings_cur = settings::visuals::chams::player_items[i];

						add_group(name); {
							for (int layer = 0; layer < _countof(layer_tabs); layer++) {
								std::string name_layer = layer_tabs[layer];
								chams_layer_settings_t& cur_layer = settings_cur.layers[layer];
								add_group(name_layer); {
									add_var("enable", cur_layer.enable);
									add_var("type", cur_layer.type);
									add_var("visible only", cur_layer.only_visible);
									add_var("visible", cur_layer.visible);
									add_var("invisible", cur_layer.invisible);
								}
								end_group();
							}
						}
						end_group();
					}

					add_group("oher chams"); {
						add_group("backtrack"); {
							add_var("draw tick", settings::visuals::chams::backtrack_tick_draw);

							for (int layer = 0; layer < _countof(layer_tabs); layer++) {
								std::string name_layer = layer_tabs[layer];
								chams_layer_settings_t& cur_layer = settings::visuals::chams::backtrack.layers[layer];
								add_group(name_layer); {
									add_var("enable", cur_layer.enable);
									add_var("type", cur_layer.type);
									add_var("visible only", cur_layer.only_visible);
									add_var("visible", cur_layer.visible);
									add_var("invisible", cur_layer.invisible);
								}
								end_group();
							}
						}
						end_group();

						add_group("ragdoll"); {
							for (int i = 0; i < _countof(ragdoll_tabs); i++) {
								std::string name = ragdoll_tabs[i];
								chams_settings_t& settings_cur = settings::visuals::chams::ragdoll_items[i];

								add_group(name); {
									for (int layer = 0; layer < _countof(layer_tabs); layer++) {
										std::string name_layer = layer_tabs[layer];
										chams_layer_settings_t& cur_layer = settings_cur.layers[layer];
										add_group(name_layer); {
											add_var("enable", cur_layer.enable);
											add_var("type", cur_layer.type);
											add_var("visible only", cur_layer.only_visible);
											add_var("visible", cur_layer.visible);
											add_var("invisible", cur_layer.invisible);
										}
										end_group();
									}
								}
								end_group();
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
												add_var("enable", cur_layer.enable);
												add_var("type", cur_layer.type);
												add_var("visible", cur_layer.visible);
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
										add_var("enable", cur_layer.enable);
										add_var("type", cur_layer.type);
										add_var("visible", cur_layer.visible);
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

				add_group("glow"); {
					add_var("using bind", settings::visuals::glow::using_bind);
					add_var("bind", settings::visuals::glow::bind);

					for (int i = 0; i < _countof(player_tabs); i++) {
						std::string name = player_tabs[i];
						glow_settings_t& settings_cur = settings::visuals::glow::glow_items[i];

						add_group(name); {
							add_var("enable", settings_cur.enable);
							add_var("type", settings_cur.type);
							add_var("visible only", settings_cur.visible_only);
							add_var("visible", settings_cur.visible);
							add_var("invisible", settings_cur.invisible);
						}
						end_group();
					}
				}
				end_group();

				add_group("other"); {
					add_group("offscreen"); {
						add_var("enable", settings::visuals::ofc::enable);
						add_var("color", settings::visuals::ofc::clr);
						add_var("visible check", settings::visuals::ofc::visible_check);
						add_var("range", settings::visuals::ofc::range);
						add_var("size", settings::visuals::ofc::size);
						add_var("type", settings::visuals::ofc::type);
					}
					end_group();

					add_var("night mode", settings::visuals::night_mode);
					add_var("asus props", settings::visuals::props::enable);
					add_var("asus props alpha", settings::visuals::props::alpha);
					add_var("bomb timer", settings::visuals::bomb_timer);
					add_var("bomb timer pos", settings::visuals::bomb_timer_pos);

					add_group("damage indicator"); {
						add_var("enable", settings::visuals::damage_indicator::enable);
						add_var("show time", settings::visuals::damage_indicator::show_time);
						add_var("speed", settings::visuals::damage_indicator::speed);
						add_var("max pos y", settings::visuals::damage_indicator::max_pos_y);
						add_var("offset at kill", settings::visuals::damage_indicator::offset_if_kill);
						add_var("offset at hit", settings::visuals::damage_indicator::offset_hit);
						add_var("range offset x", settings::visuals::damage_indicator::range_offset_x);
						add_var("kill color", settings::visuals::damage_indicator::kill_color);
					}
					end_group();

					add_group("hits"); {
						add_var("hitmarker enable", settings::visuals::hitmarker::enable);
						add_var("hitmarker show time", settings::visuals::hitmarker::show_time);
						add_var("hitmarker size", settings::visuals::hitmarker::size);
						add_var("hitmarker show type", settings::visuals::hitmarker::type);
						add_var("hitsound enable", settings::visuals::hitsound::enable);
						add_var("hitsound volume", settings::visuals::hitsound::volume);
					}
					end_group();

					add_group("agent changer"); {
						add_var("skin for ct side", settings::visuals::agent_changer::model_ct);
						add_var("skin for t side", settings::visuals::agent_changer::model_t);
					}
					end_group();

					add_group("aimbot fov"); {
						add_group("standart"); {
							add_var("outline", settings::visuals::aimbot_fov::standart_outline);
							add_var("outline color", settings::visuals::aimbot_fov::standart_outline_clr);
							add_var("filled", settings::visuals::aimbot_fov::standart_filled);
							add_var("filled color", settings::visuals::aimbot_fov::standart_filled_clr);
						}
						end_group();

						add_group("silent"); {
							add_var("outline", settings::visuals::aimbot_fov::silent_outline);
							add_var("outline color", settings::visuals::aimbot_fov::silent_outline_clr);
							add_var("filled", settings::visuals::aimbot_fov::silent_filled);
							add_var("filled color", settings::visuals::aimbot_fov::silent_filled_clr);
						}
						end_group();
					}
					end_group();

					add_group("grenade prediction"); {
						add_var("enable", settings::visuals::grenade_prediction::enable);
						add_var("radius", settings::visuals::grenade_prediction::radius);
						add_var("radius color", settings::visuals::grenade_prediction::radius_color);
						add_var("line thickness", settings::visuals::grenade_prediction::line_thickness);
						add_var("colision box size", settings::visuals::grenade_prediction::colision_box_size);
						add_var("last colision box size", settings::visuals::grenade_prediction::main_colision_box_size);
						add_var("line color", settings::visuals::grenade_prediction::main);
						add_var("colision box color", settings::visuals::grenade_prediction::main_box);
						add_var("main colision box color", settings::visuals::grenade_prediction::end_box);
					}
					end_group();

					add_group("impacts"); {
						add_group("client"); {
							add_var("enable", settings::visuals::impacts::client::enable);
							add_var("show time", settings::visuals::impacts::client::show_time);
							add_var("size", settings::visuals::impacts::client::size);
							add_var("color", settings::visuals::impacts::client::clr);
						}
						end_group();

						add_group("server"); {
							add_var("enable", settings::visuals::impacts::server::enable);
							add_var("show time", settings::visuals::impacts::server::show_time);
							add_var("size", settings::visuals::impacts::server::size);
							add_var("color", settings::visuals::impacts::server::clr);
						}
						end_group();
					}
					end_group();

					add_var("sniper crosshair", settings::visuals::sniper_crosshair);
					add_var("spread circle", settings::visuals::spread_circle::enable);
					add_var("spread circle color", settings::visuals::spread_circle::clr);

					add_group("dropped weapons"); {
						add_var("enable", settings::visuals::dropped_weapon::enable);
						add_var("box", settings::visuals::dropped_weapon::box);
						add_var("ammo bar", settings::visuals::dropped_weapon::ammo_bar);
						add_var("ammo in bar", settings::visuals::dropped_weapon::ammo_in_bar);
						add_var("ammo bar color", settings::visuals::dropped_weapon::bar_main);
						add_var("ammo bar bg color", settings::visuals::dropped_weapon::bar_background);
						add_var("ammo bar outline color", settings::visuals::dropped_weapon::bar_outline);
						add_var("box and text color", settings::visuals::dropped_weapon::box_color);
					}
					end_group();

					add_group("events"); {
						add_group("hurt"); {
							add_var("enable screen", settings::visuals::logs::hurt.enable_screen);
							add_var("enable console", settings::visuals::logs::hurt.enable_console);
							add_var("use custom color", settings::visuals::logs::hurt.using_custom_color);
							add_var("custom color", settings::visuals::logs::hurt.custm_color);
						}
						end_group();

						add_group("buy"); {
							add_var("enable screen", settings::visuals::logs::player_buy.enable_screen);
							add_var("enable console", settings::visuals::logs::player_buy.enable_console);
							add_var("use custom color", settings::visuals::logs::player_buy.using_custom_color);
							add_var("custom color", settings::visuals::logs::player_buy.custm_color);
						}
						end_group();

						add_group("planting"); {
							add_var("enable screen", settings::visuals::logs::planting.enable_screen);
							add_var("enable console", settings::visuals::logs::planting.enable_console);
							add_var("use custom color", settings::visuals::logs::planting.using_custom_color);
							add_var("custom color", settings::visuals::logs::planting.custm_color);
						}
						end_group();

						add_group("defusing"); {
							add_var("enable screen", settings::visuals::logs::defusing.enable_screen);
							add_var("enable console", settings::visuals::logs::defusing.enable_console);
							add_var("use custom color", settings::visuals::logs::defusing.using_custom_color);
							add_var("custom color", settings::visuals::logs::defusing.custm_color);
						}
						end_group();

						add_group("configs"); {
							add_var("enable screen", settings::visuals::logs::config.enable_screen);
							add_var("enable console", settings::visuals::logs::config.enable_console);
							add_var("use custom color", settings::visuals::logs::config.using_custom_color);
							add_var("custom color", settings::visuals::logs::config.custm_color);
						}
						end_group();

						add_group("scripts"); {
							add_var("enable screen", settings::visuals::logs::lua.enable_screen);
							add_var("enable console", settings::visuals::logs::lua.enable_console);
							add_var("use custom color", settings::visuals::logs::lua.using_custom_color);
							add_var("custom color", settings::visuals::logs::lua.custm_color);
						}
						end_group();
					}
					end_group();

					add_group("grenades"); {
						add_var("enable", settings::visuals::grenades::enable);

						add_var("color flash", settings::visuals::grenades::color_flash);
						add_var("color frag", settings::visuals::grenades::color_frag);
						add_var("color tactical", settings::visuals::grenades::color_tactical);
						add_var("color decoy", settings::visuals::grenades::color_decoy);

						add_var("color smoke", settings::visuals::grenades::color_smoke);
						add_var("smoke bar", settings::visuals::grenades::smoke_bar);
						add_var("smoke radius", settings::visuals::grenades::smoke_radius);
						add_var("smoke timer", settings::visuals::grenades::smoke_timer);
						add_var("smoke bar main color", settings::visuals::grenades::color_bar_smoke_main);
						add_var("smoke bar bg color", settings::visuals::grenades::color_bar_smoke_back);
						add_var("smoke radius color", settings::visuals::grenades::color_smoke_radius);

						add_var("color molotov", settings::visuals::grenades::color_molotov);
						add_var("molotov bar", settings::visuals::grenades::molotov_bar);
						add_var("molotov radius", settings::visuals::grenades::molotov_radius);
						add_var("molotov timer", settings::visuals::grenades::molotov_timer);
						add_var("molotov bar main color", settings::visuals::grenades::color_bar_molotov_main);
						add_var("molotov bar bg color", settings::visuals::grenades::color_bar_molotov_back);
						add_var("molotov radius color", settings::visuals::grenades::color_molotov_radius);
					}
					end_group();

					add_group("fog"); {
						add_var("enable", settings::visuals::fog::enable);
						add_var("ñolor", settings::visuals::fog::clr);
						add_var("start dist", settings::visuals::fog::start_dist);
						add_var("end dist", settings::visuals::fog::end_dist);
					}
					end_group();

					add_group("capsule hitbox"); {
						add_var("enable", settings::visuals::hitbox::enable);
						add_var("color", settings::visuals::hitbox::clr);
						add_var("show time", settings::visuals::hitbox::show_time);
						add_var("only when kill", settings::visuals::hitbox::show_only_kill);
					}
					end_group();
				}
				end_group();
			}
			end_group();

            add_group("windows"); {
                add_var("bind window", settings::windows::bind_window);
                add_var("spectator list", settings::windows::spectator_list);
            }
            end_group();

            add_group("misc"); {
                add_group("gravity"); {
                    add_var("slow gravity", settings::misc::inverse_gravity::enable_slow);
                    add_var("inverse gravity", settings::misc::inverse_gravity::enable);
                    add_var("inverse gravity value", settings::misc::inverse_gravity::value);
                }
                end_group();

                add_group("fake latency"); {
                    add_var("enable", settings::misc::fake_latency::enable);
                    add_var("amount", settings::misc::fake_latency::amount);
                }
                end_group();

                add_var("watermark", settings::misc::watermark);
                add_var("obs bypass", settings::misc::obs_bypass);
                add_var("menu color", globals::menu_color);
                add_var("radar ingame", settings::visuals::ingame_radar);
                add_var("bhop", settings::misc::bhop::enable);
                add_var("auto strafer type", settings::misc::bhop::strafe_type);
                add_var("unlock inventory", settings::misc::unlock_inventory);
                add_var("auto accept", settings::misc::auto_accept);
                add_var("chat filter bypass", settings::misc::chat_filter_bypass);
                add_var("prepare revolver", settings::misc::prepare_revolver::enable);
                add_var("prepare revolver bind", settings::misc::prepare_revolver::bind);
                add_var("block bot", settings::misc::block_bot::enable);
                add_var("block bot bind", settings::misc::block_bot::bind);

                add_group("clantag"); {
                    add_var("enable", settings::misc::clantag::enable);
                    add_var("type", settings::misc::clantag::clantag_type);
                    add_var("animation type", settings::misc::clantag::animation_type);
                    add_var("custom type", settings::misc::clantag::custom_type);
                    add_var("speed", settings::misc::clantag::speed);
                    add_var("check on empty", settings::misc::clantag::check_empty);
                    add_var("custom clantag", settings::misc::clantag::clantag);
                }
                end_group();

                add_group("disable"); {
                    add_var("flash", settings::misc::disable_flash);
                    add_var("flash alpha", settings::misc::flash_alpha);
                    add_var("smoke", settings::misc::disable_smoke);
                    add_var("zoom", settings::misc::disable_zoom);
                    add_var("zoom border", settings::misc::disable_zoom_border);
                }
                end_group();

                add_var("edge jump", settings::misc::edge_jump::enable);
                add_var("auto duck", settings::misc::edge_jump::auto_duck);
                add_var("edge jump key", settings::misc::edge_jump::bind);
                add_var("fast duck", settings::misc::fast_duck);
                add_var("moon walk", settings::misc::moon_walk);
                add_var("reveal money", settings::misc::reveal_money);
                add_var("reveal rank", settings::misc::reveal_rank);  

                add_group("third person"); {
                    add_var("enable", settings::misc::third_person::enable);
                    add_var("dist", settings::misc::third_person::dist);
                    add_var("bind", settings::misc::third_person::bind);
                }
                end_group();

                add_group("fov"); {
                    add_var("override", settings::misc::viewmodel::override);
                    add_var("viewmodel", settings::misc::viewmodel::viewmodel);
                    add_var("x", settings::misc::viewmodel::fov_x);
                    add_var("y", settings::misc::viewmodel::fov_y);
                    add_var("z", settings::misc::viewmodel::fov_z);
                    add_var("roll", settings::misc::viewmodel::roll);
                    add_var("save roll side", settings::misc::viewmodel::save_roll_side);
                }
                end_group();
            }
            end_group();

			add_group("profile changer"); {
				for (auto& [id, val] : mode_names) {
					add_group(val); {
						add_var("rank", settings::changers::profile::profile_items[id].rank_id);
						add_var("wins", settings::changers::profile::profile_items[id].wins);
					}
					end_group();
				}
				add_var("private rank", settings::changers::profile::private_id);
				add_var("private xp", settings::changers::profile::private_xp);

				add_group("comments"); {
					add_var("teacher", settings::changers::profile::teacher);
					add_var("friendly", settings::changers::profile::friendly);
					add_var("leader", settings::changers::profile::leader);
				}
				end_group();

				add_group("ban"); {
					add_var("type", settings::changers::profile::ban_type);
					add_var("time", settings::changers::profile::ban_time);
					add_var("time type", settings::changers::profile::ban_time_type);
				}
				end_group();

			}
			end_group();
        }

        bool save(std::string name) {
            if (name.empty()) return false;

            setup();

            CreateDirectoryA((LPCSTR)utils::snprintf("C:\\nullptr\\%s\\", _config.dir_name.c_str()).c_str(), NULL);

            std::ofstream out;
            out.open(utils::snprintf("C:\\nullptr\\%s\\%s.null", _config.dir_name.c_str(), name.c_str()));
            if (out.is_open()) {
				out << _config.config_text;
            }
            out.close();

            return true;
        }

        bool load(std::string name) {
            return _config.load(name);
        }
    }

    namespace skins {
        void setup() {
            _config.clear();
            push_config(&_config);

            add_group("general"); {
                add_var("show skins for selected weapon", settings::changers::skin::show_cur);
                add_var("skin preview", settings::changers::skin::skin_preview);
            }
            end_group();

            add_group("skinchanger"); {
                for (auto& val : skin_changer::deeps::weapon_names) {
                    add_group(val.name); {
                        auto& settings_cur = settings::changers::skin::m_items[val.definition_index];

                        add_var("paint kit", settings_cur.paint_kit_index);
                        add_var("paint kit index", settings_cur.paint_kit_vector_index);
                        add_var("selected model idx", settings_cur.definition_override_index);
                        add_var("selected model vector", settings_cur.definition_override_vector_index);
                    }
                    end_group();
                }
            }
            end_group();

            add_group("skin preview"); {
                for (int i = 0; i < skin_changer::deeps::weapon_names.size(); i++) {
                    if (i > 3) {
                        add_group(skin_changer::deeps::weapon_names[i].name); {
                            add_var("name", skin_changer::deeps::weapon_names[i]._weapon_name);
                            add_var("name skin", skin_changer::deeps::weapon_names[i].skin_name);
                        }
                        end_group();
                    }
                }

                add_group("ct side"); {
                    for (int i = 0; i < skin_changer::deeps::ct_knife_names.size(); i++) {
                        if (i > 2) {
                            add_group(skin_changer::deeps::ct_knife_names[i].name); {
                                add_var("name", skin_changer::deeps::ct_knife_names[i]._weapon_name);
                                add_var("name skin", skin_changer::deeps::ct_knife_names[i].skin_name);
                            }
                            end_group();
                        }
                    }

                    for (int i = 0; i < skin_changer::deeps::ct_glove_names.size(); i++) {
                        if (i > 2) {
                            add_group(skin_changer::deeps::ct_glove_names[i].name); {
                                add_var("name", skin_changer::deeps::ct_glove_names[i]._weapon_name);
                                add_var("name skin", skin_changer::deeps::ct_glove_names[i].skin_name);
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
                                add_var("name", skin_changer::deeps::t_knife_names[i]._weapon_name);
                                add_var("name skin", skin_changer::deeps::t_knife_names[i].skin_name);
                            }
                            end_group();
                        }
                    }

                    for (int i = 0; i < skin_changer::deeps::t_glove_names.size(); i++) {
                        if (i > 2) {
                            add_group(skin_changer::deeps::t_glove_names[i].name); {
                                add_var("name", skin_changer::deeps::t_glove_names[i]._weapon_name);
                                add_var("name skin", skin_changer::deeps::t_glove_names[i].skin_name);
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

            setup();

            CreateDirectoryA((LPCSTR)utils::snprintf("C:\\nullptr\\%s\\", _config.dir_name.c_str()).c_str(), NULL);

            std::ofstream out;
            out.open(utils::snprintf("C:\\nullptr\\%s\\%s.null", _config.dir_name.c_str(), name.c_str()));
			if (out.is_open()) {
				out << _config.config_text;
			}
            out.close();

            return true;
        }

        bool load(std::string name) {
            return _config.load(name);
        }
    }

    void push_config(c_config* config) {
        cur_config = config;
    }

    void add_group(std::string name) {
        cur_config->add_space();
        cur_config->add_group(name);
    }

    void end_group() {
        cur_config->end_group();
    }

    void add_var(std::string name, int& var) {
        cur_config->add_space();
        cur_config->add_var(name, var);
    }

    void add_var(std::string name, float& var) {
        cur_config->add_space();
        cur_config->add_var(name, var);
    }

    void add_var(std::string name, bool& var) {
        cur_config->add_space();
        cur_config->add_var(name, var);
    }

    void add_var(std::string name, std::string& var) {
        cur_config->add_space();
        cur_config->add_var(name, var);
    }

    void add_var(std::string name, color& var) {
        cur_config->add_space();
        cur_config->add_var(name, var);
    }

    void add_var(std::string name, key_bind_t& var) {
        cur_config->add_space();
        cur_config->add_var(name, var);
    }

    void add_var(std::string name, window_settings_t& var) {
        cur_config->add_space();
        cur_config->add_var(name, var);
    }
}