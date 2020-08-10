#pragma once
#include <Windows.h>

#include "misc/vfunc.h"
#include "misc/Enums.h"

#include "../helpers/math/matrix.h"
#include "../helpers/math/qangle.h"
#include "../helpers/math/vector.h"

#include "interfaces/material_var.h"
#include "interfaces/studio.h"
#include "interfaces/steam.h"
#include "interfaces/net_message.h"
#include "interfaces/base_file_system.h"
#include "interfaces/app_system.h"
#include "interfaces/view_render_beams.h"
#include "interfaces/base_client_dll.h"
#include "interfaces/client_entity.h"
#include "interfaces/client_entity_list.h"
#include "interfaces/client_mode.h"
#include "interfaces/convar.h"
#include "interfaces/cvar.h"
#include "interfaces/engine_trace.h"
#include "interfaces/engine_client.h"
#include "interfaces/debug_overlay.h"
#include "interfaces/surface.h"
#include "interfaces/input.h"
#include "interfaces/model_info_client.h"
#include "interfaces/model_render.h"
#include "interfaces/render_view.h"
#include "interfaces/game_event_manager.h"
#include "interfaces/material_system.h"
#include "interfaces/move_helper.h"
#include "interfaces/mdl_cache.h"
#include "interfaces/prediction.h"
#include "interfaces/panel.h"
#include "interfaces/engine_sound.h"
#include "interfaces/view_render.h"
#include "interfaces/client_state.h"
#include "interfaces/physics.h"
#include "interfaces/input_system.h"
#include "interfaces/ref_counted.h"
#include "interfaces/memalloc.h"
#include "interfaces/localize.h"
#include "interfaces/studio_render.h"
#include "interfaces/convar.h"
#include "interfaces/user_cmd.h"
#include "interfaces/convar_sys.h"
#include "interfaces/glow_outline_effect.h"
#include "interfaces/network_string_table.h"

#include "misc/datamap.h"

#include "../helpers/netvars.h"

class c_game_rules;
class c_player_resource;

class c_local_player {
	friend bool operator==(const c_local_player& lhs, void* rhs);
public:
	c_local_player() : m_local(nullptr) { }

	operator bool() const { return *m_local != nullptr; }
	operator c_base_player* () const { return *m_local; }

	c_base_player* operator->() { return *m_local; }

private:
	c_base_player** m_local;
};

namespace sdk {
	extern HWND                              game_hwnd;
	extern c_game_rules*                     game_rules;
	extern c_player_resource**               player_resource;
	extern c_weapon_system*                  weapon_system;
	extern c_engine_client*                  engine_client;
	extern c_base_client_dll*                chl_client;
	extern c_client_entity_list*             entity_list;
	extern c_global_vars_base*               global_vars;
	extern c_engine_trace*                   engine_trace;
	extern c_cvar*                           cvar;
	extern c_panel*                          vgui_panel;
	extern c_client_mode*                    client_mode;
	extern c_debug_overlay*                  debug_overlay;
	extern c_surface*                        vgui_surface;
	extern c_input*                          input;
	extern c_model_info_client*              mdl_info;
	extern c_model_render*                   mdl_render;
	extern c_render_view*                    render_view;
	extern c_material_system*                mat_system;
	extern c_game_event_manager*             game_events;
	extern c_move_helper*                    move_helper;
	extern c_mdl_cache*                      mdl_cache;
	extern c_prediction*                     prediction;
	extern c_game_movement*                  game_movement;
	extern c_engine_sound*                   engine_sound;
	extern c_view_render_beams*              beam;
	extern c_glow_object_manager*            glow_obj_manager;
	extern c_view_render*                    view_render;
	extern IDirect3DDevice9*                 d3d9device;
	extern c_client_state*                   client_state;
	extern c_physics_surface_props*          phys_surface;
	extern c_localize*                       localize;
	extern c_input_system*                   input_system;
	extern c_memalloc*                       mem_alloc;
	extern c_base_file_system*               base_file_system;
	extern uintptr_t*                        spatial_partition;
	extern c_studio_render*                  studio_render;
	extern c_steam_game_coordinator*         game_coordinator;
	extern c_steam_user*                     steam_user;
	extern c_steam_http*                     steam_http;
	extern c_steam_client*                   steam_client;
	extern c_steam_friends*                  steam_friends;
	extern c_local_player                    local_player;
	extern c_network_string_table_container* network_string_table;

	void initialize();
	void print();
}

#include "interfaces/e_handle.h"