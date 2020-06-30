#include <mutex>
#include "render.h"
#include "../functions/visuals/background.h"
#include "../settings/settings.h"
#include "../helpers/math/math.h"

ImDrawList* draw_list_act = nullptr;
ImDrawList* draw_list_for_render = nullptr;
std::mutex render_mutex;

namespace render {
	ImFont* menu_bar_font = nullptr;
	ImFont* menu_big_font = nullptr;
	ImFont* default_font = nullptr;
	ImDrawList* draw_list = nullptr;

	void initialize() {
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		ImGui_ImplWin32_Init(sdk::game_hwnd);
		ImGui_ImplDX9_Init(sdk::d3d9device);

		io.ConfigWindowsMoveFromTitleBarOnly = true;
		ImGui::set_nullptr_styles();

		draw_list = new ImDrawList(ImGui::GetDrawListSharedData());
		draw_list_act = new ImDrawList(ImGui::GetDrawListSharedData());
		draw_list_for_render = new ImDrawList(ImGui::GetDrawListSharedData());

		initialize_fonts();
	}

	void begin_render() {
		clear_main_list();

		if (globals::show_menu) {
			draw_box_filled(0.f, 0.f, utils::get_screen_size().x, utils::get_screen_size().y, color(0, 0, 0, 100));
		}

		notify::render();
		visuals::render();
		backdrop::render();

		move_to_act();
	}

	void initialize_fonts() {
		ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(
			fonts::main_compressed_data,
			fonts::main_compressed_size,
			12.f,
			nullptr,
			ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

		// esp font
		default_font = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(
			fonts::main_compressed_data,
			fonts::main_compressed_size,
			12.f,
			nullptr,
			ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

		menu_bar_font = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(
			fonts::main_compressed_data,
			fonts::main_compressed_size,
			14.f,
			nullptr,
			ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

		menu_big_font = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(
			fonts::main_compressed_data,
			fonts::main_compressed_size,
			18.f,
			nullptr,
			ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
	}

	void destroy() {
		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	void clear_main_list() {
		draw_list->Clear();
		draw_list->PushClipRectFullScreen();
	}

	void move_to_act() {
		render_mutex.lock();
		*draw_list_act = *draw_list;
		render_mutex.unlock();
	}

	ImDrawList* get_list_for_render() {
		if (render_mutex.try_lock()) {
			*draw_list_for_render = *draw_list_act;
			render_mutex.unlock();
		}

		return draw_list_for_render;
	}

	void draw_text_multicolor(std::vector<multicolor_t> items, vec2 pos, bool outline, bool center, int size) {
		float cur_x = pos.x - (center ? get_multicolor_size(items, size).x / 2 : 0);

		for (auto& item : items) {
			vec2 text_size = vec2(default_font->CalcTextSizeA(size, FLT_MAX, 0.0f, item.text.c_str()).x, default_font->CalcTextSizeA(size, FLT_MAX, 0.0f, item.text.c_str()).y);
			draw_text(item.text, vec2(cur_x, pos.y), item.clr, outline, false, size);
			cur_x += text_size.x;

			//if (item.text.at(item.text.length() - 1) == ' ') cur_x += 1;
		}
	}

	vec2 get_multicolor_size(std::vector<multicolor_t> items, int size) {
		vec2 cur_size = vec2(0, 0);

		for (auto& item : items) {
			vec2 text_size = vec2(default_font->CalcTextSizeA(size, FLT_MAX, 0.0f, item.text.c_str()).x, default_font->CalcTextSizeA(size, FLT_MAX, 0.0f, item.text.c_str()).y);
			cur_size.x += text_size.x;
			cur_size.y = text_size.y;

			//if (item.text.at(item.text.length() - 1) == ' ') cur_size.x += 1;
		}

		return cur_size;
	}

	void draw_text(std::string text, vec2 pos, color clr, bool outline, bool center, int size) {
		ImVec2 textSize = default_font->CalcTextSizeA(size, FLT_MAX, 0.0f, text.c_str());
		if (!default_font->ContainerAtlas) return;
		draw_list->PushTextureID(default_font->ContainerAtlas->TexID);

		if (center)
			pos.x -= textSize.x / 2.0f;

		if (outline)
		{
			draw_list->AddText(default_font, size, ImVec2(pos.x + 1, pos.y), ImGui::GetColorU32(ImVec4(0, 0, 0, clr.color_float[3])), text.c_str());
			draw_list->AddText(default_font, size, ImVec2(pos.x + 1, pos.y + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, clr.color_float[3])), text.c_str());
			draw_list->AddText(default_font, size, ImVec2(pos.x + 1, pos.y - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, clr.color_float[3])), text.c_str());
			draw_list->AddText(default_font, size, ImVec2(pos.x, pos.y + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, clr.color_float[3])), text.c_str());
			draw_list->AddText(default_font, size, ImVec2(pos.x, pos.y - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, clr.color_float[3])), text.c_str());
			draw_list->AddText(default_font, size, ImVec2(pos.x - 1, pos.y), ImGui::GetColorU32(ImVec4(0, 0, 0, clr.color_float[3])), text.c_str());
			draw_list->AddText(default_font, size, ImVec2(pos.x - 1, pos.y + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, clr.color_float[3])), text.c_str());
			draw_list->AddText(default_font, size, ImVec2(pos.x - 1, pos.y - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, clr.color_float[3])), text.c_str());
		}
		draw_list->AddText(default_font, size, ImVec2(pos.x, pos.y), ImGui::GetColorU32(get_vec4(clr)), text.c_str());

		draw_list->PopTextureID();
	}

	void draw_line(vec2 start, vec2 end, color clr, int thickness) {
		draw_list->AddLine(ImVec2(start.x, start.y), ImVec2(end.x, end.y), ImGui::GetColorU32(get_vec4(clr)), (float)thickness);
	}

	void draw_box(vec2 start, vec2 end, color clr) {
		draw_list->AddRect(ImVec2(start.x, start.y), ImVec2(end.x, end.y), ImGui::GetColorU32(get_vec4(clr)));
	}

	void draw_box_filled(vec2 start,vec2 end, color clr, float round) {
		draw_list->AddRectFilled(ImVec2(start.x, start.y), ImVec2(end.x, end.y), ImGui::GetColorU32(get_vec4(clr)), round);
	}

	void draw_box_filled_rounded(vec2 start, vec2 end, color clr, float round, int rounding_side) {
		draw_list->AddRectFilled(ImVec2(start.x, start.y), ImVec2(end.x, end.y), ImGui::GetColorU32(get_vec4(clr)), round, rounding_side);
	}

	void draw_circle(vec2 pos, float radius, int points, color clr) {
		draw_list->AddCircle(ImVec2(pos.x, pos.y), radius, ImGui::GetColorU32(get_vec4(clr)), points);
	}

	void draw_circle_filled(vec2 pos, float radius, int points, color clr) {
		draw_list->AddCircleFilled(ImVec2(pos.x, pos.y), radius, ImGui::GetColorU32(get_vec4(clr)), points);
	}

	void draw_box_filled_multicolor(vec2 start, vec2 end, color clr, color clr_right) {
		draw_list->AddRectFilledMultiColor(ImVec2(start.x, start.y), ImVec2(end.x, end.y), ImGui::GetColorU32(get_vec4(clr)), ImGui::GetColorU32(get_vec4(clr_right)), ImGui::GetColorU32(get_vec4(clr_right)), ImGui::GetColorU32(get_vec4(clr)));
	}

	void draw_circle_3d(vec3 pos, float points, float radius, color clr) {
		float step = (float)M_PI * 2.0f / points;

		for (float a = 0; a < (M_PI * 2.0f); a += step) {
			vec3 start(radius * cosf(a) + pos.x, radius * sinf(a) + pos.y, pos.z);
			vec3 end(radius * cosf(a + step) + pos.x, radius * sinf(a + step) + pos.y, pos.z);

			vec2 start2d, end2d;
			if (sdk::debug_overlay->screen_position(start, start2d) || sdk::debug_overlay->screen_position(end, end2d))
				return;

			draw_line(start2d.x, start2d.y, end2d.x, end2d.y, clr);
		}
	}
}