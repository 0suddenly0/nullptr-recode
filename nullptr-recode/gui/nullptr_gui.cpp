#include "nullptr_gui.h"
#include "../settings/globals.h"

std::string last_child_name;

namespace null_gui {
    int get_framerate() {
       return (int)ImGui::GetIO().Framerate;
    }

    void set_cursor_pos(vec2 pos) {
        ImGui::SetCursorPos(ImVec2(pos.x, pos.y));
    }

    void set_menu_color(color clr) {
        ImGui::GetStyle().Colors[ImGuiCol_nullptr_color] = render::get_vec4(clr);
    }

    void text(const char* text, ...) {
        va_list args;
        va_start(args, text);
        ImGui::TextVSpace(text, args);
        va_end(args);
    }

    void text_no_space(const char* text, ...) {
        va_list args;
        va_start(args, text);
        ImGui::TextV(text, args);
        va_end(args);
    }

    bool check_box(const char* text, bool* value) {
        return ImGui::Checkbox(text, value);
    }

    bool button(const char* text, bool max_size, vec2 size) {
        return ImGui::ButtonSpace(text, ImVec2(size.x, size.y), max_size);
    }

    bool key_bind(const char* text, key_bind_t* bind, bool tooltip) {
        return ImGui::KeyBind(text, bind, tooltip ? ImGuiKeyBindFlags_OnTooltip : 0);
    }

    bool slider_int(const char* text, int* value, int min, int max, const char* format) {
        return ImGui::SliderInt(text, value, min, max, format);
    }

    bool slider_float(const char* text, float* value, float min, float max, const char* format) {
        return ImGui::SliderFloat(text, value, min, max, format);
    }

    bool int_input(const char* text, int* value, int step_low, int step_big) {
        return ImGui::InputInt(text, value, step_low, step_big);
    }

    bool float_input(const char* text, float* value, float step_low, float step_big, const char* format) {
        return ImGui::InputFloat(text, value, step_low, step_big, format);
    }

    bool color_edit(const char* text, color* color, bool alpha) {
        return ImGui::ColorEdit4(text, color, alpha ? ImGuiColorEditFlags_AlphaBar : 0);
    }

    bool text_input(const char* text, std::string* value) {
        char buf[255];
        strcpy(buf, value->c_str());
        bool return_value = ImGui::InputText(text, buf, sizeof(buf));
        *value = std::string(buf);

        return return_value;
    }

    bool combo(const char* text, int* value, std::vector<std::string>& items) {
        static auto vector_getter = [](void* vec, int idx, const char** out_text) {
            auto& vector = *static_cast<std::vector<std::string>*>(vec);
            if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
            *out_text = vector.at(idx).c_str();
            return true;
        };

        if (items.empty()) { return false; }
        return ImGui::Combo(text, value, vector_getter, static_cast<void*>(&items), items.size());
    }

    bool multi_combo(const char* text, std::vector<std::string> names, std::vector<bool>* values) {
        if (names.size() != values->size()) return false;
        return ImGui::MultiCombo(text, names, values, (int)names.size());
    }

    void tooltip_items(const char* text, std::function<void()> func) {
        ImGui::TooltipSettings(text, func);
    }

    void tooltip(const char* text, ...) {
        if (ImGui::IsItemHovered(0)) {
            va_list args;
            va_start(args, text);
            ImGui::SetTooltipV(text, args);
            va_end(args);
        }
    }

    void line() {
        ImGui::Separator();
    }

    bool begin_list_box(const char* text, vec2 size) {
        bool ret = ImGui::ListBoxHeader(text, ImVec2(size.x, size.y));
        if (ret) {
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 2));
        }
        return ret;
    }

    bool selectable(const char* text, bool selected) {
        return ImGui::Selectable(text, selected);
    }

    void end_list_box() {
        ImGui::PopStyleVar();
        ImGui::PopStyleVar();
        ImGui::ListBoxFooter();
    }


    bool begin_window(const char* text, bool* open, vec2 size, ImGuiWindowFlags flags) {
        bool no_title = text[0] == '#' && text[1] == '#';
        bool size_setted = size.x != 0 && size.y != 0;
        if (size_setted) ImGui::SetNextWindowSize(ImVec2(size.x, size.y), ImGuiCond_Once);
        return ImGui::Begin(text, open, (no_title ? ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_TitleBarLine : ImGuiWindowFlags_TitleBarLine) | (size_setted ? ImGuiWindowFlags_NoResize : 0 ) | ImGuiWindowFlags_NoCollapse | flags);
    }

    void end_window() {
        ImGui::End();
    }

    bool begin_group(const char* text, vec2 size) {
        bool no_title = text[0] == '#' && text[1] == '#';
        last_child_name = text;
        return ImGui::BeginChild(text, ImVec2(size.x, size.y), !no_title, (no_title ? ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse : ImGuiWindowFlags_ChildTitleBarLine));
    }

    void end_group() {
        ImGui::EndChild(last_child_name.c_str());
        last_child_name = "";
    }
    

    void same_line(float x) {
        ImGui::SameLine(0, x);
    }

    void create_columns(int count) {
        ImGui::Columns(count, (const char*)0, false);
    }

    void next_column() {
        ImGui::NextColumn();
    }

    void push_font(ImFont* font) {
        ImGui::PushFont(font);
    }

    void pop_font() {
        ImGui::PopFont();
    }
}
