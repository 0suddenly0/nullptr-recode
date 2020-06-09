#include "../hooks.h"
#include "../../settings/globals.h"

IDirect3DStateBlock9* pixel_state = NULL; IDirect3DVertexDeclaration9* vertDec; IDirect3DVertexShader9* vertShader;
DWORD dwOld_D3DRS_COLORWRITEENABLE;

bool init = false;

namespace hooks {
	long __stdcall end_scene::hook(IDirect3DDevice9* pDevice) {
		auto o_end_scene = d3d9device_vhook.get_original<fn>(indexes::end_scene_idx);

		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::GetIO().MouseDrawCursor = globals::show_menu;

		menu::draw_bind_window();
		menu::draw_main_window();
		render::begin_render();

		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

		return o_end_scene(pDevice);
	}
}