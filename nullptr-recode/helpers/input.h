#pragma once

#define NOMINMAX
#include <Windows.h>
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <stdexcept>
#include <d3d9.h>

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

    key_state       key_map[256];

    std::function<void(void)> hot_keys[256];

    key_state      get_key_state(uint32_t vk);
    bool          is_key_down(uint32_t vk);
    bool          was_key_pressed(uint32_t vk);

    void register_hotkey(uint32_t vk, std::function<void(void)> f);
    void remove_hotkey(uint32_t vk);

    bool process_message(UINT uMsg, WPARAM wParam, LPARAM lParam);
    bool process_mouse_message(UINT uMsg, WPARAM wParam, LPARAM lParam);
    bool process_keybd_message(UINT uMsg, WPARAM wParam, LPARAM lParam);
}