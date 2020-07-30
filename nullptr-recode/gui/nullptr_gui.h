#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include <string>
#include <vector>
#include "../helpers/helpers.h"

enum class tabs_type {
    standart = 0,
    big = 1,
    spac = 2,
    spac_child = 3
};

template<size_t N>
void render_tabs(char* (&names)[N], int& activetab, float w, float h) {
    bool values[N] = { false };
    values[activetab] = true;
    for (auto i = 0; i < N; ++i) {
        if (i == 0) {
            if (ImGui::ToggleButton(names[i], &values[i], ImVec2{ w, h }, 1))
                activetab = i;
        } else if (i == N - 1) {
            if (ImGui::ToggleButton(names[i], &values[i], ImVec2{ w, h }, 2))
                activetab = i;
        } else {
            if (ImGui::ToggleButton(names[i], &values[i], ImVec2{ w, h }, 0))
                activetab = i;
        }
        if (i < N - 1) ImGui::SameLine();
    }
}

template<size_t N>
void render_tabsMain(char* (&names)[N], int& activetab, float w, float h, bool in_child) {
    bool values[N] = { false };
    values[activetab] = true;
    for (auto i = 0; i < N; ++i) {
        if (i == 0) {
            if (ImGui::ToggleButtonMain(names[i], &values[i], ImVec2{ w, h }, 1, in_child))
                activetab = i;
        } else if (i == N - 1) {
            if (ImGui::ToggleButtonMain(names[i], &values[i], ImVec2{ w, h }, 2, in_child))
                activetab = i;
        } else {
            if (ImGui::ToggleButtonMain(names[i], &values[i], ImVec2{ w, h }, 0, in_child))
                activetab = i;
        }
        if (i < N - 1) ImGui::SameLine();
    }
}

template<size_t N>
void render_tabs2(char* (&names)[N], int& activetab, float w, float h, bool Keybind) {
    bool values[N] = { false };

    values[activetab] = true;

    for (auto i = 0; i < N; ++i) {
        if (i == 0) {
            if (ImGui::ToggleButtonSelect(names[i], &values[i], ImVec2{ w, h }, 1, Keybind))
                activetab = i;
        } else if (i == N - 1) {
            if (ImGui::ToggleButtonSelect(names[i], &values[i], ImVec2{ w, h }, 2, Keybind))
                activetab = i;
        } else {
            if (ImGui::ToggleButtonSelect(names[i], &values[i], ImVec2{ w, h }, 0, Keybind))
                activetab = i;
        }

    }
}

namespace null_gui {
    namespace deep {
        int get_framerate();
        void set_cursor_pos(vec2 pos);
        void set_cursor_x(float x);
        void set_cursor_y(float y);
        vec2 get_scroll();
        void set_next_window_pos(vec2 pos, bool once = true);
        void set_next_window_size(vec2 size, bool once = true);
        void set_menu_color(color clr);
        void set_window_pos(vec2 pos);
        void set_window_pos(const char* name, vec2 pos, bool once = true);
        void set_window_size(vec2 size);
        void set_window_hidden(bool hidden);
        void set_next_window_alpha(float alpha);
        void set_next_window_focus();

        void get_window_pos_and_size(vec2* pos, vec2* size);
        vec2 get_window_pos();
        vec2 get_window_size();
    }

    void text(const char* text, ...);
    void text_no_space(const char* text, ...);
    bool check_box(const char* text, bool* value);
    // if max_size = true, button.size.x = window.size.x
    bool button(const char* text, bool max_size = true, vec2 size = vec2(0, 0), bool use_space = true);
    // if u need to use tooltip_items with key_bind, set tooltip to true
    bool key_bind(const char* text, key_bind_t* bind, bool tooltip = false);
    bool slider_int(const char* text, int* value, int min, int max, const char* format = "%d");
    bool slider_float(const char* text, float* value, float min, float max, const char* format = "%.1f", float power = 1.f);
    bool int_input(const char* text, int* value, int step_low = 1, int step_big = 100);
    bool float_input(const char* text, float* value, float step_low = 1.f, float step_big = 100.f, const char* format = "%.1f");
    bool color_edit(const char* text, color* color, bool alpha = true);
    bool text_input(const char* text, std::string* value);
    void image(IDirect3DTexture9* texture, vec2 min, vec2 max);
    bool combo(const char* text, int* value, std::vector<std::string>& items);
    bool combo(const char* label, int* current_item, bool(*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count);
    bool functional_combo(const char* text, const char* prev_item, std::function<void()> function);
    bool multi_combo(const char* text, std::vector<std::string> names, std::vector<bool>* values);
    bool multi_combo(const char* text, std::vector<std::string> names, std::vector<bool*> values);
    template<std::size_t SIZE>
    bool multi_combo(const char* text, std::vector<std::string> names, std::array<bool, SIZE>* values) {
        std::vector<bool*> _values;
        for (int i = 0; i < values->size(); i++) {
            _values.push_back(&values->at(i));
        }

        return multi_combo(text, names, _values);
    }
    void tooltip_items(const char* text, std::function<void()> func);
    void tooltip_on_click(const char* text, std::function<void()> func);
    void tooltip(const char* text, ...);
    void line(float size = 0.f);
    bool begin_list_box(const char* text, vec2 size = vec2(0, 0));
    bool selectable(const char* text, bool selected = false);
    void end_list_box();

    bool begin_window(const char* text, bool* open = NULL, vec2 size = vec2(0, 0), ImGuiWindowFlags flags = 0);
    bool begin_group(const char* text, vec2 size = vec2(0, 0));
    bool begin_menu_bar();
    void end_menu_bar();
    void end_group();
    void end_window();

    void same_line(float x = -1.f);
    void new_line(float y = 0.f);
    void create_columns(int count);
    void set_column_width(int idx, float size);
    void next_column();

    void push_font(ImFont* font);
    void pop_font();

    void push_style_var(ImGuiStyleVar style, vec2 val);
    void push_style_var(ImGuiStyleVar style, float val);
    void pop_style_var(int count = 1);

    void push_style_color(ImGuiCol style, color clr);
    void pop_style_color(int count = 1);

    template<size_t T>
    inline void vertical_tabs(int& tab, char* (&tabs)[T], bool key_bind = false) {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0)); {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));  {
                render_tabs2(tabs, tab, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y / T, key_bind);
            }
            ImGui::PopStyleVar();
        }
        ImGui::PopStyleVar();
    }

    template<size_t T>
    inline void horizontal(int& tab, char* (&tabs)[T], tabs_type type = tabs_type::standart) {
        auto tabs_w = (ImGui::GetWindowSize().x - ImGui::GetCursorPos().x - ImGui::GetStyle().WindowPadding.x - (type == tabs_type::spac || type == tabs_type::spac_child ? (6 *_countof(tabs) - 6) : 0)) / _countof(tabs);

        switch (type) {
        case tabs_type::standart:
            ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0)); {
                render_tabs(tabs, tab, tabs_w, 20.f);
            }
            ImGui::PopStyleVar();
            break;
        case tabs_type::big:
            ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0)); {
                render_tabs(tabs, tab, tabs_w, 25.f);
            }
            ImGui::PopStyleVar();
            break;
        case tabs_type::spac:
            ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(6, 6)); {
                render_tabsMain(tabs, tab, tabs_w, 20.f, false);
            }
            ImGui::PopStyleVar();
            break;
        case tabs_type::spac_child:
            ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(6, 6)); {
                render_tabsMain(tabs, tab, tabs_w, 20.f, true);
            }
            ImGui::PopStyleVar();
            break;
        }
    }
}
