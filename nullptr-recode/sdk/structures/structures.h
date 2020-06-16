#pragma once
#include "../sdk.h"
#include <array>
#include "../../utils/utils.h"
#include "../../helpers/math/math.h"

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
	obs_none = 0,
	obs_deathcam = 1,
	obs_freezecam = 2,
	obs_fixed = 3,
	obs_in_eye = 4,
	obs_chase = 5,
	obs_oaming = 6
};

enum cs_weapon_type
{
	type_knife = 0,
	type_pistol,
	type_submachinegun,
	type_rifle,
	type_shotgun,
	type_sniper_rifle,
	type_machinegun,
	type_c4,
	type_placeholder,
	type_grenade,
	type_unknown
};

struct datamap_t;
class c_game_rules;
class c_player_resource;
class c_econ_item_view;
class c_animation_layer;
class c_base_combat_weapon;
class c_base_attributable_item;
class c_base_view_model;
class c_csgo_player_animstate;
class c_base_player_animstate;
class c_base_entity;
class c_base_player;
class c_base_weapon_world_model;
class c_hud_texture;

#include "c_animation_layer.h"
#include "c_base_combat_weapon.h"
#include "c_base_entity.h"
#include "c_base_attributable_item.h"
#include "c_base_player.h"
#include "c_base_view_model.h"
#include "c_base_weapon_world_model.h"
#include "c_csgo_player_animstate.h"
#include "c_econ_item_view.h"
#include "c_game_rules.h"
#include "c_hud_texture.h"
#include "c_planted_c4.h"
#include "c_player_resource.h"

