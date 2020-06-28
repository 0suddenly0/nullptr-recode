#include "nullptr_gui.h"
#include "../settings/settings.h"

std::string last_child_name;

enum class last_used_item_t {
    check_box = 0,
    slider = 1,
    color_editor = 2,
    tooltip = 3,
    key_bind = 4,
    combo = 5,
    text = 6,
    button = 7,
    input = 8
};

last_used_item_t last_item;

namespace null_gui {
    namespace deep {
        int get_framerate() {
            return (int)ImGui::GetIO().Framerate;
        }

        void set_cursor_pos(vec2 pos) {
            ImGui::SetCursorPos(ImVec2(pos.x, pos.y));
        }

        void set_cursor_x(float x) {
            ImGui::SetCursorPosX(x);
        }

        void set_cursor_y(float y) {
            ImGui::SetCursorPosY(y);
        }

        vec2 get_scroll() {
            return vec2(ImGui::GetScrollX(), ImGui::GetScrollY());
        }

        void set_next_window_pos(vec2 pos, bool once) {
            ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y), once ? ImGuiCond_Once : 0);
        }

        void set_next_window_size(vec2 size, bool once) {
            ImGui::SetNextWindowSize(ImVec2(size.x, size.y), once ? ImGuiCond_Once : 0);
        }

        void set_menu_color(color clr) {
            ImGui::GetStyle().Colors[ImGuiCol_nullptr_color] = render::get_vec4(clr);
        }

        void set_window_pos(vec2 pos) {
            ImGui::SetWindowPos(ImVec2(pos.x, pos.y));
        }

        void set_window_pos(const char* name, vec2 pos, bool once) {
            ImGui::SetWindowPos(name, ImVec2(pos.x, pos.y), once ? ImGuiCond_Once : 0);
        }

        void set_window_size(vec2 size) {
            ImGui::SetWindowSize(ImVec2(size.x, size.y));
        }

        void set_window_hidden(bool hidden) {
            ImGui::SetWindowHidden(hidden);
        }

        void set_next_window_alpha(float alpha) {
            ImGui::SetNextWindowBgAlpha(alpha);
        }

        void set_next_window_focus() {
            ImGui::SetNextWindowFocus();
        }

        void get_window_pos_and_size(vec2* pos, vec2* size) {
            *pos = vec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
            *size = vec2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
        }

        vec2 get_window_pos() {
            return vec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
        }

        vec2 get_window_size() {
            return vec2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
        }
    }

    void text(const char* text, ...) {
        last_item = last_used_item_t::text;

        va_list args;
        va_start(args, text);
        ImGui::TextVSpace(text, args);
        va_end(args);
    }

    void text_no_space(const char* text, ...) {
        last_item = last_used_item_t::text;

        va_list args;
        va_start(args, text);
        ImGui::TextV(text, args);
        va_end(args);
    }

    bool check_box(const char* text, bool* value) {
        last_item = last_used_item_t::check_box;

        return ImGui::Checkbox(text, value);
    }

    bool button(const char* text, bool max_size, vec2 size, bool use_space) {
        last_item = last_used_item_t::button;

        return use_space ? ImGui::ButtonSpace(text, ImVec2(size.x, size.y), max_size) : ImGui::Button(text, ImVec2(size.x, size.y), use_space);
    }

    bool key_bind(const char* text, key_bind_t* bind, bool tooltip) {
        bool ret = ImGui::KeyBind(text, bind, last_item == last_used_item_t::tooltip ? ImGuiKeyBindFlags_OnTooltip : 0);
        last_item = last_used_item_t::key_bind;
        return ret;
    }

    bool slider_int(const char* text, int* value, int min, int max, const char* format) {
        last_item = last_used_item_t::slider;

        return ImGui::SliderInt(text, value, min, max, format);
    }

    bool slider_float(const char* text, float* value, float min, float max, const char* format, float power) {
        last_item = last_used_item_t::slider;

        return ImGui::SliderFloat(text, value, min, max, format, power);
    }

    bool int_input(const char* text, int* value, int step_low, int step_big) {
        last_item = last_used_item_t::input;

        return ImGui::InputInt(text, value, step_low, step_big);
    }

    bool float_input(const char* text, float* value, float step_low, float step_big, const char* format) {
        last_item = last_used_item_t::input;

        return ImGui::InputFloat(text, value, step_low, step_big, format);
    }

    bool color_edit(const char* text, color* color, bool alpha) {
        last_item = last_used_item_t::color_editor;

        return ImGui::ColorEdit4(text, color, alpha ? ImGuiColorEditFlags_AlphaBar : 0);
    }

    bool text_input(const char* text, std::string* value) {
        last_item = last_used_item_t::input;

        char buf[255];
        strcpy(buf, value->c_str());
        bool return_value = ImGui::InputText(text, buf, sizeof(buf));
        *value = std::string(buf);

        return return_value;
    }

    void image(IDirect3DTexture9* texture, vec2 size) {
        ImGui::Image(texture, ImVec2(size.x, size.y));
    }

    bool combo(const char* text, int* value, std::vector<std::string>& items) {
        last_item = last_used_item_t::combo;

        static auto vector_getter = [](void* vec, int idx, const char** out_text) {
            auto& vector = *(std::vector<std::string>*)(vec);
            if (idx < 0 || idx >= (int)(vector.size())) { return false; }
            *out_text = vector.at(idx).c_str();
            return true;
        };

        if (items.empty()) return false;
        return ImGui::Combo(text, value, vector_getter, (void*)(&items), items.size());
    }

    bool functional_combo(const char* text, const char* prev_item, std::function<void()> function) {
        last_item = last_used_item_t::combo;

        return ImGui::FunctionCombo(text, prev_item, function);
    }

    bool multi_combo(const char* text, std::vector<std::string> names, std::vector<bool>* values) {
        if (names.size() != values->size()) return false;
        last_item = last_used_item_t::combo;

        return ImGui::MultiCombo(text, names, values, (int)names.size());
    }

    bool multi_combo(const char* text, std::vector<std::string> names, std::vector<bool*> values) {
        if (names.size() != values.size()) return false;
        last_item = last_used_item_t::combo;

        return ImGui::MultiCombo(text, names, values, (int)names.size());
    }

    void tooltip_items(const char* text, std::function<void()> func) {
        ImGui::TooltipSettings(text, func);

        last_item = last_used_item_t::tooltip;
    }

    void tooltip_on_click(const char* text, std::function<void()> func) {
        if (ImGui::ItemsToolTipBegin(text, 1)) {
            func();
            ImGui::ItemsToolTipEnd();
        }
    }

    void tooltip(const char* text, ...) {
        if (ImGui::IsItemHovered(0)) {
            va_list args;
            va_start(args, text);
            ImGui::SetTooltipV(text, args);
            va_end(args);
        }
    }

    void line(float size) {
        ImGui::Separator(size);
    }

    bool begin_list_box(const char* text, vec2 size) {
        bool ret = ImGui::ListBoxHeader(text, ImVec2(size.x, size.y));
       /* if (ret) {
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 2));
        }*/
        return ret;
    }

    bool selectable(const char* text, bool selected) {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 2));
        bool ret =  ImGui::Selectable(text, selected);
        ImGui::PopStyleVar();
        ImGui::PopStyleVar();
        return ret;
    }

    void end_list_box() {
       /* ImGui::PopStyleVar();
        ImGui::PopStyleVar();*/
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
    
    bool begin_menu_bar() {
        return ImGui::BeginMenuBar();
    }

    void end_menu_bar() {
        ImGui::EndMenuBar();
    }

    void same_line(float x) {
        ImGui::SameLine(0, x);
    }

    void create_columns(int count) {
        ImGui::Columns(count, (const char*)0, false);
    }

    void set_column_width(int idx, float size) {
        ImGui::SetColumnWidth(idx, size);
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

    void push_style_var(ImGuiStyleVar style, vec2 val) {
        ImGui::PushStyleVar(style, ImVec2(val.x, val.y));
    }

    void push_style_var(ImGuiStyleVar style, float val) {
        ImGui::PushStyleVar(style, val);
    }

    void pop_style_var(int count) {
        ImGui::PopStyleVar(count);
    }

    void push_style_color(ImGuiCol style, color clr) {
        ImGui::PushStyleColor(style, render::get_vec4(clr));
    }

    void pop_style_color(int count) {
        ImGui::PopStyleColor(count);
    }
}
