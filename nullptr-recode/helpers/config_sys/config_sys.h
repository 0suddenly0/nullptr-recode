#pragma once
#include "../math/math.h"
#include "../../utils/utils.h"
#include "../input.h"
#include "../../settings/settings.h"
#include <string>
#include <vector>

namespace config {
    enum class var_t {
        _int = 0,
        _float = 1,
        _bool = 2,
        _string = 3,
        _color = 4,
        _bind = 5,
        _window = 6
    };

    enum class line_t {
        group = 0,
        var = 1
    };

    namespace config_utils {
        std::string del_last_simbols(std::string* string, int num);
        std::string del_first_simbols(std::string* string, int num);
        std::string del_last_simbols(std::string string, int num);
        std::string del_first_simbols(std::string string, int num);
        void load_group(std::vector<std::string>& groups_list, std::string line);
        std::string get_var_name(std::string line);
        std::string get_var_value(std::string line);
        line_t get_line_type(std::string line);
    }

    class c_var {
    public:
        template <typename T>
        c_var(std::string _name, T* var, std::vector<std::string> last_groups) {
            for (std::string group : last_groups) {
                name += group;
            }
            name += _name;

            if (std::is_same<T, int>::value) {
                type = var_t::_int;
                var_int = (int*)var;
            } else if (std::is_same<T, float>::value) {
                type = var_t::_float;
                var_float = (float*)var;
            } else if (std::is_same<T, bool>::value) {
                type = var_t::_bool;
                var_bool = (bool*)var;
            } else if (std::is_same<T, std::string>::value) {
                type = var_t::_string;
                var_string = (std::string*)var;
            } else if (std::is_same<T, color>::value) {
                type = var_t::_color;
                var_color = (color*)var;
            } else if (std::is_same<T, key_bind_t>::value) {
                type = var_t::_bind;
                var_bind = (key_bind_t*)var;
            } else if (std::is_same<T, window_settings_t>::value) {
                type = var_t::_window;
                var_window = (window_settings_t*)var;
            }
        }

        static bool find_var(std::vector<c_var>* var_list, std::string line, std::string full_var_name) {
            for (int i = 0; i < var_list->size(); i++) {
                c_var& var = var_list->at(i);
                std::string var_value = config_utils::get_var_value(line);

                if (var.name == full_var_name) {
                    var.load(var_value);
                    var_list->erase(var_list->begin() + i);
                    return true;
                }
            }
            return false;
        }

        void load(std::string var_value) {
            switch (type) {
            case var_t::_float: {
                *var_float = std::stof(var_value);
            } break;
            case var_t::_int: {
                *var_int = std::stoi(var_value);
            } break;
            case var_t::_bool: {
                *var_bool = var_value == "true" ? true : false;
            } break;
            case var_t::_string: {
                config_utils::del_last_simbols(&var_value, 1);
                config_utils::del_first_simbols(&var_value, 1);
                *var_string = var_value;
            } break;
            case var_t::_color: {
                std::string red_string = config_utils::del_last_simbols(var_value, var_value.length() - var_value.find("-"));
                std::string green_string = config_utils::del_first_simbols(var_value, red_string.length() + 1);
                config_utils::del_last_simbols(&green_string, green_string.length() - green_string.find("-"));
                std::string blue_string = config_utils::del_first_simbols(var_value, red_string.length() + green_string.length() + 2);
                config_utils::del_last_simbols(&blue_string, blue_string.length() - blue_string.find("-"));
                std::string alpha_string = config_utils::del_first_simbols(var_value, red_string.length() + green_string.length() + blue_string.length() + 3);

                *var_color = color(std::stoi(red_string), std::stoi(green_string), std::stoi(blue_string), std::stoi(alpha_string));
            } break;
            case var_t::_bind: {
                std::string key_string = config_utils::del_last_simbols(var_value, var_value.length() - var_value.find("-"));
                std::string type_string = config_utils::del_first_simbols(var_value, key_string.length() + 1);
                config_utils::del_last_simbols(&type_string, type_string.length() - type_string.find("-"));
                std::string state_string = config_utils::del_first_simbols(var_value, key_string.length() + type_string.length() + 2);

                *var_bind = key_bind_t{ std::stoi(key_string), std::stoi(type_string), state_string == "true" ? true : false };
            } break;
            case var_t::_window: {
                std::string show_string = config_utils::del_last_simbols(var_value, var_value.length() - var_value.find("-"));
                std::string pos_x_string = config_utils::del_first_simbols(var_value, show_string.length() + 1);
                config_utils::del_last_simbols(&pos_x_string, pos_x_string.length() - pos_x_string.find("-"));
                std::string pos_y_string = config_utils::del_first_simbols(var_value, show_string.length() + pos_x_string.length() + 2);
                config_utils::del_last_simbols(&pos_y_string, pos_y_string.length() - pos_y_string.find("-"));
                std::string alpha_string = config_utils::del_first_simbols(var_value, show_string.length() + pos_x_string.length() + pos_y_string.length() + 3);

                null_gui::deep::set_window_pos(var_window->name.c_str(), vec2(std::stoi(pos_x_string), std::stoi(pos_y_string)), false);
                *var_window = window_settings_t{ var_window->name, std::stoi(alpha_string), vec2(std::stoi(pos_x_string), std::stoi(pos_y_string)), show_string == "true" ? true : false };
            } break;
            }
        }

        std::string name;

        var_t type;

        // int
        int* var_int;
        // float
        float* var_float;
        // bool
        bool* var_bool;
        // string
        std::string* var_string;
        // color
        color* var_color;
        // bind
        key_bind_t* var_bind;
        //window
        window_settings_t* var_window;
    };


    class c_config {
    public:
        c_config(std::string dir) {
            dir_name = dir;
        }

        void clear() {
            cur_group = 0;
            config_text.clear();
            vars.clear();
            last_groups.clear();
        }

        void add_space() {
            for (int i = 0; i < cur_group; i++) {
                if (i == cur_group - 1) config_text += "|- ";
                else config_text += "|  ";
            }
        }

        template<typename T>
        void add_var(std::string name, T& var) {
            c_var _var = c_var{ name, &var, last_groups };
            vars.push_back(_var);
            switch (vars.back().type) {
            case var_t::_float:
                config_text += utils::snprintf("%s = %.3f;\n", name.c_str(), *_var.var_float);
                break;
            case var_t::_int:
                config_text += utils::snprintf("%s = %d;\n", name.c_str(), *_var.var_int);
                break;
            case var_t::_bool:
                config_text += utils::snprintf("%s = %s;\n", name.c_str(), *_var.var_bool == true ? "true" : "false");
                break;
            case var_t::_string:
                config_text += utils::snprintf("%s = '%s';\n", name.c_str(), _var.var_string->c_str());
                break;
            case var_t::_color:
                config_text += utils::snprintf("%s = %d-%d-%d-%d;\n", name.c_str(), _var.var_color->r(), _var.var_color->g(), _var.var_color->b(), _var.var_color->a());
                break;
            case var_t::_bind:
                config_text += utils::snprintf("%s = %d-%d-%s;\n", name.c_str(), _var.var_bind->key_id, _var.var_bind->bind_type, _var.var_bind->enable ? "true" : "false");
                break;
            case var_t::_window:
                config_text += utils::snprintf("%s = %s-%.0f-%.0f-%d;\n", name.c_str(), _var.var_window->show ? "true" : "false", _var.var_window->pos.x, _var.var_window->pos.y, _var.var_window->alpha);
                break;
            }
        }

        void add_group(std::string name) {
            config_text += utils::snprintf("[ %s ]\n", name.c_str());
            last_groups.push_back(utils::snprintf("[ %s ] ", name.c_str()));
            cur_group++;
        }

        void end_group() {
            last_groups.pop_back();
            cur_group--;
            add_space();
            config_text += "[ end ]\n";
        }

        bool load(std::string name) {
            if (name.empty()) return false;

            CreateDirectoryA((LPCSTR)utils::snprintf("C:\\nullptr\\%s\\", dir_name.c_str()).c_str(), NULL);

            std::vector<c_var> local_vars = vars;
            std::vector<std::string> last_groups;
            std::string line;
            std::ifstream in(utils::snprintf("C:\\nullptr\\%s\\%s.null", dir_name.c_str(), name.c_str()));

            if (in.is_open()) {
                while (getline(in, line)) {
                    if (config_utils::get_line_type(line) == line_t::group) {
                        config_utils::load_group(last_groups, line);
                    } else if(local_vars.size() > 0){
                        std::string becup = line;
                        std::string end_var_name;
                        for (std::string group : last_groups) {
                            end_var_name = utils::snprintf("%s%s ", end_var_name.c_str(), group.c_str());
                        }
                        end_var_name = utils::snprintf("%s%s", end_var_name.c_str(), config_utils::get_var_name(line).c_str());
                        std::string var_value = config_utils::get_var_value(line);
                        if (local_vars.front().name == end_var_name) {
                            c_var& var = local_vars.front();
                            var.load(var_value);
                            local_vars.erase(local_vars.begin());
                        } else {
                            c_var::find_var(&local_vars, line, end_var_name);
                        }
                    }
                }
            }
            in.close();
            return true;
        }

        std::string dir_name;

        std::vector<std::string> last_groups;
        std::string config_text;
        std::vector<c_var> vars;

        int cur_group = 0;
    };

    namespace standart {
        c_config _config = c_config("standart");
        void setup();

        bool save(std::string name);
        bool load(std::string name);
    }

    namespace skins {
        c_config _config = c_config("skins");
        void setup();

        bool save(std::string name);
        bool load(std::string name);
    }

    void push_config(c_config* cfg);
    void add_group(std::string name);
    void end_group();

    void add_var(std::string name, int& var);
    void add_var(std::string name, float& var);
    void add_var(std::string name, bool& var);
    void add_var(std::string name, std::string& var);
    void add_var(std::string name, color& var);
    void add_var(std::string name, key_bind_t& var);
    void add_var(std::string name, window_settings_t& var);
}