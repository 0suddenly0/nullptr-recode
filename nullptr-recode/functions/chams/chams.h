#pragma once
#include "../../sdk/structures/structures.h"
#include "../../hooks/hooks.h"

namespace chams {
	namespace materials {
        c_material* regular = nullptr;
        c_material* glow = nullptr;
        c_material* flat = nullptr;
        c_material* wireframe = nullptr;
	}

    void init();

    void dme(c_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& pInfo, matrix3x4* matrix);

    bool players(std::function<void()> o_dme, c_base_player* player);
    bool ragdolls(std::function<void()> o_dme, c_base_player* player);
    bool weapons(std::function<void()> o_dme, c_base_player* player, bool dropped);
    bool hands(std::function<void()> o_dme);

    c_material* create_material(const std::string& material_name, const std::string& shader_type, const std::string& material_data);
    void override_mat(bool visible_check, int type, color& clr);
}