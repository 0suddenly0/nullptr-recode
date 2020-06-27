#pragma once
#include <string>
#include <deque>
#include "../../sdk/sdk.h"
#include "../../utils/utils.h"
#include "../../render/render.h"

enum log_type {
	console = 0,
	standart = 1,
	big = 2	
};

struct log_settings_t {
	bool enable_screen;
	bool enable_console;

	bool using_menu_color;
	bool using_custom_color;
	color custm_color;
	color color;

	log_type screen_type;
};

namespace notify {
	void add(std::string pre, std::string body, color color_pre, bool first, bool two, log_type first_type, log_type two_type, int life_time = 7500);
	void add(std::string pre, std::string body, color color_pre, color color_body, int life_time, log_type type);
	void add(std::string pre, std::string body, color color_pre, int life_time, log_type type);
	void add(std::string pre, std::string body, log_settings_t log, int life_time = 7500);
	void add(std::string pre, std::string body, color color_pre, log_type type);
	void add(std::string pre, std::string body, log_type type);
	void add(std::string pre, std::string body, color color_pre, color color_body = color(255,255,255,255));
	void add(std::string pre, std::string body, int life_time);
	void add(std::string pre, std::string body);
	void add(std::string body, int life_time);
	void add(std::string body);

	void render();
}