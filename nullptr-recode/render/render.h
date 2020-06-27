#pragma once
#include "../sdk/sdk.h"
#include "../helpers/helpers.h"
#include "../gui/nullptr_gui.h"
#include "fonts/fonts.h"
#include "../functions/notify/notify.h"
#include "../functions/visuals/visuals.h"
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"d3d9.lib")


namespace render {
	extern ImFont* menu_bar_font;
	extern ImFont* menu_big_font;
	extern ImFont* default_font;
	extern ImDrawList* draw_list;

	void initialize();
	void begin_render();
	void initialize_fonts();
	void destroy();

	void clear_main_list();
	void move_to_act();
	ImDrawList* get_list_for_render();

	ImVec4 get_vec4(color clr) {
		return ImVec4(clr.color_float[0], clr.color_float[1], clr.color_float[2], clr.color_float[3]);
	}

	void draw_polugin(std::vector<ImVec2> points, color clr) {
		draw_list->AddConvexPolyFilled(points.data(), points.size(), ImGui::GetColorU32(get_vec4(clr)));
	}

	template <typename T>
	void draw_text(std::string text, T x, T y, color clr, bool outline = true, bool center = true, int size = 12) {
		draw_text(text, vec2(x, y), clr, outline, center, size);
	}
	void draw_text(std::string text, vec2 pos, color clr, bool outline = true, bool center = true, int size = 12);

	template <typename T>
	void draw_line(T x_start, T y_start, T x_end, T y_end, color clr, int thickness = 1) {
		draw_line(vec2(x_start, y_start), vec2(x_end, y_end), clr, thickness);
	}
	void draw_line(vec2 start, vec2 end, color clr, int thickness = 1);

	template <typename T>
	void draw_box(float x_start, float y_start, float x_end, float y_end, color clr) {
		draw_box(vec2(x_start, y_start), vec2(x_end, y_end), clr);
	}
	void draw_box(vec2 start, vec2 end, color clr);

	template <typename T>
	void draw_box_filled(T x_start, T y_start, T x_end, T y_end, color clr, float round = 0.f) {
		draw_box_filled(vec2(x_start, y_start), vec2(x_end, y_end), clr, round);
	}
	void draw_box_filled(vec2 start, vec2 end, color clr, float round = 0.f);

	// top = 3, bot = 12, left = 5, right = 10
	template <typename T>
	void draw_box_filled_rounded(T x_start, T y_start, T x_end, T y_end, color clr, float round = 0.f, int rounding_side = 0) {
		draw_box_filled_rounded(vec2(x_start, y_start), vec2(x_end, y_end), clr, round, rounding_side);
	}
	void draw_box_filled_rounded(vec2 start, vec2 end, color clr, float round = 0.f, int rounding_side = 0);

	template <typename T>
	void draw_box_filled_multicolor(T x_start, T y_start, T x_end, T y_end, color clr, color clr_right) {
		draw_box_filled_multicolor(vec2(x_start, y_start), vec2(x_end, y_end), clr, clr_right);
	}
	void draw_box_filled_multicolor(vec2 start, vec2 end, color clr, color clr_right);

	template<typename T>
	void draw_circle(T x, T y, T radius, int points, color clr) {
		draw_circle(vec2(x, y), radius, points, clr);
	}
	void draw_circle(vec2 pos, float radius, int points, color clr);

	template<typename T>
	void draw_circle_filled(T x, T y, float radius, int points, color clr) {
		draw_circle_filled(vec2(x, y), radius, points, clr);
	}
	void draw_circle_filled(vec2 pos, float radius, int points, color clr);

	template<typename T>
	void draw_circle_3d(T x, T y, T z, float radius, int points, color clr) {
		draw_circle_3d(vec3(x, y, z), radius, points, clr);
	}
	void draw_circle_3d(vec3 pos, float points, float radius, color clr);
}