#define NOMINMAX
#include "../../sdk/structures/structures.h"
#include "misc.h"
#include "../../settings/settings.h"
#include "../notify/notify.h"
#include "../../helpers/input.h"
#include "../../helpers/math/math.h"

namespace misc {
	void agent_changer(frame_stage_t stage) {
		if (!sdk::engine_client->is_in_game() || (stage != frame_stage_t::render_start && stage != frame_stage_t::render_end)) return;

		static const auto get_player_viewmodel_arm_config_for_player_model = (const char**(__fastcall*)(const char*))(utils::pattern_scan(GetModuleHandleW(L"client.dll"), "E8 ? ? ? ? 89 87 ? ? ? ? 6A 00"));
		static int original_model = 0;

		if (!sdk::local_player) {
			original_model = 0;
			return;
		}

		constexpr auto get_model = [](int team) constexpr noexcept -> const char* {
			constexpr std::array models{
			"models/player/custom_player/legacy/ctm_fbi_variantb.mdl",
			"models/player/custom_player/legacy/ctm_fbi_variantf.mdl",
			"models/player/custom_player/legacy/ctm_fbi_variantg.mdl",
			"models/player/custom_player/legacy/ctm_fbi_varianth.mdl",
			"models/player/custom_player/legacy/ctm_sas_variantf.mdl",
			"models/player/custom_player/legacy/ctm_st6_variante.mdl",
			"models/player/custom_player/legacy/ctm_st6_variantg.mdl",
			"models/player/custom_player/legacy/ctm_st6_varianti.mdl",
			"models/player/custom_player/legacy/ctm_st6_variantk.mdl",
			"models/player/custom_player/legacy/ctm_st6_variantm.mdl",
			"models/player/custom_player/legacy/tm_balkan_variantf.mdl",
			"models/player/custom_player/legacy/tm_balkan_variantg.mdl",
			"models/player/custom_player/legacy/tm_balkan_varianth.mdl",
			"models/player/custom_player/legacy/tm_balkan_varianti.mdl",
			"models/player/custom_player/legacy/tm_balkan_variantj.mdl",
			"models/player/custom_player/legacy/tm_leet_variantf.mdl",
			"models/player/custom_player/legacy/tm_leet_variantg.mdl",
			"models/player/custom_player/legacy/tm_leet_varianth.mdl",
			"models/player/custom_player/legacy/tm_leet_varianti.mdl",
			"models/player/custom_player/legacy/tm_phoenix_variantf.mdl",
			"models/player/custom_player/legacy/tm_phoenix_variantg.mdl",
			"models/player/custom_player/legacy/tm_phoenix_varianth.mdl",

			"models/player/custom_player/legacy/tm_pirate.mdl",
			"models/player/custom_player/legacy/tm_pirate_varianta.mdl",
			"models/player/custom_player/legacy/tm_pirate_variantb.mdl",
			"models/player/custom_player/legacy/tm_pirate_variantc.mdl",
			"models/player/custom_player/legacy/tm_pirate_variantd.mdl",
			"models/player/custom_player/legacy/tm_anarchist.mdl",
			"models/player/custom_player/legacy/tm_anarchist_varianta.mdl",
			"models/player/custom_player/legacy/tm_anarchist_variantb.mdl",
			"models/player/custom_player/legacy/tm_anarchist_variantc.mdl",
			"models/player/custom_player/legacy/tm_anarchist_variantd.mdl",
			"models/player/custom_player/legacy/tm_balkan_varianta.mdl",
			"models/player/custom_player/legacy/tm_balkan_variantb.mdl",
			"models/player/custom_player/legacy/tm_balkan_variantc.mdl",
			"models/player/custom_player/legacy/tm_balkan_variantd.mdl",
			"models/player/custom_player/legacy/tm_balkan_variante.mdl",
			"models/player/custom_player/legacy/tm_jumpsuit_varianta.mdl",
			"models/player/custom_player/legacy/tm_jumpsuit_variantb.mdl",
			"models/player/custom_player/legacy/tm_jumpsuit_variantc.mdl"
			};

			switch (team) {
			case 2: return static_cast<std::size_t>(settings::visuals::agent_changer::model_t - 1) < models.size() ? models[settings::visuals::agent_changer::model_t - 1] : nullptr;
			case 3: return static_cast<std::size_t>(settings::visuals::agent_changer::model_ct - 1) < models.size() ? models[settings::visuals::agent_changer::model_ct - 1] : nullptr;
			default: return nullptr;
			}
		};

		const char* model = get_model(sdk::local_player->team_num());
		if (model) {
			if (stage == frame_stage_t::render_start) {
				original_model = sdk::local_player->model_index();
				c_network_string_table* modelprecache = sdk::network_string_table->find_table("modelprecache");
				if (modelprecache) {
					modelprecache->add_string(false, model);
				}
			}

			const auto idx = stage == frame_stage_t::render_end && original_model ? original_model : sdk::mdl_info->get_model_index(model);

			sdk::local_player->set_model_index(idx);
			if (c_base_player* ragdoll = (c_base_player*)sdk::entity_list->get_client_entity_from_handle(sdk::local_player->ragdoll()))
				ragdoll->set_model_index(idx);
		}
	}

	void prepare_revolver(c_user_cmd* cmd) {
		if (!sdk::local_player || !sdk::engine_client->is_connected() || !sdk::local_player->active_weapon() || !sdk::local_player->is_alive()) return;

		float revolver_prepare_time = 0.234375f;

		static float ready_time;
		if (settings::misc::prepare_revolver::bind.enable) {
			c_base_combat_weapon* active_weapon = sdk::local_player->active_weapon().get();
			if (active_weapon && active_weapon->item().item_definition_index() == item_definition_index::revolver) {
				if (!ready_time) ready_time = utils::get_curtime(cmd) + revolver_prepare_time;

				auto ticks_to_ready = TIME_TO_TICKS(ready_time - utils::get_curtime(cmd) - sdk::engine_client->get_net_channel_info()->get_latency(FLOW_OUTGOING));
				if (ticks_to_ready > 0 && ticks_to_ready <= TIME_TO_TICKS(revolver_prepare_time))
					cmd->buttons |= IN_ATTACK;
				else
					ready_time = 0.0f;
			}
		}
	}

	void moon_walk(c_user_cmd* cmd)
	{
		if (settings::misc::moon_walk)
		{
			if (sdk::local_player->move_type() == move_type::noclip) return;
			if (sdk::local_player->move_type() == move_type::ladder) return;
			if (!(sdk::local_player->m_flags() & entity_flags::on_ground)) return;

			if (cmd->buttons & IN_FORWARD){
				cmd->forwardmove = 450;
				cmd->buttons &= ~IN_FORWARD;
				cmd->buttons |= IN_BACK;
			} else if (cmd->buttons & IN_BACK) {
				cmd->forwardmove = -450;
				cmd->buttons &= ~IN_BACK;
				cmd->buttons |= IN_FORWARD;
			}

			if (cmd->buttons & IN_MOVELEFT) {
				cmd->sidemove = -450;
				cmd->buttons &= ~IN_MOVELEFT;
				cmd->buttons |= IN_MOVERIGHT;
			} else if (cmd->buttons & IN_MOVERIGHT) {
				cmd->sidemove = 450;
				cmd->buttons &= ~IN_MOVERIGHT;
				cmd->buttons |= IN_MOVELEFT;
			}
		}
	}

	void block_bot(c_user_cmd* cmd) {
		if (!settings::misc::block_bot::bind.enable) return;

		int bestdist = settings::misc::block_bot::max_dist;

		int index = -1;

		for (int i = 1; i < 64; i++) {
			c_base_player* entity = (c_base_player*)sdk::entity_list->get_client_entity(i);

			if (!entity) continue;
			if (!entity->is_alive() || entity->is_dormant() || entity == sdk::local_player) continue;

			float dist = sdk::local_player->get_abs_origin().dist_to(entity->get_abs_origin());

			if (dist < bestdist)
			{
				bestdist = dist;
				index = i;
			}
		}

		if (index == -1) return;

		c_base_player* target = (c_base_player*)sdk::entity_list->get_client_entity(index);

		if (!target) return;

		qangle angles = math::calc_angle(sdk::local_player->get_abs_origin(), target->get_abs_origin());

		qangle shit;

		sdk::engine_client->get_view_angles(&shit);

		angles.yaw -= shit.yaw;
		math::fix_angles(angles);

		if (angles.yaw < 0.20f) cmd->sidemove = 450.f;
		else if (angles.yaw > 0.20f) cmd->sidemove = -450.f;
	}

	void fast_duck(c_user_cmd* cmd) {
		if (settings::misc::fast_duck)
			cmd->buttons |= IN_BULLRUSH;
	}

	void edge_jump(c_user_cmd* cmd, int old_flags) {
		static bool jumped = false;

		if (!sdk::engine_client->is_connected()) return;
		if (!sdk::local_player || !sdk::local_player->is_alive()) return;

		if (settings::misc::edge_jump::enable) {

			if (settings::misc::edge_jump::bind.enable) {
				if ((old_flags & entity_flags::on_ground) && !(sdk::local_player->m_flags() & entity_flags::on_ground) && !(cmd->buttons & IN_JUMP)) {
					cmd->buttons |= IN_JUMP;
					jumped = true;
				}
			}

			if (settings::misc::edge_jump::auto_duck && jumped) cmd->buttons |= IN_DUCK;
			if ((old_flags & entity_flags::on_ground) && (sdk::local_player->m_flags() & entity_flags::on_ground)) jumped = false;
		}
	}

	void fog() {
		static convar* fog_enable = sdk::cvar->find_var("fog_enable");
		static convar* fog_override = sdk::cvar->find_var("fog_override");
		static convar* fog_color = sdk::cvar->find_var("fog_color");
		static convar* fog_start = sdk::cvar->find_var("fog_start");
		static convar* fog_end = sdk::cvar->find_var("fog_end");
		static convar* fog_destiny = sdk::cvar->find_var("fog_maxdensity");

		static const bool fog_enable_bec = fog_enable->get_bool();
		static const bool fog_override_bec = fog_override->get_bool();
		static const const char* fog_color_bec = fog_color->get_string();
		static const float fog_start_bec = fog_start->get_float();
		static const float fog_end_bec = fog_end->get_float();
		static const float fog_destiny_bec = fog_destiny->get_float();

		if (settings::visuals::fog::enable && !globals::unloading) {
			fog_enable->m_fnChangeCallbacks.m_Size = 0;
			fog_override->m_fnChangeCallbacks.m_Size = 0;
			fog_color->m_fnChangeCallbacks.m_Size = 0;
			fog_start->m_fnChangeCallbacks.m_Size = 0;
			fog_end->m_fnChangeCallbacks.m_Size = 0;
			fog_destiny->m_fnChangeCallbacks.m_Size = 0;

			color clr = settings::visuals::fog::clr;

			fog_enable->set_value(settings::visuals::fog::enable);
			fog_override->set_value(settings::visuals::fog::enable);
			fog_color->set_value(utils::snprintf("%d %d %d", clr.r(), clr.g(), clr.b()).c_str());
			fog_start->set_value(settings::visuals::fog::start_dist);
			fog_end->set_value(settings::visuals::fog::end_dist);
			fog_destiny->set_value(settings::visuals::fog::clr.a());
		} else if (globals::unloading || !settings::visuals::fog::enable) {
			fog_enable->m_fnChangeCallbacks.m_Size = 0;
			fog_override->m_fnChangeCallbacks.m_Size = 0;
			fog_color->m_fnChangeCallbacks.m_Size = 0;
			fog_start->m_fnChangeCallbacks.m_Size = 0;
			fog_end->m_fnChangeCallbacks.m_Size = 0;
			fog_destiny->m_fnChangeCallbacks.m_Size = 0;

			fog_enable->set_value(fog_enable_bec);
			fog_override->set_value(fog_override_bec);
			fog_color->set_value(fog_color_bec);
			fog_start->set_value(fog_start_bec);
			fog_end->set_value(fog_end_bec);
			fog_destiny->set_value(fog_destiny_bec);
		}
	}

	void no_smoke() {
		static DWORD smoke_count;
		static uint8_t* offset;
		if (!sdk::engine_client->is_in_game() || !sdk::local_player || !sdk::mat_system)
			return;

		if (!offset) offset = utils::pattern_scan(GetModuleHandleW(L"client.dll"), "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0");
		if (!smoke_count) smoke_count = *(DWORD*)(offset + 0x8);

		if (settings::misc::disable_smoke) *(int*)(smoke_count) = 0;

		static bool set = true;
		static std::vector<const char*> smoke_materials = {
			"particle/vistasmokev1/vistasmokev1_fire",
			"particle/vistasmokev1/vistasmokev1_smokegrenade",
			"particle/vistasmokev1/vistasmokev1_emods",
			"particle/vistasmokev1/vistasmokev1_emods_impactdust",
		};

		if (!settings::misc::disable_smoke || globals::unloading) {
			if (set) {
				for (auto material_name : smoke_materials) {
					c_material* mat = sdk::mat_system->find_material(material_name, TEXTURE_GROUP_OTHER);
					mat->set_material_var_flag(MATERIAL_VAR_WIREFRAME, false);
				}
				set = false;
			}
			return;
		}

		set = true;
		for (auto material_name : smoke_materials) {
			c_material* mat = sdk::mat_system->find_material(material_name, TEXTURE_GROUP_OTHER);
			mat->set_material_var_flag(MATERIAL_VAR_WIREFRAME, true);
		}
	}

	void disable_flash_alpha() {
		if (settings::misc::disable_flash) sdk::local_player->flash_max_alpha() = settings::misc::flash_alpha;
		else sdk::local_player->flash_max_alpha() = 255.f;
	}

	void viewmodel() {
		static convar* fov_x = sdk::cvar->find_var("viewmodel_offset_x");
		static convar* fov_y = sdk::cvar->find_var("viewmodel_offset_y");
		static convar* fov_z = sdk::cvar->find_var("viewmodel_offset_z");
		static convar* view_model = sdk::cvar->find_var("viewmodel_fov");

		static const float fov_x_becup = fov_x->get_float();
		static const float fov_y_becup = fov_y->get_float();
		static const float fov_z_becup = fov_z->get_float();
		static const float view_model_becup = view_model->get_float();

		if (!globals::unloading) {
			fov_x->m_fnChangeCallbacks.m_Size = 0;
			fov_y->m_fnChangeCallbacks.m_Size = 0;
			fov_z->m_fnChangeCallbacks.m_Size = 0;
			view_model->m_fnChangeCallbacks.m_Size = 0;

			fov_x->set_value(settings::misc::viewmodel::fov_x);
			fov_y->set_value(settings::misc::viewmodel::fov_y);
			fov_z->set_value(settings::misc::viewmodel::fov_z);
			view_model->set_value(settings::misc::viewmodel::viewmodel);
		} else {
			fov_x->m_fnChangeCallbacks.m_Size = 0;
			fov_y->m_fnChangeCallbacks.m_Size = 0;
			fov_z->m_fnChangeCallbacks.m_Size = 0;
			view_model->m_fnChangeCallbacks.m_Size = 0;

			fov_x->set_value(fov_x_becup);
			fov_y->set_value(fov_y_becup);
			fov_z->set_value(fov_z_becup);
			view_model->set_value(view_model_becup);
		}
	}

	void gravity() {
		static convar* cl_ragdoll_gravity = sdk::cvar->find_var("cl_ragdoll_gravity");
		static convar* cl_phys_timescale = sdk::cvar->find_var("cl_phys_timescale");

		static const int becup_cl_ragdoll_gravity = cl_ragdoll_gravity->get_int();
		static const int becup_cl_phys_timescale = cl_phys_timescale->get_int();

		if (settings::misc::inverse_gravity::enable && !globals::unloading) {
			cl_ragdoll_gravity->m_fnChangeCallbacks.m_Size = 0;
			cl_ragdoll_gravity->set_value(settings::misc::inverse_gravity::value * 100);
		} else {
			cl_ragdoll_gravity->m_fnChangeCallbacks.m_Size = 0;
			cl_ragdoll_gravity->set_value(becup_cl_ragdoll_gravity);
		}

		if (settings::misc::inverse_gravity::enable_slow && !globals::unloading) {
			cl_phys_timescale->m_fnChangeCallbacks.m_Size = 0;
			cl_phys_timescale->set_value(0.2f);
		} else {
			cl_phys_timescale->m_fnChangeCallbacks.m_Size = 0;
			cl_phys_timescale->set_value(becup_cl_phys_timescale);
		}
	}

	namespace clan_tag {
		int user_index;

		size_t pos = 0;
		std::string clantag;
		float last_time = 0;

		void init() {
			if (!settings::misc::clantag::enable || !sdk::local_player) return;

			int tick = int(sdk::global_vars->curtime * 2.4f);
			static std::string local_tag;

			if (settings::misc::clantag::clantag_type == 0) {
				switch (settings::misc::clantag::animation_type) {
				case 0:
					switch (tick % 26) {
					case 0:	 utils::set_clantag(u8"|nullptr|"); break;
					case 1:	 utils::set_clantag(u8"|nullpt| "); break;
					case 2:  utils::set_clantag(u8"|nullp|  "); break;
					case 3:	 utils::set_clantag(u8"|null|   "); break;
					case 5:	 utils::set_clantag(u8"|nul|    "); break;
					case 6:  utils::set_clantag(u8"|nu|     "); break;
					case 7:  utils::set_clantag(u8"|n|      "); break;
					case 8:	 utils::set_clantag(u8"||       "); break;
					case 9:  utils::set_clantag(u8" ||      "); break;
					case 10: utils::set_clantag(u8"  ||     "); break;
					case 11: utils::set_clantag(u8"   ||    "); break;
					case 12: utils::set_clantag(u8"    ||   "); break;
					case 13: utils::set_clantag(u8"     ||  "); break;
					case 14: utils::set_clantag(u8"      || "); break;
					case 15: utils::set_clantag(u8"       ||"); break;
					case 16: utils::set_clantag(u8"      |r|"); break;
					case 17: utils::set_clantag(u8"     |tr|"); break;
					case 18: utils::set_clantag(u8"    |ptr|"); break;
					case 19: utils::set_clantag(u8"   |lptr|"); break;
					case 20: utils::set_clantag(u8"  |llptr|"); break;
					case 21: utils::set_clantag(u8" |ullptr|"); break;
					case 22: utils::set_clantag(u8"|nullptr|"); break;
					case 23: utils::set_clantag(u8"|nullptr|"); break;
					case 24: utils::set_clantag(u8"|nullptr|"); break;
					case 25: utils::set_clantag(u8"|nullptr|"); break;
					}
					break;
				case 1:
					switch (tick % 18) {
					case 0:  utils::set_clantag("         "); break;
					case 1:  utils::set_clantag("        g"); break;
					case 2:  utils::set_clantag("       ga"); break;
					case 3:  utils::set_clantag("      gam"); break;
					case 4:  utils::set_clantag("     game"); break;
					case 5:  utils::set_clantag("    games"); break;
					case 6:  utils::set_clantag("   gamese"); break;
					case 7:  utils::set_clantag("  gamesen"); break;
					case 8:  utils::set_clantag(" gamesens"); break;
					case 9:  utils::set_clantag("gamesense"); break;
					case 10: utils::set_clantag("gamesense"); break;
					case 11: utils::set_clantag("amesense "); break;
					case 12: utils::set_clantag("mesense  "); break;
					case 13: utils::set_clantag("esense   "); break;
					case 14: utils::set_clantag("sense    "); break;
					case 15: utils::set_clantag("ense     "); break;
					case 16: utils::set_clantag("nse      "); break;
					case 17: utils::set_clantag("se       "); break;
					case 18: utils::set_clantag("e        "); break;
					}
					break;
				case 2:
					switch (tick % 13) {
					case 0:  utils::set_clantag("AIMWARE.net"); break;
					case 1:  utils::set_clantag("IMWARE.net "); break;
					case 2:  utils::set_clantag("MWARE.net A"); break;
					case 3:  utils::set_clantag("WARE.net AI"); break;
					case 4:  utils::set_clantag("ARE.net AIM"); break;
					case 5:  utils::set_clantag("RE.net AIMW"); break;
					case 6:  utils::set_clantag("E.net AIMWA"); break;
					case 7:  utils::set_clantag(".net AIMWAR"); break;
					case 8:  utils::set_clantag("net AIMWARE"); break;
					case 9:  utils::set_clantag("et AIMWARE."); break;
					case 10: utils::set_clantag("t AIMWARE.n"); break;
					case 11: utils::set_clantag(" AIMWARE.ne"); break;
					case 12: utils::set_clantag("AIMWARE.net"); break;
					case 13: utils::set_clantag("AIMWARE.net"); break;
					}
					break;
				case 3:
					switch (tick % 15) {
					case 0:   utils::set_clantag("clown");     break;
					case 1:   utils::set_clantag("c lown");    break;
					case 2:   utils::set_clantag("c low n");   break;
					case 3:   utils::set_clantag("c l ow n");  break;
					case 4:   utils::set_clantag("c l o w n"); break;
					case 5:   utils::set_clantag("c l o w n"); break;
					case 6:   utils::set_clantag(" c l o w "); break;
					case 7:   utils::set_clantag("  c l o w"); break;
					case 8:   utils::set_clantag("   c l o "); break;
					case 9:   utils::set_clantag("    c l o"); break;
					case 10:  utils::set_clantag("     c l "); break;
					case 11:  utils::set_clantag("      c l"); break;
					case 12:  utils::set_clantag("       c "); break;
					case 13:  utils::set_clantag("        c"); break;
					case 14:  utils::set_clantag("         "); break;
					case 15:  utils::set_clantag("n        "); break;
					case 16:  utils::set_clantag("w n      "); break;
					case 17:  utils::set_clantag("o w n    "); break;
					case 18:  utils::set_clantag("l o w n  "); break;
					case 19:  utils::set_clantag("c l o w n"); break;
					case 20:  utils::set_clantag("c l o w n"); break;
					case 21:  utils::set_clantag("c l ow n");  break;
					case 22:  utils::set_clantag("c low n");   break;
					case 23:  utils::set_clantag("c lown");    break;
					}
					break;
				}
			} else if (settings::misc::clantag::clantag_type == 1) {
				if (user_index == 0) return;
				auto* player = c_base_player::get_player_by_index(user_index);
				if (!player) return;
				const auto info = player->get_player_info();
				if (info.fakeplayer) return;
				auto user_tag = std::string((*sdk::player_resource)->clantag()[player->get_index()]);

				utils::set_clantag(user_tag);
			} else if (settings::misc::clantag::clantag_type == 2) {
				if (settings::misc::clantag::clantag.empty() || settings::misc::clantag::clantag.length() == 0) return;

				if (settings::misc::clantag::custom_type == 0) {	
					switch (tick % 2) {
					case 1: utils::set_clantag(settings::misc::clantag::clantag, false); break;
					}

				} else if (settings::misc::clantag::custom_type == 1) {
					if (clantag != settings::misc::clantag::clantag || clantag.length() < pos) {
						clantag = settings::misc::clantag::clantag;
						pos = 0;
					}

					if (last_time + settings::misc::clantag::speed > sdk::global_vars->realtime) return;

					last_time = sdk::global_vars->realtime;

					utils::set_clantag(clantag.substr(0, pos).c_str());
					pos++;
				} else if (settings::misc::clantag::custom_type == 2) {

					static float last_change_time = 0.f;

					if (sdk::global_vars->realtime - last_change_time >= settings::misc::clantag::speed) {
						last_change_time = sdk::global_vars->realtime;

						std::string temp = settings::misc::clantag::clantag_visible;
						settings::misc::clantag::clantag_visible.erase(0, 1);
						settings::misc::clantag::clantag_visible += temp[0];

						utils::set_clantag(settings::misc::clantag::clantag_visible.data());
					}
				}
			}
		}
	}

	namespace bhop {
		void on_create_move(c_user_cmd* cmd) {
			if (!settings::misc::bhop::enable) return;

			static bool jumped_last_tick = false;
			static bool should_fake_jump = false;

			if (!jumped_last_tick && should_fake_jump) {
				should_fake_jump = false;
				cmd->buttons |= IN_JUMP;
			} else if (cmd->buttons & IN_JUMP) {
				if (sdk::local_player->m_flags() & entity_flags::on_ground) {
					jumped_last_tick = true;
					should_fake_jump = true;
				} else {
					cmd->buttons &= ~IN_JUMP;
					jumped_last_tick = false;
				}
			} else {
				jumped_last_tick = false;
				should_fake_jump = false;
			}
		}

		void auto_strafe(c_user_cmd* cmd, qangle va) {
			if (settings::misc::bhop::strafe_type == 0) return;

			static auto cl_sidespeed = sdk::cvar->find_var("cl_sidespeed");

			if (settings::misc::bhop::strafe_type == 1) {
				if (!sdk::local_player || !sdk::local_player->is_alive() || sdk::local_player->move_type() != move_type::walk) return;

				bool on_ground = (sdk::local_player->m_flags() & entity_flags::on_ground) && !(cmd->buttons & IN_JUMP);
				if (on_ground) return;

				static auto side = 1.0f;
				side = -side;

				auto velocity = sdk::local_player->velocity();
				velocity.z = 0.0f;

				qangle wish_angle = cmd->viewangles;

				auto speed = velocity.length();
				auto ideal_strafe = math::clamp(RAD2DEG(atan(15.f / speed)), 0.0f, 90.0f);

				if (cmd->forwardmove > 0.0f) cmd->forwardmove = 0.0f;

				static float old_yaw = 0.f;
				auto yaw_delta = std::remainderf(wish_angle.yaw - old_yaw, 360.0f);
				auto abs_angle_delta = abs(yaw_delta);
				old_yaw = wish_angle.yaw;

				if (abs_angle_delta <= ideal_strafe || abs_angle_delta >= 30.0f) {
					qangle velocity_direction;
					math::vector_angles(velocity, velocity_direction);
					auto velocity_delta = std::remainderf(wish_angle.yaw - velocity_direction.yaw, 360.0f);
					auto retrack = math::clamp(RAD2DEG(atan(30.0f / speed)), 0.0f, 90.0f) * 10.f;
					if (velocity_delta <= retrack || speed <= 15.0f) {
						if (-(retrack) <= velocity_delta || speed <= 15.0f) {
							wish_angle.yaw += side * ideal_strafe;
							cmd->sidemove = cl_sidespeed->get_float() * side;
						} else {
							wish_angle.yaw = velocity_direction.yaw - retrack;
							cmd->sidemove = cl_sidespeed->get_float();
						}
					} else {
						wish_angle.yaw = velocity_direction.yaw + retrack;
						cmd->sidemove = -cl_sidespeed->get_float();
					}

					math::movement_fix(cmd, wish_angle, cmd->viewangles);
				} else if (yaw_delta > 0.0f) {
					cmd->sidemove = -cl_sidespeed->get_float();
				} else if (yaw_delta < 0.0f) {
					cmd->sidemove = cl_sidespeed->get_float();
				}
			} else if (settings::misc::bhop::strafe_type == 2) {
				static auto old_yaw = 0.0f;

				auto get_velocity_degree = [](float velocity)
				{
					auto tmp = RAD2DEG(atan(30.0f / velocity));

					if (CHECK_IF_NON_VALID_NUMBER(tmp) || tmp > 90.0f)
						return 90.0f;

					else if (tmp < 0.0f)
						return 0.0f;
					else
						return tmp;
				};
				if (!sdk::local_player || !sdk::local_player->is_alive() || sdk::local_player->move_type() != move_type::walk || (sdk::local_player->m_flags() & entity_flags::on_ground) && !(cmd->buttons & IN_JUMP)) return;

				auto velocity = sdk::local_player->velocity();
				velocity.z = 0.0f;

				auto forwardmove = cmd->forwardmove;
				auto sidemove = cmd->sidemove;

				if (velocity.length_2d() < 5.0f && !forwardmove && !sidemove)
					return;

				static auto flip = false;
				flip = !flip;

				auto turn_direction_modifier = flip ? 1.0f : -1.0f;
				auto viewangles = cmd->viewangles;

				if (forwardmove || sidemove)
				{
					cmd->forwardmove = 0.0f;
					cmd->sidemove = 0.0f;

					auto turn_angle = atan2(-sidemove, forwardmove);
					viewangles.yaw += turn_angle * M_RADPI;
				}
				else if (forwardmove) //-V550
					cmd->forwardmove = 0.0f;

				auto strafe_angle = RAD2DEG(atan(15.0f / velocity.length_2d()));

				if (strafe_angle > 90.0f)
					strafe_angle = 90.0f;
				else if (strafe_angle < 0.0f)
					strafe_angle = 0.0f;

				auto temp = vec3(0.0f, viewangles.yaw - old_yaw, 0.0f);
				temp.y = math::normalize_yaw(temp.y);

				auto yaw_delta = temp.y;
				old_yaw = viewangles.yaw;

				auto abs_yaw_delta = fabs(yaw_delta);

				if (abs_yaw_delta <= strafe_angle || abs_yaw_delta >= 30.0f)
				{
					qangle velocity_angles;
					math::vector_angles(velocity, velocity_angles);

					temp = vec3(0.0f, viewangles.yaw - velocity_angles.yaw, 0.0f);
					temp.y = math::normalize_yaw(temp.y);

					auto velocityangle_yawdelta = temp.y;
					auto velocity_degree = get_velocity_degree(velocity.length_2d()) * 0.01f;

					if (velocityangle_yawdelta <= velocity_degree || velocity.length_2d() <= 15.0f)
					{
						if (-velocity_degree <= velocityangle_yawdelta || velocity.length_2d() <= 15.0f)
						{
							viewangles.yaw += strafe_angle * turn_direction_modifier;
							cmd->sidemove = cl_sidespeed->get_float() * turn_direction_modifier;
						}
						else
						{
							viewangles.yaw = velocity_angles.yaw - velocity_degree;
							cmd->sidemove = cl_sidespeed->get_float();
						}
					}
					else
					{
						viewangles.yaw = velocity_angles.yaw + velocity_degree;
						cmd->sidemove = -cl_sidespeed->get_float();
					}
				}
				else if (yaw_delta > 0.0f)
					cmd->sidemove = -cl_sidespeed->get_float();
				else if (yaw_delta < 0.0f)
					cmd->sidemove = cl_sidespeed->get_float();

				auto move = vec3(cmd->forwardmove, cmd->sidemove, 0.0f);
				auto speed = move.length();

				qangle angles_move;
				math::vector_angles(move, angles_move);

				auto normalized_x = fmod(cmd->viewangles.pitch + 180.0f, 360.0f) - 180.0f;
				auto normalized_y = fmod(cmd->viewangles.yaw + 180.0f, 360.0f) - 180.0f;

				auto yaw = DEG2RAD((normalized_y - viewangles.yaw) + angles_move.yaw);

				if (normalized_x >= 90.0f || normalized_x <= -90.0f || cmd->viewangles.pitch >= 90.0f && cmd->viewangles.pitch <= 200.0f || cmd->viewangles.pitch <= -90.0f && cmd->viewangles.pitch <= 200.0f) //-V648
					cmd->forwardmove = -cos(yaw) * speed;
				else
					cmd->forwardmove = cos(yaw) * speed;

				cmd->sidemove = sin(yaw) * speed;
			}
		}
	}

	void fake_latency(c_net_channel* net_channel) {
		static int32_t old_seq = 0;

		if (old_seq == net_channel->in_sequence_nr) return;
		old_seq = net_channel->in_sequence_nr;

		net_channel->in_sequence_nr += 63 * 2 - (uint32_t)(63 * (settings::misc::fake_latency::amount / 1000.f));
	}
}