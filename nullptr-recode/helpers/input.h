#pragma once

#define NOMINMAX
#include <Windows.h>
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <stdexcept>
#include <d3d9.h>

typedef int bind_info_flags;

enum bind_info_flags_
{
	bind_info_flags_none = 0,
	bind_info_flags_standart = 1 << 0,
	bind_info_flags_side = 1 << 1
};

struct bind_info
{
	std::string name;
	bool enable;
	bind_info_flags flag;
};

enum key_bind_type
{
    always = 0,
    press = 1,
    press_invers = 2,
    toggle = 3
};


struct key_bind_t {
    int key_id = 0;
    int bind_type = 0;
    bool enable = false;
};

enum class key_state {
    none = 1,
    down,
    up,
    pressed /*Down and then up*/
};

DEFINE_ENUM_FLAG_OPERATORS(key_state);

namespace input {

    std::vector<bind_info*> binds;
    key_state       key_map[256];

    std::function<void(void)> hot_keys[256];

    void create_binds();

    key_state      get_key_state(uint32_t vk);
    bool          is_key_down(uint32_t vk);
    bool          was_key_pressed(uint32_t vk);

    void register_hotkey(uint32_t vk, std::function<void(void)> f);
    void remove_hotkey(uint32_t vk);

    bool process_message(UINT uMsg, WPARAM wParam, LPARAM lParam);
    bool process_mouse_message(UINT uMsg, WPARAM wParam, LPARAM lParam);
    bool process_keybd_message(UINT uMsg, WPARAM wParam, LPARAM lParam);

	bind_info* find_bind_info(std::string name);
	bind_info* create_bind_info(std::string name, bool state, bind_info_flags flag);
	void delete_bind_info(std::string name);
    void create_bind(std::string name, bool enable, key_bind_t* bind, bind_info_flags flag);
	void create_bind(std::string name, bool enable, key_bind_t* bind, float* side, bind_info_flags flag);
}