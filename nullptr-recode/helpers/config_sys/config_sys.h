#pragma once
#include "../math/math.h"
#include "../../utils/utils.h"
#include "../../gui/menu/menu.h"
#include "../input.h"
#include <string>
#include <vector>

class int_v {
public:
	std::string text_full;
	std::string text;
	int& v;
	int line_id;
	int group_id;
};

class float_v {
public:
	std::string text_full;
	std::string text;
	float& v;
	int line_id;
	int group_id;
};

class bool_v {
public:
	std::string text_full;
	std::string text;
	bool& v;
	int line_id;
	int group_id;
};

class color_v {
public:
	std::string text_full;
	std::string text;
	color& v;
	int line_id;
	int group_id;
};

class bind_v {
public:
	std::string text_full;
	std::string text;
	key_bind_t& v;
	int line_id;
	int group_id;
};

class string_v {
public:
	std::string text_full;
	std::string text;
	std::string& v;
	int line_id;
	int group_id;
};

class window_v {
public:
	std::string text_full;
	std::string text;
	std::string window_name;
	float& x;
	float& y;
	int line_id;
	int group_id;
};

namespace config {
	namespace config_utils {
		std::string split_line(std::string temp);

		void add_var(int& cur_line, int& cur_group, std::vector<std::string>& config, std::vector<int_v>& vars, std::string name, int& var);
		void add_var(int& cur_line, int& cur_group, std::vector<std::string>& config, std::vector<bool_v>& vars, std::string name, bool& var);
		void add_var(int& cur_line, int& cur_group, std::vector<std::string>& config, std::vector<float_v>& vars, std::string name, float& var);
		void add_var(int& cur_line, int& cur_group, std::vector<std::string>& config, std::vector<color_v>& vars, std::string name, color& var);
		void add_var(int& cur_line, int& cur_group, std::vector<std::string>& config, std::vector<bind_v>& vars, std::string name, key_bind_t& var);
		void add_var(int& cur_line, int& cur_group, std::vector<std::string>& config, std::vector<string_v>& vars, std::string name, std::string& var);
		void add_var(int& cur_line, int& cur_group, std::vector<std::string>& config, std::vector<window_v>& vars, std::string name, std::string wind, float& x, float& y);

		void add_space(int& cur_group, std::vector<std::string>& config);

		void add_group(int& cur_line, int& cur_group, std::vector<std::string>& config, std::string name);
		void end_group(int& cur_group);

		bool save(std::string path, std::vector<std::string>& config, std::string name);
		bool load(std::string path, std::string name, std::vector<int_v>& ints, std::vector<bool_v>& bools, std::vector<float_v>& floats, std::vector<color_v>& colors, std::vector<bind_v>& binds, std::vector<string_v>& strings, std::vector<window_v>& windows);
	}

	class c_config {
	public:
		c_config(std::string _dir_name) {
			dir_name = _dir_name;
		}

		void clear() {
			config.clear();

			ints.clear();
			bools.clear();
			binds.clear();
			floats.clear();
			colors.clear();
			strings.clear();
			windows.clear();

			cur_line = 0;
			cur_group = 0;
		}

		std::string dir_name;

		int cur_group = 0;
		int cur_line = 0;

		std::vector<std::string> config;

		std::vector<int_v> ints;
		std::vector<bind_v> binds;
		std::vector<bool_v> bools;
		std::vector<float_v> floats;
		std::vector<color_v> colors;
		std::vector<string_v> strings;
		std::vector<window_v> windows;
	};

	namespace standart {
		config::c_config _config = config::c_config("standart");

		void setup_vars();
		bool save(std::string name);
		bool load(std::string name);
	}

	namespace skins {
		config::c_config _config = config::c_config("skins");

		void setup_vars();
		bool save(std::string name);
		bool load(std::string name);
	}

	void push_cur_config(c_config* cfg);
	void add_var(std::string name, int& var);
	void add_var(std::string name, bool& var);
	void add_var(std::string name, float& var);
	void add_var(std::string name, color& var);
	void add_var(std::string name, key_bind_t& var);
	void add_var(std::string name, std::string& var);
	void add_var(std::string name, std::string wind, float& x, float& y);
	void add_group(std::string name);
	void end_group();
}