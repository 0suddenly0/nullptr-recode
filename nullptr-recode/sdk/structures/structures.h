#pragma once
#include "../sdk.h"
#include <array>
#include "../../utils/utils.h"
#include "../interfaces/e_handle.h"
#include "../../helpers/netvars.h"

#define NETVAR(type, name, table, netvar_name)                      \
    type& name##() const {                                          \
        static int _##name = netvar::get_offset(table, netvar_name);\
        return *(type*)((uintptr_t)this + _##name);                 \
    }

#define PNETVAR(type, name, table, netvar_name)                     \
    type* name##() const {                                          \
        static int _##name = netvar::get_offset(table, netvar_name);\
        return (type*)((uintptr_t)this + _##name);                 \
    } \


#define NETVAR_EX( type, name, table, netvar_name, extra ) \
  type& name##() const {                                                      \
    static int _##name = netvar::get_offset( table, netvar_name ) + extra; \
    return *(type*)( (uintptr_t)this + _##name );                             \
  }

#define NETPROP(name, table, netvar_name) static recv_prop* name() \
{ \
	static auto prop_ptr = netvar::get_netvar_prop(table,netvar_name); \
	return prop_ptr; \
}

enum observer_mode_t {
	OBS_MODE_NONE = 0,
	OBS_MODE_DEATHCAM = 1,
	OBS_MODE_FREEZECAM = 2,
	OBS_MODE_FIXED = 3,
	OBS_MODE_IN_EYE = 4,
	OBS_MODE_CHASE = 5,
	OBS_MODE_ROAMING = 6
};

enum cs_weapon_type
{
	WEAPONTYPE_KNIFE = 0,
	WEAPONTYPE_PISTOL,
	WEAPONTYPE_SUBMACHINEGUN,
	WEAPONTYPE_RIFLE,
	WEAPONTYPE_SHOTGUN,
	WEAPONTYPE_SNIPER_RIFLE,
	WEAPONTYPE_MACHINEGUN,
	WEAPONTYPE_C4,
	WEAPONTYPE_PLACEHOLDER,
	WEAPONTYPE_GRENADE,
	WEAPONTYPE_UNKNOWN
};

struct datamap_t;
class c_animation_layer;
class c_base_combat_weapon;
class c_base_attributable_item;
class c_base_view_model;
class c_csgo_player_animstate;
class c_base_player_animstate;
class c_base_entity;

#include "c_animation_layer.h"
#include "c_base_attributable_item.h"
#include "c_base_entity.h"
#include "c_base_player.h"
#include "c_base_view_model.h"
#include "c_base_weapon_world_model.h"
#include "c_csgo_player_animstate.h"
#include "c_econ_item_view.h"
#include "c_game_rules.h"
#include "c_hud_texture.h"
#include "c_planted_c4.h"
#include "c_player_resource.h"

