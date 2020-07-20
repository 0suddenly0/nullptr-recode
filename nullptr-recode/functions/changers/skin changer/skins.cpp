#define _CRT_SECURE_NO_WARNINGS
#include "skins.h"
#include "../../../settings/settings.h"
#include "../../../render/render.h"
#include <d3dx9tex.h>
#include <winsock.h>
#pragma comment(lib,"ws2_32.lib")

struct hud_weapons_t {
	std::int32_t* get_weapon_count() {
		return (std::int32_t*)(std::uintptr_t(this) + 0x80);
	}
};
template<class T>
static T* find_hud_element(const char* name) {
	static auto pThis = *(DWORD**)(utils::pattern_scan(GetModuleHandleA("client.dll"), "B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08") + 1);

	static auto find_hud_element = (DWORD(__thiscall*)(void*, const char*))(utils::pattern_scan(GetModuleHandleA("client.dll"), "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28"));
	return (T*)find_hud_element(pThis, name);
}

namespace skin_changer {
	namespace deeps {
		const std::map<size_t, weapon_info> weapon_models_info = {
			{item_definition_index::knife,{"models/weapons/v_knife_default_ct.mdl", "knife"}},
			{item_definition_index::knife_t,{"models/weapons/v_knife_default_t.mdl", "knife_t"}},
			{item_definition_index::knife_bayonet, {"models/weapons/v_knife_bayonet.mdl", "bayonet"}},
			{item_definition_index::knife_css,{"models/weapons/v_knife_css.mdl", "knife_css"}},
			{item_definition_index::knife_skeleton,{"models/weapons/v_knife_skeleton.mdl", "knife_skeleton"}},
			{item_definition_index::knife_outdoor,{"models/weapons/v_knife_outdoor.mdl", "knife_outdoor"}},
			{item_definition_index::knife_canis,{"models/weapons/v_knife_canis.mdl", "knife_canis"}},
			{item_definition_index::knife_cord,{"models/weapons/v_knife_cord.mdl", "knife_cord"}},
			{item_definition_index::knife_flip, {"models/weapons/v_knife_flip.mdl", "knife_flip"}},
			{item_definition_index::knife_gut, {"models/weapons/v_knife_gut.mdl", "knife_gut"}},
			{item_definition_index::knife_karambit, {"models/weapons/v_knife_karam.mdl", "knife_karambit"}},
			{item_definition_index::knife_m9_bayonet, {"models/weapons/v_knife_m9_bay.mdl", "knife_m9_bayonet"}},
			{item_definition_index::knife_tactical, {"models/weapons/v_knife_tactical.mdl", "knife_tactical"}},
			{item_definition_index::knife_falchion, {"models/weapons/v_knife_falchion_advanced.mdl", "knife_falchion"}},
			{item_definition_index::knife_survival_bowie, {"models/weapons/v_knife_survival_bowie.mdl", "knife_survival_bowie"}},
			{item_definition_index::knife_butterfly, {"models/weapons/v_knife_butterfly.mdl", "knife_butterfly"}},
			{item_definition_index::knife_push, {"models/weapons/v_knife_push.mdl", "knife_push"}},
			{item_definition_index::knife_ursus,{"models/weapons/v_knife_ursus.mdl", "knife_ursus"}},
			{item_definition_index::knife_gypsy_jack,{"models/weapons/v_knife_gypsy_jackknife.mdl", "knife_gypsy_jackknife"}},
			{item_definition_index::knife_stiletto,{"models/weapons/v_knife_stiletto.mdl", "knife_stiletto"}},
			{item_definition_index::knife_widowmaker,{"models/weapons/v_knife_widowmaker.mdl", "knife_widowmaker"}},

			{item_definition_index::glove_studded_bloodhound,{"models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl"}},
			{item_definition_index::glove_t_side,{"models/weapons/v_models/arms/glove_fingerless/v_glove_fingerless.mdl"}},
			{item_definition_index::glove_ct_side,{"models/weapons/v_models/arms/glove_hardknuckle/v_glove_hardknuckle.mdl"}},
			{item_definition_index::glove_sporty,{"models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl"}},
			{item_definition_index::glove_slick,{"models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl"}},
			{item_definition_index::glove_leather_wrap,{"models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl"}},
			{item_definition_index::glove_motorcycle,{"models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl"}},
			{item_definition_index::glove_specialist,{"models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl"}},
			{item_definition_index::glove_hydra,{"models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound_hydra.mdl"}}
		};

		std::vector<weapon_name> t_knife_names = {
			{0,                                            "default"},
			{item_definition_index::knife_t,               "default t"},
			{item_definition_index::knife,                 "default ct"},
			{item_definition_index::knife_bayonet,         "bayonet"},
			{item_definition_index::knife_css,             "classic knife"},
			{item_definition_index::knife_cord,            "cord knife"},
			{item_definition_index::knife_canis,           "canis knife"},
			{item_definition_index::knife_outdoor,         "outdoor knife"},
			{item_definition_index::knife_skeleton,        "skeleton knife"},
			{item_definition_index::knife_flip,            "flip knife"},
			{item_definition_index::knife_gut,             "gut knife"},
			{item_definition_index::knife_karambit,        "karambit"},
			{item_definition_index::knife_m9_bayonet,      "m9 bayonet"},
			{item_definition_index::knife_tactical,        "huntsman knife"},
			{item_definition_index::knife_falchion,        "falchion knife"},
			{item_definition_index::knife_survival_bowie,  "bowie knife"},
			{item_definition_index::knife_butterfly,       "butterfly knife"},
			{item_definition_index::knife_push,            "shadow daggers"},
			{item_definition_index::knife_ursus,           "ursus knife"},
			{item_definition_index::knife_gypsy_jack,      "navaja knife"},
			{item_definition_index::knife_stiletto,        "stiletto knife"},
			{item_definition_index::knife_widowmaker,      "talon knife"}
		};

		std::vector<weapon_name> ct_knife_names = {
			{0,                                            "default"},
			{item_definition_index::knife_t,               "default t"},
			{item_definition_index::knife,                 "default ct"},
			{item_definition_index::knife_bayonet,         "bayonet"},
			{item_definition_index::knife_css,             "classic knife"},
			{item_definition_index::knife_cord,            "cord knife"},
			{item_definition_index::knife_canis,           "canis knife"},
			{item_definition_index::knife_outdoor,         "outdoor knife"},
			{item_definition_index::knife_skeleton,        "skeleton knife"},
			{item_definition_index::knife_flip,            "flip knife"},
			{item_definition_index::knife_gut,             "gut knife"},
			{item_definition_index::knife_karambit,        "karambit"},
			{item_definition_index::knife_m9_bayonet,      "m9 bayonet"},
			{item_definition_index::knife_tactical,        "huntsman knife"},
			{item_definition_index::knife_falchion,        "falchion knife"},
			{item_definition_index::knife_survival_bowie,  "bowie knife"},
			{item_definition_index::knife_butterfly,       "butterfly knife"},
			{item_definition_index::knife_push,            "shadow daggers"},
			{item_definition_index::knife_ursus,           "ursus knife"},
			{item_definition_index::knife_gypsy_jack,      "navaja knife"},
			{item_definition_index::knife_stiletto,        "stiletto knife"},
			{item_definition_index::knife_widowmaker,      "talon knife"}
		};

		std::vector<weapon_name> t_glove_names = {
			{0,                                               "default"},
			{item_definition_index::glove_t_side,             "default t"},
			{item_definition_index::glove_ct_side,            "default ct"},
			{item_definition_index::glove_studded_bloodhound, "bloodhound"},
			{item_definition_index::glove_sporty,             "sporty"},
			{item_definition_index::glove_slick,              "slick"},
			{item_definition_index::glove_leather_wrap,       "handwrap"},
			{item_definition_index::glove_motorcycle,         "motorcycle"},
			{item_definition_index::glove_specialist,         "specialist"},
			{item_definition_index::glove_hydra,              "hydra"}
		};

		std::vector<weapon_name> ct_glove_names = {
			{0,                                               "default"},
			{item_definition_index::glove_t_side,             "default t"},
			{item_definition_index::glove_ct_side,            "default ct"},
			{item_definition_index::glove_studded_bloodhound, "bloodhound"},
			{item_definition_index::glove_sporty,             "sporty"},
			{item_definition_index::glove_slick,              "slick"},
			{item_definition_index::glove_leather_wrap,       "handwrap"},
			{item_definition_index::glove_motorcycle,         "motorcycle"},
			{item_definition_index::glove_specialist,         "specialist"},
			{item_definition_index::glove_hydra,              "hydra"}
		};


		std::vector<weapon_name> weapon_names = {
			{item_definition_index::knife, "knife ct"},
			{item_definition_index::knife_t, "knife t"},
			{item_definition_index::glove_ct_side, "glove ct"},
			{item_definition_index::glove_t_side, "glove t"},
			{61, "usp"},
			{32, "p2000"},
			{4, "glock-18"},
			{2, "dual berettas"},
			{36, "p250"},
			{3, "five-seven"},
			{30, "tec-9"},
			{63, "cz75 auto"},
			{64, "r8 revolver"},
			{1, "deagle"},

			{34, "mp9"},
			{17, "mac-10"},
			{23, "mp5-sd"},
			{33, "mp7"},
			{24, "ump-45"},
			{19, "p90"},
			{26, "pp-bizon"},

			{7, "ak-47"},
			{60, "m4a1-s"},
			{16, "m4a4"},
			{8, "aug"},
			{39, "sg553"},
			{10, "famas"},
			{13, "galil"},

			{40, "ssg08"},
			{38, "scar-20"},
			{9, "awp"},
			{11, "g3sg1"},

			{14, "m249"},
			{28, "negev"},

			{27, "mag-7"},
			{35, "nova"},
			{29, "sawed-off"},
			{25, "xm1014"}
		};

		const std::vector<weapon_name> knife_names_preview = {
			{0, "default"},
			{item_definition_index::knife_t, "weapon_knife"},
			{item_definition_index::knife, "weapon_knife"},
			{item_definition_index::knife_bayonet, "weapon_bayonet"},
			{item_definition_index::knife_css, "weapon_knife_css"},
			{item_definition_index::knife_cord, "weapon_knife_cord"},
			{item_definition_index::knife_canis, "weapon_knife_canis"},
			{item_definition_index::knife_outdoor, "weapon_knife_outdoor"},
			{item_definition_index::knife_skeleton, "weapon_knife_skeleton"},
			{item_definition_index::knife_flip, "weapon_knife_flip"},
			{item_definition_index::knife_gut, "weapon_knife_gut"},
			{item_definition_index::knife_karambit, "weapon_knife_karambit"},
			{item_definition_index::knife_m9_bayonet, "weapon_knife_m9_bayonet"},
			{item_definition_index::knife_tactical, "weapon_knife_tactical"},
			{item_definition_index::knife_falchion, "weapon_knife_falchion"},
			{item_definition_index::knife_survival_bowie, "weapon_knife_survival_bowie"},
			{item_definition_index::knife_butterfly, "weapon_knife_butterfly"},
			{item_definition_index::knife_push, "weapon_knife_push"},
			{item_definition_index::knife_ursus, "weapon_knife_ursus"},
			{item_definition_index::knife_gypsy_jack, "weapon_knife_gypsy_jackknife"},
			{item_definition_index::knife_stiletto, "weapon_knife_stiletto"},
			{item_definition_index::knife_widowmaker, "weapon_knife_widowmaker"}
		};

		const std::vector<weapon_name> glove_names_preview = {
			{0, "default"},
			{item_definition_index::glove_t_side, "t_gloves"},
			{item_definition_index::glove_ct_side, "ct_gloves"},
			{item_definition_index::glove_studded_bloodhound, "studded_bloodhound_gloves"},
			{item_definition_index::glove_sporty, "sporty_gloves"},
			{item_definition_index::glove_slick, "slick_gloves"},
			{item_definition_index::glove_leather_wrap, "leather_handwraps"},
			{item_definition_index::glove_motorcycle, "motorcycle_gloves"},
			{item_definition_index::glove_specialist, "specialist_gloves"},
			{item_definition_index::glove_hydra, "studded_hydra_gloves"}
		};

		const std::vector<weapon_name> weapon_names_preview = {
			{item_definition_index::knife,    "knife ct"},
			{item_definition_index::knife_t,  "knife t"},
			{item_definition_index::glove_t_side,   "glove ct"},
			{item_definition_index::glove_ct_side,    "glove t"},

			{61,              "weapon_usp_silencer"},
			{32,              "weapon_hkp2000"},
			{4,               "weapon_glock"},
			{2,               "weapon_elite"},
			{36,              "weapon_p250"},
			{3,               "weapon_fiveseven"},
			{30,              "weapon_tec9"},
			{63,              "weapon_cz75a"},
			{64,              "weapon_revolver"},
			{1,               "weapon_deagle"},

			{34,              "weapon_mp9"},
			{17,              "weapon_mac10"},
			{23,              "weapon_mp5sd"},
			{33,              "weapon_mp7"},
			{24,              "weapon_ump45"},
			{19,              "weapon_p90"},
			{26,              "weapon_bizon"},

			{7,               "weapon_ak47"},
			{60,              "weapon_m4a1_silencer"},
			{16,              "weapon_m4a1"},
			{8,               "weapon_aug"},
			{39,              "weapon_sg556"},
			{10,              "weapon_famas"},
			{13,              "weapon_galilar"},

			{40,              "weapon_ssg08"},
			{38,              "weapon_scar20"},
			{9,               "weapon_awp"},
			{11,              "weapon_g3sg1"},

			{14,              "weapon_m249"},
			{28,              "weapon_negev"},

			{27,              "weapon_mag7"},
			{35,              "weapon_nova"},
			{29,              "weapon_sawedoff"},
			{25,              "weapon_xm1014"}
		};

		create_client_class_fn get_wearable_create_fn() {
			auto clazz = sdk::chl_client->get_all_classes();

			while (strcmp(clazz->network_name, "CEconWearable"))
				clazz = clazz->next;
			return clazz->create;
		}

		void erase_override_if_exists_by_index(const int definition_index) {
			if (weapon_models_info.count(definition_index)) {
				auto& icon_override_map = settings::changers::skin::m_icon_overrides;
				const auto& original_item = weapon_models_info.at(definition_index);
				if (original_item.icon && icon_override_map.count(original_item.icon)) {
					icon_override_map.erase(icon_override_map.at(original_item.icon));
				}
			}
		}

		void apply_config_on_attributable_item(c_base_attributable_item* item, const item_setting_t* config, const unsigned xuid_low) {
			item->item().item_id_high() = -1;
			item->item().account_id() = xuid_low;
			if (config->custom_name[0]) strcpy_s(item->item().custom_name(), config->custom_name);
			if (config->paint_kit_index) item->fallback_paint_kit() = config->paint_kit_index;
			if (config->seed) item->fallback_seed() = config->seed;
			if (config->stat_trak) {
				item->fallback_stat_trak() = settings::changers::skin::statrack_items[config->definition_index].statrack_new.counter;
				item->item().entity_quality() = 9;
			} else {
				item->item().entity_quality() = is_knife(config->definition_index) ? 3 : 0;
			}
			item->fallback_wear() = config->wear;
			auto& definition_index = item->item().item_definition_index();
			auto& icon_override_map = settings::changers::skin::m_icon_overrides;

			if (config->definition_override_index && config->definition_override_index != definition_index && weapon_models_info.count(config->definition_override_index)) {
				const auto old_definition_index = definition_index;
				definition_index = config->definition_override_index;
				const auto& replacement_item = weapon_models_info.at(config->definition_override_index);
				item->model_index() = sdk::mdl_info->get_model_index(replacement_item.model);
				item->set_module_index(sdk::mdl_info->get_model_index(replacement_item.model));
				item->get_client_networkable()->pre_data_update(0);
				if (old_definition_index && weapon_models_info.count(old_definition_index)) {
					const auto& original_item = weapon_models_info.at(old_definition_index);
					if (original_item.icon && replacement_item.icon) {
						icon_override_map[original_item.icon] = replacement_item.icon;
					}
				}
			} else {
				erase_override_if_exists_by_index(definition_index);
			}
		}

		IDirect3DTexture9* get_preview_image() {
			static IDirect3DTexture9* skin_texture = nullptr;

			static std::string selected_weapon_name = "";
			static std::string selected_skin_name = "";

			static std::string old_name_skin = "";
			static std::string old_name_weap = "";

			int definition_index = settings::changers::skin::m_items[weapon_names[menu::definition_vector_index].definition_index].definition_index;
			int definition_override_vector_index = settings::changers::skin::m_items[weapon_names[menu::definition_vector_index].definition_index].definition_override_vector_index;

			bool is_weapon = definition_index != item_definition_index::glove_t_side &&
				definition_index != item_definition_index::glove_ct_side &&
				definition_index != item_definition_index::knife &&
				definition_index != item_definition_index::knife_t;

			bool is_glove = definition_index == item_definition_index::glove_t_side ||
				definition_index == item_definition_index::glove_ct_side;

			bool is_knife = definition_index == item_definition_index::knife ||
				definition_index == item_definition_index::knife_t;

			bool is_ct = definition_index == item_definition_index::knife || definition_index == item_definition_index::glove_ct_side;
			bool is_t = definition_index == item_definition_index::knife_t || definition_index == item_definition_index::glove_t_side;

			weapon_name cur_gloves = { 0, "" };
			weapon_name cur_knifes = { 0, "" };
			if (is_glove) cur_gloves = is_ct ? ct_glove_names.at(definition_override_vector_index) : t_glove_names.at(definition_override_vector_index);
			if (is_knife) cur_knifes = is_ct ? ct_knife_names.at(definition_override_vector_index) : t_knife_names.at(definition_override_vector_index);

			if (is_weapon) {
				selected_skin_name = weapon_names.at(menu::definition_vector_index).skin_name.c_str();
				selected_weapon_name = weapon_names.at(menu::definition_vector_index)._weapon_name.c_str();
			} else if (is_knife) {
				selected_skin_name = cur_knifes.skin_name.c_str();
				selected_weapon_name = cur_knifes._weapon_name.c_str();
			} else if (is_glove) {
				selected_skin_name = cur_gloves.skin_name.c_str();
				selected_weapon_name = cur_gloves._weapon_name.c_str();
			}

			bool skin_changed = selected_skin_name != old_name_skin;
			bool weapon_changed = selected_weapon_name != old_name_weap;
			bool standart = selected_skin_name == "" || selected_skin_name == "empty";

			bool dont_draw = selected_weapon_name == "default" || ((menu::definition_vector_index == 2 || menu::definition_vector_index == 3) && standart && (definition_override_vector_index != 1 && definition_override_vector_index != 2));

			if (skin_changed || weapon_changed) {
				std::string filename = standart ? "resource/flash/econ/weapons/base_weapons/" + std::string(selected_weapon_name) + ".png" : "resource/flash/econ/default_generated/" + std::string(selected_weapon_name) + "_" + std::string(selected_skin_name) + "_light_large.png";

				const auto handle = sdk::base_file_system->open(filename.c_str(), "r", "GAME");
				if (handle) {
					int file_len = sdk::base_file_system->size(handle);
					char* image = new char[file_len];

					sdk::base_file_system->read(image, file_len, handle);
					sdk::base_file_system->close(handle);

					D3DXCreateTextureFromFileInMemory(sdk::d3d9device, image, file_len, &skin_texture);

					delete[] image;

					old_name_skin = selected_skin_name;
					old_name_weap = selected_weapon_name;
				}
			}

			if (!dont_draw)
				return skin_texture;
		}
	}

	void update() {
		static auto clear_hud_weapon_icon_fn = (std::int32_t(__thiscall*)(void*, std::int32_t))(
			utils::pattern_scan(GetModuleHandleA("client.dll"), "55 8B EC 51 53 56 8B 75 08 8B D9 57 6B FE 2C 89 5D FC"));

		auto element = find_hud_element<std::uintptr_t*>("CCSGO_HudWeaponSelection");

		if (element) {
			auto hud_weapons = (hud_weapons_t*)(std::uintptr_t(element) - 0xa0);
			if (hud_weapons != nullptr) {
				if (*hud_weapons->get_weapon_count()) {
					for (std::int32_t i = 0; i < *hud_weapons->get_weapon_count(); i++)
						i = clear_hud_weapon_icon_fn(hud_weapons, i);

					sdk::client_state->force_full_update();

				}
			}
		}
	}

	color get_color_ratiry(int rar) {
		switch (rar) {
		case 1: return color(150, 150, 150, 255); break;
		case 2: return color(100, 100, 255, 255); break;
		case 3: return color(50, 50, 255, 255); break;
		case 4: return color(255, 64, 242, 255); break;
		case 5: return color(255, 50, 50, 255); break;
		case 6: return color(255, 50, 50, 255); break;
		case 7:return color(255, 196, 46, 255); break;
		default: return color(150, 150, 150, 255); break;
		}
	}

	const char* get_icon_override(const std::string original) {
		return settings::changers::skin::m_icon_overrides.count(original) ? settings::changers::skin::m_icon_overrides.at(original).data() : nullptr;
	}

	void on_frame_stage_notify(frame_stage_t stage) {
		const auto local_index = sdk::engine_client->get_local_player();
		const auto local = (c_base_player*)(sdk::entity_list->get_client_entity(local_index));

		if (!local)
			return;
		player_info_t player_info;
		if (!sdk::engine_client->get_player_info(local_index, &player_info))
			return;


		if (stage == frame_stage_t::net_update_post_data_update_end) {
			const auto wearables = local->my_wearables();
			const auto glove_config = &settings::changers::skin::m_items[local->team_num() == 3 ? glove_ct_side : glove_t_side];
			static auto glove_handle = c_base_handle(0);
			auto glove = (c_base_attributable_item*)(sdk::entity_list->get_client_entity_from_handle(wearables[0]));
			if (!glove) {
				const auto our_glove = (c_base_attributable_item*)(sdk::entity_list->get_client_entity_from_handle(glove_handle));
				if (our_glove) {
					wearables[0] = glove_handle;
					glove = our_glove;
				}
			}
			if (!local->is_alive()) {
				if (glove) {
					glove->get_client_networkable()->set_destroyed_on_recreate_entities();
					glove->get_client_networkable()->release();
				}
				return;
			}

			if (glove_config && glove_config->definition_override_index) {
				if (!glove) {
					static auto create_wearable_fn = deeps::get_wearable_create_fn();
					const auto entry = sdk::entity_list->get_highest_entity_index() + 1;
					const auto serial = rand() % 0x1000;

					create_wearable_fn(entry, serial);
					glove = (c_base_attributable_item*)(sdk::entity_list->get_client_entity(entry));
					assert(glove); {
						static auto set_abs_origin_addr = utils::pattern_scan(GetModuleHandleA("client.dll"), "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8");
						const auto set_abs_origin_fn = (void(__thiscall*)(void*, const std::array<float, 3>&))(set_abs_origin_addr);
						static constexpr std::array<float, 3> new_pos = { 10000.f, 10000.f, 10000.f };
						set_abs_origin_fn(glove, new_pos);
					}
					const auto wearable_handle = (c_base_handle*)(&wearables[0]);
					*wearable_handle = entry | serial << 16;
					glove_handle = wearables[0];
				}

				glove->set_glove_model_index(-1);
				deeps::apply_config_on_attributable_item(glove, glove_config, player_info.xuid_low);
			}
		}
		else if (stage == frame_stage_t::net_update_post_data_update_start) {
			auto weapons = local->my_weapons();
			for (int i = 0; weapons[i].is_valid(); i++) {
				c_base_attributable_item* weapon = (c_base_attributable_item*)sdk::entity_list->get_client_entity_from_handle(weapons[i]);

				if (!weapon) continue;

				auto& definition_index = weapon->item().item_definition_index();
				const auto active_conf = &settings::changers::skin::m_items[is_knife(definition_index) ? (local->team_num() == 3 ? knife : knife_t) : definition_index];
				deeps::apply_config_on_attributable_item(weapon, active_conf, player_info.xuid_low);
			}
		}
		const auto view_model_handle = local->view_model();
		if (!view_model_handle.is_valid()) return;

		const auto view_model = (c_base_view_model*)(sdk::entity_list->get_client_entity_from_handle(view_model_handle));
		if (!view_model) return;

		const auto view_model_weapon_handle = view_model->weapon();
		if (!view_model_weapon_handle.is_valid()) return;

		const auto view_model_weapon = (c_base_combat_weapon*)(sdk::entity_list->get_client_entity_from_handle(view_model_weapon_handle));
		if (!view_model_weapon) return;

		if (deeps::weapon_models_info.count(view_model_weapon->item().item_definition_index())) {
			const auto override_model = deeps::weapon_models_info.at(view_model_weapon->item().item_definition_index()).model;
			auto override_model_index = sdk::mdl_info->get_model_index(override_model);

			view_model->model_index() = override_model_index;

			auto world_model_handle = view_model_weapon->weapon_world_model();
			if (!world_model_handle.is_valid()) return;

			const auto world_model = (c_base_weapon_world_model*)(sdk::entity_list->get_client_entity_from_handle(world_model_handle));
			if (!world_model) return;

			world_model->model_index() = override_model_index + 1;

		}
	}
}