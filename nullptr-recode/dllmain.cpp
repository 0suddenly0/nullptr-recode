#include <windows.h>
#include "utils/utils.h"
#include "sdk/sdk.h"
#include "settings/globals.h"
#include "hooks/hooks.h"
#define _CRT_SECURE_NO_WARNINGS

DWORD WINAPI attach(LPVOID base) {

	if (utils::wait_for_modules(10000, { L"client.dll", L"engine.dll", L"shaderapidx9.dll" }) == WAIT_TIMEOUT) {
		return FALSE;
	}

#ifdef _DEBUG
	utils::attach_console();
#endif

	utils::console_print("[-] injected\n");

	utils::console_print("[-] initialization sdk...\n");
	sdk::initialize();
	sdk::print();
	utils::console_print("[-] sdk initialized\n");

	utils::console_print("[-] initialization render...\n");
	render::initialize();
	utils::console_print("[-] render initialized\n");

	utils::console_print("[-] waiting hooks...\n");
	hooks::initialize();
	utils::console_print("[-] all func hooked\n");

#ifdef _DEBUG
	input::register_hotkey(VK_END, [base]() {
		globals::unloading = true;
		});
#endif

	input::register_hotkey(VK_INSERT, [base]() {
		globals::show_menu = !globals::show_menu;
		});


	while (!globals::unloading) Sleep(1000);

	FreeLibraryAndExitThread(static_cast<HMODULE>(base), 1);
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
		if (lpReserved == nullptr)
			return detach();
		break;
	}
	return TRUE;
}

