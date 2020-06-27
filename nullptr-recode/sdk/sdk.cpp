#include "sdk.h"
#include "structures/structures.h"

create_interface_fn get_module_factory(HMODULE module) {
	return (create_interface_fn)(GetProcAddress(module, "CreateInterface"));
}

template<typename T>
T* get_interface(create_interface_fn f, const char* szInterfaceVersion) {
	auto result = (T*)(f(szInterfaceVersion, nullptr));

	if (!result)
		throw std::runtime_error(std::string("[get_interface] failed to get_offset interface: ") + szInterfaceVersion);

	return result;
}

template <typename T>
T get_steam_interface(const char* version) {
	static const auto steam_api = GetModuleHandleA("steam_api.dll");
	return reinterpret_cast<T(__cdecl*)(void)>(GetProcAddress(steam_api, version))();
}

namespace sdk {
	HWND                      game_hwnd         = nullptr;
	c_game_rules*             game_rules        = nullptr;
	c_player_resource**       player_resource   = nullptr;
	c_weapon_system*          weapon_system     = nullptr;
	c_engine_client*          engine_client     = nullptr;
	c_base_client_dll*        chl_client        = nullptr;
	c_client_entity_list*     entity_list       = nullptr;
	c_global_vars_base*       global_vars       = nullptr;
	c_engine_trace*           engine_trace      = nullptr;
	c_cvar*                   cvar              = nullptr;
	c_panel*                  vgui_panel        = nullptr;
	c_client_mode*            client_mode       = nullptr;
	c_debug_overlay*          debug_overlay     = nullptr;
	c_surface*                vgui_surface      = nullptr;
	c_input*                  input             = nullptr;
	c_model_info_client*      mdl_info          = nullptr;
	c_model_render*           mdl_render        = nullptr;
	c_render_view*            render_view       = nullptr;
	c_material_system*        mat_system        = nullptr;
	c_game_event_manager*     game_events       = nullptr;
	c_move_helper*            move_helper       = nullptr;
	c_mdl_cache*              mdl_cache         = nullptr;
	c_prediction*             prediction        = nullptr;
	c_game_movement*          game_movement     = nullptr;
	c_engine_sound*           engine_sound      = nullptr;
	c_view_render_beams*      beam              = nullptr;
	c_glow_object_manager*    glow_obj_manager  = nullptr;
	c_view_render*            view_render       = nullptr;
	IDirect3DDevice9*         d3d9device        = nullptr;
	c_client_state*           client_state      = nullptr;
	c_physics_surface_props*  phys_surface      = nullptr;
	c_localize*               localize          = nullptr;
	c_input_system*           input_system      = nullptr;
	c_memalloc*               mem_alloc         = nullptr;
	c_base_file_system*       base_file_system  = nullptr;
	uintptr_t*                spatial_partition = nullptr;
	c_studio_render*          studio_render     = nullptr;
	c_steam_game_coordinator* game_coordinator  = nullptr;
	c_local_player            local_player;
	c_steam_user*             steam_user        = nullptr;
	c_steam_http*             steam_http        = nullptr;
	c_steam_client*           steam_client      = nullptr;
	c_steam_friends*          steam_friends     = nullptr;

	void initialize() {
		HMODULE h_dx9api            = GetModuleHandleW(L"shaderapidx9.dll");
		HMODULE h_engine            = GetModuleHandleW(L"engine.dll");
		HMODULE h_client            = GetModuleHandleW(L"client.dll");
		HMODULE h_stdlib            = GetModuleHandleA("vstdlib.dll");
		HMODULE h_vguimatsurface    = GetModuleHandleW(L"vguimatsurface.dll");
		HMODULE h_vgui2             = GetModuleHandleW(L"vgui2.dll");
		HMODULE h_materialsystem    = GetModuleHandleW(L"materialsystem.dll");
		HMODULE h_datacache         = GetModuleHandleW(L"datacache.dll");
		HMODULE h_vphysics          = GetModuleHandleW(L"vphysics.dll");
		HMODULE h_inputsystem       = GetModuleHandleW(L"inputsystem.dll");
		HMODULE h_localize          = GetModuleHandleW(L"localize.dll");
		HMODULE h_studiorender      = GetModuleHandleW(L"studiorender.dll");
		HMODULE h_file_system_stdio = GetModuleHandleW(L"filesystem_stdio.dll");
		HMODULE h_tier0             = GetModuleHandleA("tier0.dll");

		create_interface_fn engine_factory    = get_module_factory(h_engine);
		create_interface_fn client_factory    = get_module_factory(h_client);
		create_interface_fn vgui_factory      = get_module_factory(h_vguimatsurface);
		create_interface_fn vgui2_factory     = get_module_factory(h_vgui2);
		create_interface_fn matsys_factory    = get_module_factory(h_materialsystem);
		create_interface_fn datacache_factory = get_module_factory(h_datacache);
		create_interface_fn vphysics_factory  = get_module_factory(h_vphysics);
		create_interface_fn inputsys_factory  = get_module_factory(h_inputsystem);
		create_interface_fn localize_factory  = get_module_factory(h_localize);
		create_interface_fn studio_factory    = get_module_factory(h_studiorender);
		create_interface_fn filesys_factory   = get_module_factory(h_file_system_stdio);
		create_interface_fn valvestd_factory  = get_module_factory(h_stdlib);
		HSteamUser          _steam_user       = get_steam_interface<HSteamUser>("SteamAPI_GetHSteamUser");
		HSteamPipe          _steam_pipe       = get_steam_interface<HSteamPipe>("SteamAPI_GetHSteamPipe");

		steam_client      = get_steam_interface<c_steam_client*>  ("SteamClient");
		base_file_system  = get_interface<c_base_file_system>     (filesys_factory, "VBaseFileSystem011");
		chl_client        = get_interface<c_base_client_dll>      (client_factory, "VClient018");
		entity_list       = get_interface<c_client_entity_list>   (client_factory, "VClientEntityList003");
		prediction        = get_interface<c_prediction>           (client_factory, "VClientPrediction001");
		game_movement     = get_interface<c_gamemovement>         (client_factory, "GameMovement001");
		mdl_cache         = get_interface<c_mdl_cache>            (datacache_factory, "MDLCache004");
		engine_client     = get_interface<c_engine_client>        (engine_factory, "VEngineClient014");
		mdl_info          = get_interface<c_model_info_client>    (engine_factory, "VModelInfoClient004");
		mdl_render        = get_interface<c_model_render>         (engine_factory, "VEngineModel016");
		render_view       = get_interface<c_render_view>          (engine_factory, "VEngineRenderView014");
		engine_trace      = get_interface<c_engine_trace>         (engine_factory, "EngineTraceClient004");
		debug_overlay     = get_interface<c_debug_overlay>        (engine_factory, "VDebugOverlay004");
		game_events       = get_interface<c_game_event_manager>   (engine_factory, "GAMEEVENTSMANAGER002");
		engine_sound      = get_interface<c_engine_sound>         (engine_factory, "IEngineSoundClient003");
		mat_system        = get_interface<c_material_system>      (matsys_factory, "VMaterialSystem080");
		cvar              = get_interface<c_cvar>                 (valvestd_factory, "VEngineCvar007");
		vgui_panel        = get_interface<c_panel>                (vgui2_factory, "VGUI_Panel009");
		vgui_surface      = get_interface<c_surface>              (vgui_factory, "VGUI_Surface031");
		phys_surface      = get_interface<c_physics_surface_props>(vphysics_factory, "VPhysicsSurfaceProps001");
		input_system      = get_interface<c_input_system>         (inputsys_factory, "InputSystemVersion001");
		spatial_partition = get_interface<uintptr_t>              (engine_factory, "SpatialPartition001");
		localize          = get_interface<c_localize>             (localize_factory, "Localize_001");
		studio_render     = get_interface<c_studio_render>        (studio_factory, "VStudioRender026");
		steam_user        = steam_client->get_user                (_steam_user, _steam_user, "SteamUser019");
		game_coordinator  = steam_client->get_coordinator         (_steam_user, _steam_pipe, "SteamGameCoordinator001");
		steam_friends     = steam_client->get_friends             (_steam_user, _steam_pipe, "SteamFriends015");
		steam_http        = steam_client->get_http                (_steam_user, _steam_pipe, "STEAMHTTP_INTERFACE_VERSION002");

		mem_alloc              = *(c_memalloc**)GetProcAddress    (h_tier0,                      "g_pMemAlloc");
		d3d9device             = **(IDirect3DDevice9***)          (utils::pattern_scan(h_dx9api, "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1);
		beam                   = *(c_view_render_beams**)         (utils::pattern_scan(h_client, "B9 ? ? ? ? A1 ? ? ? ? FF 10 A1 ? ? ? ? B9") + 1);
		global_vars            = **(c_global_vars_base***)        (utils::pattern_scan(h_client, "A1 ? ? ? ? 5E 8B 40 10") + 1);
		input                  = *(c_input**)                     (utils::pattern_scan(h_client, "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10") + 1);
		move_helper            = **(c_move_helper***)             (utils::pattern_scan(h_client, "8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01") + 2);
		glow_obj_manager       = *(c_glow_object_manager**)       (utils::pattern_scan(h_client, "0F 11 05 ? ? ? ? 83 C8 01") + 3);
		view_render            = *(c_view_render**)               (utils::pattern_scan(h_client, "A1 ? ? ? ? B9 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? FF 10") + 1);
		client_state           = **(c_client_state***)            (utils::pattern_scan(h_engine, "A1 ? ? ? ? 8B 80 ? ? ? ? C3") + 1);
		player_resource        = *(c_player_resource***)          (utils::pattern_scan(h_client, "A1 ? ? ? ? 57 85 C0 74 08") + 1);
		game_rules             = *(c_game_rules**)                (utils::pattern_scan(h_client, "8B 0D ?? ?? ?? ?? 85 C0 74 0A 8B 01 FF 50 78 83 C0 54") + 2);
		local_player           = *(c_local_player*)               (utils::pattern_scan(h_client, "8B 0D ? ? ? ? 83 FF FF 74 07") + 2);
		weapon_system          = *(c_weapon_system**)             (utils::pattern_scan(h_client, "8B 35 ? ? ? ? FF 10 0F B7 C0") + 2);
		client_mode            = **(c_client_mode***)             ((*(DWORD**)chl_client)[10] + 0x5);

		D3DDEVICE_CREATION_PARAMETERS params;
		d3d9device->GetCreationParameters(&params);
		game_hwnd = params.hFocusWindow;
	}

	void print() {
#define STRINGIFY_IMPL(s) #s
#define STRINGIFY(s)      STRINGIFY_IMPL(s)
#define PRINT_INTERFACE(name) utils::console_print("[interface] %-20s: %p\n", STRINGIFY(name), name)

		PRINT_INTERFACE(weapon_system);
		PRINT_INTERFACE(engine_client);
		PRINT_INTERFACE(chl_client);
		PRINT_INTERFACE(entity_list);
		PRINT_INTERFACE(global_vars);
		PRINT_INTERFACE(engine_trace);
		PRINT_INTERFACE(cvar);
		PRINT_INTERFACE(vgui_panel);
		PRINT_INTERFACE(client_mode);
		PRINT_INTERFACE(debug_overlay);
		PRINT_INTERFACE(vgui_surface);
		PRINT_INTERFACE(input);
		PRINT_INTERFACE(mdl_info);
		PRINT_INTERFACE(mdl_render);
		PRINT_INTERFACE(render_view);
		PRINT_INTERFACE(mat_system);
		PRINT_INTERFACE(game_events);
		PRINT_INTERFACE(move_helper);
		PRINT_INTERFACE(mdl_cache);
		PRINT_INTERFACE(prediction);
		PRINT_INTERFACE(game_movement);
		PRINT_INTERFACE(engine_sound);
		PRINT_INTERFACE(beam);
		PRINT_INTERFACE(glow_obj_manager);
		PRINT_INTERFACE(view_render);
		PRINT_INTERFACE(d3d9device);
		PRINT_INTERFACE(client_state);
		PRINT_INTERFACE(phys_surface);
		PRINT_INTERFACE(localize);
		PRINT_INTERFACE(input_system);
		PRINT_INTERFACE(mem_alloc);
		PRINT_INTERFACE(base_file_system);
		PRINT_INTERFACE(spatial_partition);
		PRINT_INTERFACE(studio_render);
		PRINT_INTERFACE(game_coordinator);
		PRINT_INTERFACE(steam_user);
		PRINT_INTERFACE(steam_http);
		PRINT_INTERFACE(steam_client);
		PRINT_INTERFACE(steam_friends);
		PRINT_INTERFACE(player_resource);
		PRINT_INTERFACE(game_rules);
		PRINT_INTERFACE(local_player);
	}
}

#include "interfaces/e_handle.h"