#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include <string>
#include <vector>
#include "../helpers/helpers.h"

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
void render_tabsMain(char* (&names)[N], int& activetab, float w, float h) {
    bool values[N] = { false };
    values[activetab] = true;
    for (auto i = 0; i < N; ++i) {
        if (i == 0) {
            if (ImGui::ToggleButtonMain(names[i], &values[i], ImVec2{ w, h }, 1))
                activetab = i;
        } else if (i == N - 1) {
            if (ImGui::ToggleButtonMain(names[i], &values[i], ImVec2{ w, h }, 2))
                activetab = i;
        } else {
            if (ImGui::ToggleButtonMain(names[i], &values[i], ImVec2{ w, h }, 0))
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
    int get_framerate();
    void set_cursor_pos(vec2 pos);
    void set_menu_color(color clr);

    void text(const char* text, ...);
    void text_no_space(const char* text, ...);
    bool check_box(const char* text, bool* value);
    // if max_size = true, button.size.x = window.size.x
    bool button(const char* text, bool max_size = true, vec2 size = vec2(0, 0));
    // if u need to use tooltip_items with key_bind, set tooltip to true
    bool key_bind(const char* text, key_bind_t* bind, bool tooltip = false);
    bool slider_int(const char* text, int* value, int min, int max, const char* format = "%d");
    bool slider_float(const char* text, float* value, float min, float max, const char* format = "%.1f");
    bool int_input(const char* text, int* value, int step_low = 1, int step_big = 100);
    bool float_input(const char* text, float* value, float step_low = 0.f, float step_big = 0.f, const char* format = "%.1f");
    bool color_edit(const char* text, color* color, bool alpha = true);
    bool text_input(const char* text, std::string* value);
    bool combo(const char* text, int* value, std::vector<std::string>& items);
    bool multi_combo(const char* text, std::vector<std::string> names, std::vector<bool>* values);
    void tooltip_items(const char* text, std::function<void()> func);
    void tooltip(const char* text, ...);
    void line();
    bool begin_list_box(const char* text, vec2 size = vec2(0, 0));
    bool selectable(const char* text, bool selected);
    void end_list_box();

    bool begin_window(const char* text, bool* open = NULL, vec2 size = vec2(0, 0), ImGuiWindowFlags flags = 0);
    bool begin_group(const char* text, vec2 size = vec2(0, 0));
    void end_window();
    void end_group();

    void same_line(float x = 0.f);
    void create_columns(int count);
    void next_column();

    void push_font(ImFont* font);
    void pop_font();

    template<size_t T>
    inline void vertical_tabs(int& tab, char* (&tabs)[T], bool key_bind = false) {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing_new, ImVec2(0, 0)); {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));  {
                render_tabs2(tabs, tab, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y / T, key_bind);
            }
            ImGui::PopStyleVar();
        }
        ImGui::PopStyleVar();
    }

    template<size_t T>
    inline void horizontal(int& tab, char* (&tabs)[T], bool main = false) {
        auto tabs_w = (ImGui::GetWindowSize().x - ImGui::GetCursorPos().x - ImGui::GetStyle().WindowPadding.x) / _countof(tabs);

        ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing_new, ImVec2(0, 0)); {
            if (main) render_tabsMain(tabs, tab, tabs_w, 30.f);
            else render_tabs(tabs, tab, tabs_w, 25.f);
        }
        ImGui::PopStyleVar();
    }
}
