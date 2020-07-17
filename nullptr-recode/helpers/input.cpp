#include "input.h"
#include "../gui/nullptr_gui.h"
#include "../settings/settings.h"
#include "../sdk/sdk.h"

bool is_mouse(int vk) {
	if (vk == 1) return true;
	else if (vk == 2) return true;
	else if (vk == 4) return true;
	else if (vk == 5) return true;
	else if (vk == 6) return true;

	return false;
}

bool mouse_key(int vk, bool down) {
	if (down) {
		if (vk == 1) return ImGui::IsMouseDown(0);
		else if (vk == 2) return ImGui::IsMouseDown(1);
		else if (vk == 4) return ImGui::IsMouseDown(2);
		else if (vk == 5) return ImGui::IsMouseDown(3);
		else if (vk == 6) return ImGui::IsMouseDown(4);
	} else {
		if (vk == 1) return ImGui::IsMouseReleased(0);
		else if (vk == 2) return ImGui::IsMouseReleased(1);
		else if (vk == 4) return ImGui::IsMouseReleased(2);
		else if (vk == 5) return ImGui::IsMouseReleased(3);
		else if (vk == 6) return ImGui::IsMouseReleased(4);
	}

	return false;
}


namespace input {
	void create_binds() {
		create_bind("prepare revolver", settings::misc::prepare_revolver::enable, &settings::misc::prepare_revolver::bind, bind_info_flags_standart);
		create_bind("block bot", settings::misc::block_bot::enable, &settings::misc::block_bot::bind, bind_info_flags_standart);
		create_bind("edge jump", settings::misc::edge_jump::enable, &settings::misc::edge_jump::bind, bind_info_flags_standart);
		create_bind("third person", settings::misc::third_person::enable, &settings::misc::third_person::bind, bind_info_flags_standart);
		create_bind("esp", settings::visuals::esp::using_bind, &settings::visuals::esp::bind, bind_info_flags_standart);
		create_bind("chams", settings::visuals::chams::using_bind, &settings::visuals::chams::bind, bind_info_flags_standart);
	}

	bool process_message(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		switch (uMsg) {
		case WM_MBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
		case WM_LBUTTONDBLCLK:
		case WM_XBUTTONDBLCLK:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_LBUTTONDOWN:
		case WM_XBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
		case WM_LBUTTONUP:
		case WM_XBUTTONUP:
			return process_mouse_message(uMsg, wParam, lParam);
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
			return process_keybd_message(uMsg, wParam, lParam);
		default:
			return false;
		}
	}

	bool process_mouse_message(UINT uMsg, WPARAM wParam, LPARAM lParam) {
		auto key = VK_LBUTTON;
		auto state = key_state::none;
		switch (uMsg) {
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
			state = uMsg == WM_MBUTTONUP ? key_state::up : key_state::down;
			key = VK_MBUTTON;
			break;
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			state = uMsg == WM_RBUTTONUP ? key_state::up : key_state::down;
			key = VK_RBUTTON;
			break;
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
			state = uMsg == WM_LBUTTONUP ? key_state::up : key_state::down;
			key = VK_LBUTTON;
			break;
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
			state = uMsg == WM_XBUTTONUP ? key_state::up : key_state::down;
			key = (HIWORD(wParam) == XBUTTON1 ? VK_XBUTTON1 : VK_XBUTTON2);
			break;
		default:
			return false;
		}

		if (state == key_state::up && key_map[key] == key_state::down)
			key_map[key] = key_state::pressed;
		else
			key_map[key] = state;
		return true;
	}

	bool process_keybd_message(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		auto key = wParam;
		auto state = key_state::none;

		switch (uMsg) {
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			state = key_state::down;
			break;
		case WM_KEYUP:
		case WM_SYSKEYUP:
			state = key_state::up;
			break;
		default:
			return false;
		}

		if (state == key_state::up && key_map[int(key)] == key_state::down) {
			key_map[int(key)] = key_state::pressed;

			auto& hotkey_callback = hot_keys[key];

			if (hotkey_callback)
				hotkey_callback();

		} else {
			key_map[int(key)] = state;
		}

		return true;
	}

	key_state get_key_state(std::uint32_t vk) {
		return key_map[vk];
	}

	bool is_key_down(std::uint32_t vk) {
		if (is_mouse(vk)) {
			return mouse_key(vk, true);
		} else {
			return ImGui::IsKeyDown(vk);
		}
	}

	bool was_key_pressed(std::uint32_t vk) {
		if (is_mouse(vk)) {
			return mouse_key(vk, false);
		} else {
			return ImGui::IsKeyPressed(vk, false);
		}
	}

	void register_hotkey(std::uint32_t vk, std::function<void(void)> f) {
		hot_keys[vk] = f;
	}

	void remove_hotkey(std::uint32_t vk) {
		hot_keys[vk] = nullptr;
	}

	bind_info* find_bind_info(std::string name) {
		for (int i = 0; i < binds.size(); i++) {
			if (binds[i]->name == name) {
				return binds[i];
			}
		}
		return NULL;
	}

	bind_info* create_bind_info(std::string name, key_bind_t* bind, bind_info_flags flag) {
		bind_info* bind_find = find_bind_info(name);

		if (bind_find != NULL) {
			bind_find->bind->enable = bind->enable;
			return bind_find;
		}

		bind_info* bind_inf = new bind_info{ name, bind, flag };
		binds.push_back(bind_inf);
		return bind_inf;
	}

	void delete_bind_info(std::string name) {
		for (int i = 0; i < binds.size(); i++) {
			if (binds[i]->name == name) {
				binds.erase(binds.begin() + i);
			}
		}
	}
	
	void create_bind(std::string name, bool enable, key_bind_t* bind, bind_info_flags flag) {
		if (enable) {
			bind_info* bindinfo = create_bind_info(name, bind, flag);

			if (bind->bind_type == key_bind_type::always) {
				bind->enable = true;
			} else if (bind->bind_type == key_bind_type::toggle) {
				if (was_key_pressed(bind->key_id)) {
					bind->enable = !bind->enable;
				}
			}
			else if (bind->bind_type == key_bind_type::press) {
				if (is_key_down(bind->key_id)) {
					bind->enable = true;
				} else if (!is_key_down(bind->key_id)) {
					bind->enable = false;
				}
			} else if (bind->bind_type == key_bind_type::press_invers) {
				if (is_key_down(bind->key_id)) {
					bind->enable = false;
				} else if (!is_key_down(bind->key_id)) {
					bind->enable = true;
				}
			}

			if (bind->key_id == 0 && bind->bind_type != key_bind_type::always) bind->enable = false;
		} else if (!enable) {
			delete_bind_info(name);
			bind->enable = false;
		}
	}

	void create_bind(std::string name, bool enable, key_bind_t* bind, float* side, bind_info_flags flag) {
		if (enable) {
			bind_info* bindinfo = create_bind_info(name, bind, flag);

			if (was_key_pressed(bind->key_id)) {
				bind->enable = !bind->enable;

				*side = -*side;
			}

		} else if (!enable) {
			delete_bind_info(name);
		}
	}
}