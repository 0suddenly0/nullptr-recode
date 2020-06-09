#pragma once
#include <string>
#include <deque>
#include "../../settings/globals.h"
#include "../../sdk/sdk.h"
#include "../../render/render.h"

enum log_type {
	standart = 0,
	big = 1	
};

namespace notify {
	void add(std::string pre, std::string body, color color_pre, color color_body, int life_time, log_type type);
	void add(std::string pre, std::string body, color color_pre, int life_time, log_type type);
	void add(std::string pre, std::string body, color color_pre, log_type type);
	void add(std::string pre, std::string body, log_type type);
	void add(std::string pre, std::string body, color color_pre, color color_body = color(255,255,255,255));
	void add(std::string pre, std::string body, int life_time);
	void add(std::string pre, std::string body);
	void add(std::string body, int life_time);
	void add(std::string body);

	void render();
}