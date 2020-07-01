#define NOMINMAX
#define _CRT_SECURE_NO_WARNINGS
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include "helpers/config_sys/config_sys.h"
#include <windows.h>
#include "utils/utils.h"
#include "sdk/sdk.h"
#include "settings/globals.h"
#include "hooks/hooks.h"
#include "helpers/netvars.h"

DWORD WINAPI attach(LPVOID base) {

	if (utils::wait_for_modules(10000, { L"client.dll", L"engine.dll", L"shaderapidx9.dll" }) == WAIT_TIMEOUT) {
		return FALSE;
	}

#ifdef _DEBUG
	utils::attach_console();
#endif

	try {
		
		config::skins::setup_vars();
		config::standart::setup_vars();

		utils::console_print("[-] injected\n");

		utils::console_print("[-] initialization sdk...\n");
		sdk::initialize();
		sdk::print();
		utils::console_print("[-] sdk initialized\n");

		utils::console_print("[-] initialization netvars...\n");
		netvar::initialize();
		utils::console_print("[-] netvars initialized\n");

		utils::console_print("[-] initialization render...\n");
		render::initialize();
		utils::console_print("[-] render initialized\n");

		utils::console_print("[-] waiting hooks...\n");
		hooks::initialize();
		utils::console_print("[-] all func hooked\n");

		std::vector<render::multicolor_t> items = {
			render::multicolor_t{ "nullptr", globals::menu_color },
			render::multicolor_t{ " injected", color(255,255,255,255) }
		};

		notify::add("nullptr", globals::menu_color, items, log_type::big);
		notify::add("nullptr", globals::menu_color, items, log_type::console);

#ifdef _DEBUG
		input::register_hotkey(VK_END, [base]() {
			globals::unloading = true;
			});
#endif

		input::register_hotkey(VK_INSERT, [base]() {
			globals::show_menu = !globals::show_menu;
			});

		if (skin_parser::skins.size() == 0)
		{
			skin_parser::initialize_kits();
		}

		while (!globals::unloading) Sleep(1000);
		
		FreeLibraryAndExitThread((HMODULE)(base), 1);
	} catch (const std::exception & ex) {
		utils::console_print("[-] an error occured during initialization:\n");
		utils::console_print("[-] %s\n", ex.what());
		utils::console_print("[-] Press any key to exit.\n");
		utils::console_read_key();
		utils::detach_console();

		FreeLibraryAndExitThread((HMODULE)(base), 1);
	}
}

BOOL WINAPI detach() {
#ifdef _DEBUG
	utils::detach_console();
#endif

	hooks::unhook();
	render::destroy();

	return TRUE;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
) {

	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)attach, hModule, 0, 0);
		return TRUE;
	case DLL_PROCESS_DETACH:
		if (lpReserved == nullptr) return detach();
		break;
	}
	return TRUE;
}

