#include "../hooks.h"
#include "../../functions/visuals/chams.h"

namespace hooks {
	void __fastcall draw_model_execute::hook(void* _this, int edx, c_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix3x4* boneo_matrix) {
		static auto o_draw_model_execute = mdl_render_vhook.get_original<fn>(indexes::draw_model_execute);

		bool shadow_depth = (info.flags & STUDIO_SHADOWDEPTHTEXTURE) != 0;

		if (!sdk::studio_render->is_forced_material_override() && !shadow_depth) {
			if (sdk::local_player && sdk::local_player->is_alive()) {
				c_material* xblur_mat = sdk::mat_system->find_material("dev/blurfilterx_nohdr", TEXTURE_GROUP_OTHER, true);
				c_material* yblur_mat = sdk::mat_system->find_material("dev/blurfiltery_nohdr", TEXTURE_GROUP_OTHER, true);
				c_material* scope = sdk::mat_system->find_material("dev/scope_bluroverlay", TEXTURE_GROUP_OTHER, true);

				xblur_mat->set_material_var_flag(MATERIAL_VAR_NO_DRAW, settings::misc::disable_zoom || settings::misc::disable_zoom_border);
				yblur_mat->set_material_var_flag(MATERIAL_VAR_NO_DRAW, settings::misc::disable_zoom || settings::misc::disable_zoom_border);
				scope->set_material_var_flag(MATERIAL_VAR_NO_DRAW, settings::misc::disable_zoom || settings::misc::disable_zoom_border);
			}

			chams::dme(ctx, state, info, boneo_matrix);

			sdk::mdl_render->forced_material_override(nullptr);
		}
		else
			return o_draw_model_execute(_this, edx, ctx, state, info, boneo_matrix);
	}
}