#include "visuals.h"
#include "../../settings/settings.h"
#include "../../sdk/structures/structures.h"

namespace visuals {
	void render() {
		draw_watermark();
		entity_loop();
	}

	void draw_watermark() {
		if (!settings::misc::watermark) return;

		//settings
		vec2 offsets = vec2(10, 10);
		vec2 offsets_text = vec2(6, 4);
		float bar_size = 4.f;
		float rounding = 4.f;

		vec2 screen_size = utils::get_screen_size();
		std::string watermark_text = utils::snprintf("nullptr | %s | fps: %i", utils::current_date_time().c_str(), null_gui::deep::get_framerate());
		vec2 watermark_text_size = vec2(render::default_font->CalcTextSizeA(12.f, FLT_MAX, 0.0f, watermark_text.c_str()).x, render::default_font->CalcTextSizeA(12.f, FLT_MAX, 0.0f, watermark_text.c_str()).y);

		vec2 bar_start_position  = vec2(screen_size.x - offsets.x - watermark_text_size.x - offsets_text.x, offsets.y);
		vec2 bar_end_position    = vec2(screen_size.x - offsets.x                                         , offsets.y + bar_size);

		vec2 main_start_position = vec2(screen_size.x - offsets.x - watermark_text_size.x - offsets_text.x, offsets.y + bar_size);
		vec2 main_end_position   = vec2(screen_size.x - offsets.x                                         , offsets.y + watermark_text_size.y + bar_size + offsets_text.y);

		render::draw_box_filled_rounded(bar_start_position, bar_end_position, color(globals::menu_color, 255), rounding, 3);  // draw top line
		render::draw_box_filled_rounded(main_start_position, main_end_position, color(50, 50, 50, 100), rounding, 12); // draw watermark body

		render::draw_text(watermark_text, main_start_position + vec2(offsets_text.x / 2, offsets_text.y / 2), color(255,255,255,255), false, false);
	}

	void entity_loop() {
		for (int i = 1; i < sdk::entity_list->get_highest_entity_index(); ++i) {
			c_base_entity* ent = c_base_entity::get_entity_by_index(i);
			if (!ent) continue;
			
			if (ent->get_client_class()->class_id == class_id::c_inferno) {
				c_base_inferno* inferno = (c_base_inferno*)ent;
				if (!inferno) continue;

				render::draw_circle_3d(inferno->vec_origin(), 50, inferno->fire_count() * 10.f, color(255,255,255,255));

				render::draw_text(utils::snprintf("%d : %.0f", inferno->fire_count(), ((float)inferno->fire_count() * 10.f)), vec2(100, 10), color(255, 255, 255, 255), true, false);
			}
		}
	}

	void thirdperson() {
		static observer_mode_t bec_person = observer_mode_t::obs_in_eye;
		static bool setted_person = false;

		if (!sdk::local_player) return;

		if (settings::misc::third_person::bind.enable) {
			if (sdk::local_player->is_alive()) {
				if (!sdk::input->m_fCameraInThirdPerson) {
					sdk::input->m_fCameraInThirdPerson = true;
				}

				float dist = settings::misc::third_person::dist;

				qangle* view = sdk::local_player->get_vangles();
				trace_t tr;
				ray_t ray;

				vec3 desiredCamOffset = vec3(cos(DEG2RAD(view->yaw)) * dist,
					sin(DEG2RAD(view->yaw)) * dist,
					sin(DEG2RAD(-view->pitch)) * dist
				);

				ray.init(sdk::local_player->get_eye_pos(), (sdk::local_player->get_eye_pos() - desiredCamOffset));
				c_trace_filter traceFilter;
				traceFilter.pSkip = sdk::local_player;
				sdk::engine_trace->trace_ray(ray, MASK_SHOT, &traceFilter, &tr);

				vec3 diff = sdk::local_player->get_eye_pos() - tr.endpos;

				float distance2D = sqrt(abs(diff.x * diff.x) + abs(diff.y * diff.y));

				bool horOK = distance2D > (dist - 2.0f);
				bool vertOK = (abs(diff.z) - abs(desiredCamOffset.z) < 3.0f);

				float cameraDistance;

				if (horOK && vertOK) {
					cameraDistance = dist; 
				} else {
					if (vertOK) {
						cameraDistance = distance2D * 0.95f;
					} else {
						cameraDistance = abs(diff.z) * 0.95f;
					}
				}
				sdk::input->m_fCameraInThirdPerson = true;

				sdk::input->m_vecCameraOffset.z = cameraDistance;

				setted_person = false;
			} else {
				if (sdk::local_player->observer_target() != nullptr) {
					if (sdk::local_player->observer_mode() == observer_mode_t::obs_in_eye) {
						bec_person = sdk::local_player->observer_mode();
						setted_person = true;
					}

					if (setted_person) sdk::local_player->observer_mode() = observer_mode_t::obs_chase;
				}

				sdk::input->m_fCameraInThirdPerson = false;
			}
		} else {
			if (!sdk::local_player->is_alive()) {
				if (setted_person) sdk::local_player->observer_mode() = bec_person;
				setted_person = false;
			}
			sdk::input->m_fCameraInThirdPerson = false;
		}
	}
}