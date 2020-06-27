#include "notify.h"
#include "../../settings/settings.h"
#include <map>

struct notify_t {
	std::string s_pre;
	std::string s_body;
	color c_pre;
	color c_body;

	float time_del;
	float time_life = 0;

	float x_body = 0;
	float x_main = 0;
	float y = 0;

	float max_x;
};

std::deque<notify_t> notifications_standart;
std::deque<notify_t> notifications_big;

namespace notify {
	void add(std::string pre, std::string body, log_settings_t log, int life_time) {
		if (log.enable_console) add(pre, body, log.using_custom_color ? log.custm_color : log.using_menu_color ? globals::menu_color : log.color, color(255, 255, 255, 255), life_time, log_type::console);
		if (log.enable_screen) add(pre, body, log.using_custom_color ? log.custm_color : log.using_menu_color ? globals::menu_color : log.color, color(255, 255, 255, 255), life_time, log.screen_type);
	}

	void add(std::string pre, std::string body, color color_pre, bool first, bool two, log_type first_type, log_type two_type, int life_time) {
		if (first) add(pre, body, color_pre, color(255, 255, 255, 255), life_time, first_type);
		if (two)   add(pre, body, color_pre, color(255, 255, 255, 255), life_time, two_type);
	}

	void add(std::string pre, std::string body, color color_pre, color color_body, int life_time, log_type type) {
		switch (type) {
		case console: 
			if (!sdk::engine_client || !sdk::cvar)
				return;

			sdk::cvar->console_dprintf("[ ");
			sdk::cvar->console_color_printf(color_pre, pre.data());
			sdk::cvar->console_dprintf(" ] ");
			sdk::cvar->console_color_printf(color_body, body.data());
			sdk::cvar->console_dprintf("\n");
			break;
		case standart:
			notifications_standart.push_front(notify_t{ pre, body, color_pre, color_body, (float)life_time });
			break;
		case big:
			notifications_big.push_front(notify_t{ "", body, color_pre, color(0,0,0,0), (float)life_time });
			break;
		}
	}

	void add(std::string body, int life_time) {
		add("", body, globals::menu_color, color(255, 255, 255, 255), life_time, log_type::big);
	}

	void add(std::string body) {
		add("", body, globals::menu_color, color(255, 255, 255, 255), 2000.f, log_type::big);
	}

	void add(std::string pre, std::string body) {
		add(pre, body, globals::menu_color, color(255, 255, 255, 255), 2000.f, log_type::standart);
	}

	void add(std::string pre, std::string body, int life_time) {
		add(pre, body, globals::menu_color, color(255, 255, 255, 255), life_time, log_type::standart);
	}

	void add(std::string pre, std::string body, color color_pre, color color_body) {
		add(pre, body, color_pre, color_body, 2000.f, log_type::standart);
	}

	void add(std::string pre, std::string body, color color_pre, int life_time, log_type type) {
		add(pre, body, color_pre, color(255, 255, 255, 255), life_time, type);
	}
	void add(std::string pre, std::string body, color color_pre, log_type type)
	{
		add(pre, body, color_pre, color(255, 255, 255, 255), 2000.f, type);
	}
	void add(std::string pre, std::string body, log_type type)
	{
		add(pre, body, globals::menu_color, color(255, 255, 255, 255), 2000.f, type);
	}

	void render()
	{
		int screen_w, screen_h;

		screen_w = utils::get_screen_size().x;
		screen_h = utils::get_screen_size().y;

		if (!notifications_standart.empty()) {
			for (size_t i = 0; i < notifications_standart.size(); i++) {
				auto& notify = notifications_standart.at(i);

				int left_size = render::default_font->CalcTextSizeA(12.f, FLT_MAX, 0.f, "[ ").x;
				int right_size = render::default_font->CalcTextSizeA(12.f, FLT_MAX, 0.f, " ]").x;
				int space_size = render::default_font->CalcTextSizeA(12.f, FLT_MAX, 0.f, "  ").x;

				std::string all_text = utils::snprintf("[ %s ] %s", notify.s_pre.c_str(), notify.s_body.c_str());

				ImVec2 pre_size = render::default_font->CalcTextSizeA(12.f, FLT_MAX, 0.0f, notify.s_pre.c_str());
				ImVec2 all_text_size = render::default_font->CalcTextSizeA(12.f, FLT_MAX, 0.f, all_text.c_str());
				notify.max_x = all_text_size.x + 10;

				notify.y = utils::lerp(notify.y, i * (all_text_size.y + 3), 0.03f);

				if (notify.y > screen_h + 10 || notify.x_main < -3) {
					notifications_standart.erase(notifications_standart.begin() + i);
					continue;
				}

				if (notify.time_life == 0 || notify.time_life >= utils::epoch_time()) {
					notify.x_main = utils::lerp(notify.x_main, notify.max_x + 5, 0.06f);

					if (notify.x_main > notify.max_x - (notify.max_x / 3)) {
						notify.x_body = utils::lerp(notify.x_body, notify.max_x + 3, 0.05f);

						if (notify.x_body >= notify.max_x && notify.time_life == 0) {
							notify.time_life = utils::epoch_time() + notify.time_del;
						}
					}
				} else {
					notify.x_body = utils::lerp(notify.x_body, -20, 0.06f);

					if (notify.x_body < notify.max_x / 3) {
						notify.x_main = utils::lerp(notify.x_main, -20, 0.07f);
					}
				}

				int procent_x = std::clamp((100 * notify.x_body) / notify.max_x, 0.f, 100.f);
				int opacity = (255.f / 100.f) * procent_x;

				notify.c_body = color(notify.c_body, opacity);
				notify.c_pre = color(notify.c_pre, opacity);

				float start_text = notify.x_body - all_text_size.x - 7.f;

				render::draw_box_filled(0.f, notify.y, notify.x_body, notify.y + all_text_size.y + 3, color(0, 0, 0, 100));
				render::draw_box_filled(notify.x_body, notify.y, notify.x_main, notify.y + all_text_size.y + 3, color(notify.c_pre, 255));

				render::draw_text("[", vec2(start_text, notify.y + 2.f), color(255, 255, 255, 255), false, false);
				render::draw_text(" ]", vec2(start_text + pre_size.x + left_size + 1, notify.y + 1), color(255, 255, 255, 255), false, false);
				render::draw_text(notify.s_pre, vec2(start_text + left_size + 1, notify.y + 2), notify.c_pre, false, false);
				render::draw_text(notify.s_body, vec2(start_text + left_size + right_size + space_size + pre_size.x + 2, notify.y + 1), notify.c_body, false, false);
			}
		}

		if (!notifications_big.empty()) {
			for (size_t i = 0; i < notifications_big.size(); i++) {
				auto& notify = notifications_big.at(i);

				float round = ImGui::GetStyle().FrameRounding;

				ImVec2 text_size = render::default_font->CalcTextSizeA(12.f, FLT_MAX, 0.0f, notify.s_body.c_str());

				float box_size_x = 50.f + text_size.x;
				float box_size_y = 40.f;

				float procent_x = 100;

				if (notify.x_body == 0 && notify.y == 0) {
					notify.x_body = screen_w + box_size_x;
					notify.y = screen_h + box_size_y;
				}
	
				float start_pos_x = screen_w - 100.f;
				float start_pos_y = screen_h - (i * box_size_y) - (i * 15.f) - 50.f;

				if (notify.time_life == 0 || notify.time_life >= utils::epoch_time()) {
					notify.x_body = utils::lerp(notify.x_body, start_pos_x, 0.03f);
					notify.y = utils::lerp(notify.y, start_pos_y, 0.03f);

					if (notify.time_life == 0 && notify.x_body < start_pos_x + 2 && notify.y < start_pos_y + 2)
						notify.time_life = utils::epoch_time() + notify.time_del;
				} else {
					notify.x_body = utils::lerp(notify.x_body, screen_w + box_size_x + 40, 0.03f);
				}


				if (notify.x_body > screen_w + box_size_x + 4 || notify.y < 0) {
					notifications_big.erase(notifications_big.begin() + i);
					continue;
				}

				if (notify.time_life != 0) {
					procent_x = (100 * (notify.time_life - utils::epoch_time())) / notify.time_del;
				}

				float opacity = std::clamp((box_size_y / 100.f) * procent_x, 0.f, box_size_y);

				render::draw_box_filled_rounded(notify.x_body - box_size_x,       notify.y - box_size_y, notify.x_body, notify.y, color(50, 50, 50, 200), round, 10);
				render::draw_box_filled_rounded(notify.x_body - box_size_x - 4.f, notify.y - box_size_y, notify.x_body - box_size_x, notify.y, color(notify.c_pre, 255), round, 5);
				render::draw_box_filled        (notify.x_body - box_size_x,       notify.y - opacity, notify.x_body - box_size_x + 2.f, notify.y, color(notify.c_pre, 255));

				render::draw_text(notify.s_body, notify.x_body - (box_size_x / 2), notify.y - (box_size_y / 2) - (text_size.y / 2), color(255, 255, 255, 255), false, true);
			}
		}

	}
}