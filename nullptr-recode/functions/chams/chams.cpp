#include "chams.h"
#include "../../sdk/interfaces/key_values.h"

namespace chams {
    void init() {
        materials::regular = create_material("null_regular.vmt", "VertexLitGeneric", R"#("VertexLitGeneric" {
        "$basetexture" "vgui/white_additive"
        "$ignorez"      "0"
        "$envmap"       ""
        "$nofog"        "1"
        "$model"        "1"
        "$nocull"       "0"
        "$selfillum"    "1"
        "$halflambert"  "1"
        "$znearer"      "0"
        "$flat"         "1"
        })#");

        materials::flat = create_material("null_flat.vmt", "UnlitGeneric", R"#("UnlitGeneric" {
        "$basetexture" "vgui/white_additive"
        "$ignorez"      "0"
        "$envmap"       ""
        "$nofog"        "1"
        "$model"        "1"
        "$nocull"       "0"
        "$selfillum"    "1"
        "$halflambert"  "1"
        "$znearer"      "0"
        "$flat"         "1"
        })#");

        materials::glow = create_material("null_glow.vmt", "VertexLitGeneric", R"#("VertexLitGeneric" {
        "$additive"               "1"
        "$envmap"                 "models/effects/cube_white"
        "$envmaptint"             "[1 1 1]"
        "$envmapfresnel"          "1"
        "$envmapfresnelminmaxexp" "[0 1 2]"
        "$ignorez"                "0"
        })#");

        materials::wireframe = create_material("null_wireframe.vmt", "VertexLitGeneric", R"#("VertexLitGeneric" {
        "$basetexture" "vgui/white_additive"
        "$ignorez"      "0"
        "$envmap"       ""
        "$nofog"        "1"
        "$model"        "1"
        "$nocull"       "0"
        "$selfillum"    "1"
        "$halflambert"  "1"
        "$znearer"      "0"
        "$flat"         "1"
        })#");
    }

    void dme(c_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix3x4* matrix) {
        static auto o_draw_model_execute = hooks::mdl_render_vhook.get_original<hooks::draw_model_execute::fn>(hooks::indexes::draw_model_execute);
        std::function<void()> o_dme = [ctx, state, info, matrix]() {
            o_draw_model_execute(sdk::mdl_render, 0, ctx, state, info, matrix);
        };

        c_base_player* entity = (c_base_player*)sdk::entity_list->get_client_entity(info.entity_index);
        bool can_draw_original = true;

        if (std::strstr(info.pModel->szName, "models/weapons/")) {
            if (std::strstr(info.pModel->szName, "models/weapons/v_") && (std::strstr(info.pModel->szName + 17, "sleeve") || std::strstr(info.pModel->szName + 17, "arms"))) {
                can_draw_original = hands(o_dme);
            } else if (!std::strstr(info.pModel->szName + 17, "tablet")
                && !std::strstr(info.pModel->szName + 17, "parachute")
                && !std::strstr(info.pModel->szName + 17, "fists")
                && entity) {
                weapons(o_dme, entity, std::strstr(info.pModel->szName, "models/weapons/w_") && std::strstr(info.pModel->szName, "_dropped.mdl"));
            }
        } else if (entity && strstr(info.pModel->szName, "models/player")) {
            can_draw_original = entity->is_alive() ? players(o_dme, entity) : ragdolls(o_dme, entity);
        }

        if(can_draw_original)
            o_draw_model_execute(sdk::mdl_render, 0, ctx, state, info, matrix);
    }

    bool players(std::function<void()> o_dme, c_base_player* player) {
        bool ret = true;
        if (player->team_num() != sdk::local_player->team_num()) {
            for (auto& layer : settings::visuals::chams::player_items[esp_types::enemies].layers) {
                if (!layer.enable)
                    continue;

                override_mat(true, layer.type, layer.invisible);
                o_dme();
                if (!layer.only_visible) {
                    override_mat(false, layer.type, layer.visible);
                    o_dme();
                }

                ret = false;
            }
        } else if (player->team_num() == sdk::local_player->team_num() && player != sdk::local_player) {
            for (auto& layer : settings::visuals::chams::player_items[esp_types::teammates].layers) {
                if (!layer.enable)
                    continue;

                override_mat(true, layer.type, layer.invisible);
                o_dme();
                if (!layer.only_visible) {
                    override_mat(false, layer.type, layer.visible);
                    o_dme();
                }

                ret = false;
            }
        } else if (player == sdk::local_player) {
            for (auto& layer : settings::visuals::chams::player_items[esp_types::local_player].layers) {
                if (!layer.enable)
                    continue;

                override_mat(true, layer.type, layer.invisible);
                o_dme();
                if (!layer.only_visible) {
                    override_mat(false, layer.type, layer.visible);
                    o_dme();
                }

                ret = false;
            }
        }

        return ret;
    }

    bool ragdolls(std::function<void()> o_dme, c_base_player* player) {
        bool ret = true;
        if (player->team_num() != sdk::local_player->team_num()) {
            for (auto& layer : settings::visuals::chams::ragdoll_items[esp_types::enemies].layers) {
                if (!layer.enable)
                    continue;

                override_mat(true, layer.type, layer.invisible);
                o_dme();
                if (!layer.only_visible) {
                    override_mat(false, layer.type, layer.visible);
                    o_dme();
                }

                ret = false;
            }
        } else if (player->team_num() == sdk::local_player->team_num()) {
            for (auto& layer : settings::visuals::chams::ragdoll_items[esp_types::teammates].layers) {
                if (!layer.enable)
                    continue;

                override_mat(true, layer.type, layer.invisible);
                o_dme();
                if (!layer.only_visible) {
                    override_mat(false, layer.type, layer.visible);
                    o_dme();
                }

                ret = false;
            }
        }

        return ret;
    }

    bool weapons(std::function<void()> o_dme, c_base_player* player, bool dropped) {
        bool ret = true;

        if (!dropped) {
            for (auto& layer : settings::visuals::chams::weapon_items[weapon_chams_types::in_hands].layers) {
                if (!layer.enable)
                    continue;

                override_mat(false, layer.type, layer.visible);
                o_dme();

                ret = false;
            }
        } else {
            for (auto& layer : settings::visuals::chams::weapon_items[weapon_chams_types::dropped].layers) {
                if (!layer.enable)
                    continue;

                override_mat(false, layer.type, layer.visible);
                o_dme();

                ret = false;
            }
        }

        return ret;
    }

    bool hands(std::function<void()> o_dme) {
        if (!sdk::local_player || !sdk::local_player->is_alive())
            return true;

        bool ret = true;
        for (auto& layer : settings::visuals::chams::hands.layers) {
            if (!layer.enable)
                continue;

            override_mat(false, layer.type, layer.visible);
            o_dme();

            ret = false;
        }

        return ret;
    }

	c_material* create_material(const std::string& material_name, const std::string& shader_type, const std::string& material_data) {
		const auto key_values = (c_key_values*)(sdk::mem_alloc->alloc(36u));

		key_values->init(shader_type.c_str());
		key_values->load_from_buffer(material_name.c_str(), material_data.c_str());

        return sdk::mat_system->create_material(material_name.c_str(), key_values);
	}

    void override_mat(bool visible_check, int type, color& clr) {
        c_material* material = nullptr;

        switch (type) {
        case 0: material = materials::regular; break;
        case 1: material = materials::flat; break;
        case 2: material = materials::glow; break;
        case 3: material = materials::wireframe; material->set_material_var_flag(MATERIAL_VAR_WIREFRAME, true); break;
        }

        if (!material)
            return; 

        material->alpha_modulate(clr.a<float>());
        material->color_modulate(clr.r<float>(), clr.g<float>(), clr.b<float>());

        if (const auto alpha = material->find_var("$alpha", nullptr, false)) alpha->set_value(clr.a<float>());
        if (const auto envmaptint = material->find_var("$envmaptint", nullptr, false)) envmaptint->set_value(vec3(clr.r<float>(), clr.g<float>(), clr.b<float>()));

        //if (const auto ignorez = material->find_var("$ignorez", nullptr, false)) {
        //    ignorez->set_value(1.f);
            //notify::add("гучи флип флап", "фак ит ю хит йюр бич ин май сакс");
        //}

        material->set_material_var_flag(MATERIAL_VAR_IGNOREZ, visible_check);

        material->increment_reference_count();
        sdk::studio_render->forced_material_override(material);
    }
}