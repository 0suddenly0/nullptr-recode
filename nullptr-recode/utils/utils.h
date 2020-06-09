#pragma once
#include <Windows.h>
#include <Windows.h>
#include <stdio.h>
#include <vadefs.h>
#include <chrono>
#include <string>
#include <vector>
#include <iostream>
#include "../helpers/helpers.h"
#include "../gui/nullptr_gui.h"
#include "../sdk/sdk.h"
#include "../sdk/misc/datamap.h"

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

namespace utils {
	vec2 get_screen_size();

	std::string snprintf(const char* text, ...);

	void attach_console();
	void detach_console();

	char console_read_key();
	bool console_print(const char* fmt, ...);

	unsigned int find_in_data_map(datamap_t* p_map, const char* name);
	int wait_for_modules(std::int32_t timeout, const std::initializer_list<std::wstring>& modules);
	std::uint8_t* pattern_scan(void* module, const char* signature);

	std::string current_date_time();
	int epoch_time();
	float lerp(float a, float b, float f);
}