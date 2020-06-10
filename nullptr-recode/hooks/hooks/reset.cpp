#include "../hooks.h"

namespace hooks {
	long __stdcall reset::hook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters) {
		auto o_reset = d3d9device_vhook.get_original<fn>(indexes::reset);

		ImGui_ImplDX9_InvalidateDeviceObjects();

		auto hr = o_reset(device, pPresentationParameters);

		if (hr >= 0) ImGui_ImplDX9_CreateDeviceObjects();

		return hr;
	}
}