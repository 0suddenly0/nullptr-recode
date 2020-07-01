#include "../hooks.h"

namespace hooks {
	void __stdcall override_view::hook(c_view_setup* view) {
		static auto o_override_view = client_mode_vhook.get_original<fn>(indexes::override_view);

		visuals::grenade_prediction::view();

		if (sdk::engine_client->is_in_game() && view) {
			visuals::thirdperson();
		}

		if (sdk::local_player && sdk::engine_client->is_in_game() && sdk::local_player->is_alive()) {
			if (settings::misc::viewmodel::override != 0 && !sdk::local_player->is_scoped())
				view->fov = settings::misc::viewmodel::override;

			static convar* zoom_sensitivity_ratio_mouse = sdk::cvar->find_var("zoom_sensitivity_ratio_mouse");
			if (settings::misc::disable_zoom && sdk::local_player->is_scoped()) {
				view->fov = (settings::misc::viewmodel::override == 0) ? 90 : settings::misc::viewmodel::override;
				zoom_sensitivity_ratio_mouse->set_value(0);
			} else {
				zoom_sensitivity_ratio_mouse->set_value(1);
			}

			auto view_model = (c_base_player*)(sdk::entity_list->get_client_entity_from_handle(sdk::local_player->view_model()));
			if (view_model)
			{
				static convar* hand = sdk::cvar->find_var("cl_righthand");
				vec3 temp_angle = view->angles;
				temp_angle.z = (settings::misc::viewmodel::save_roll_side ? (hand->get_bool() == true ? -settings::misc::viewmodel::roll : settings::misc::viewmodel::roll) : settings::misc::viewmodel::roll * -1);
				view_model->set_abs_angles(temp_angle);
			}
		}

		o_override_view(sdk::client_mode, view);
	}
}