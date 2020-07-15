#pragma once 
#include "c_base_attributable_item.h"
#include "c_base_player.h"
#include "c_base_inferno.h"

class c_base_combat_weapon : public c_base_attributable_item {
public:
	NETVAR(float_t, next_primary_attack, "DT_BaseCombatWeapon", "m_flNextPrimaryAttack");
	NETVAR(float_t, next_secondary_attack, "DT_BaseCombatWeapon", "m_flNextSecondaryAttack");
	NETVAR(int32_t, clip1, "DT_BaseCombatWeapon", "m_iClip1");
	NETVAR(int32_t, clip2, "DT_BaseCombatWeapon", "m_iClip2");
	NETVAR(bool, started_arming, "DT_WeaponC4", "m_bStartedArming");
	NETVAR(float_t, recoil_index, "DT_WeaponCSBase", "m_flRecoilIndex");
	NETVAR(int32_t, view_model_index, "DT_BaseCombatWeapon", "m_iViewModelIndex");
	NETVAR(int32_t, world_model_index, "DT_BaseCombatWeapon", "m_iWorldModelIndex");
	NETVAR(int32_t, world_dropped_model_index, "DT_BaseCombatWeapon", "m_iWorldDroppedModelIndex");
	NETVAR(bool, pin_pulled, "DT_BaseCSGrenade", "m_bPinPulled");
	NETVAR(float_t, throw_time, "DT_BaseCSGrenade", "m_fThrowTime");
	NETVAR(float_t, postpone_fire_ready_time, "DT_BaseCombatWeapon", "m_flPostponeFireReadyTime");
	NETVAR(c_handle<c_base_weapon_world_model>, weapon_world_model, "DT_BaseCombatWeapon", "m_hWeaponWorldModel");
	NETVAR(int32_t, weapon_mode, "DT_WeaponCSBase", "m_weaponMode");
	NETVAR(int32_t, zoom_level, "DT_WeaponCSBaseGun", "m_zoomLevel");


	c_cs_weapon_info* get_cs_weapondata() {
		return sdk::weapon_system->get_wpn_data(this->item().item_definition_index());
	}

	bool has_bullets() {
		return !is_reloading() && clip1() > 0;
	}

	bool can_fire() {
		static decltype(this) stored_weapon = nullptr;
		static auto stored_tick = 0;
		if (stored_weapon != this || stored_tick >= sdk::local_player->tick_base()) {
			stored_weapon = this;
			stored_tick = sdk::local_player->tick_base();
			return false;
		}

		if (is_reloading() || clip1() <= 0 || !sdk::local_player || !sdk::engine_client)
			return false;

		auto flServerTime = sdk::local_player->tick_base() * sdk::global_vars->interval_per_tick;

		return next_primary_attack() <= flServerTime;
	}

	bool is_grenade() {
		return get_cs_weapondata()->weapon_type == weapon_type_grenade;
	}

	bool is_knife() {
		if (this->item().item_definition_index() == item_definition_index::taser) return false;
		return get_cs_weapondata()->weapon_type == weapon_type_knife;
	}

	bool is_zeus() {
		if (this->item().item_definition_index() == taser) return true;
		return false;
	}

	bool is_reloading() {
		static auto inReload = *(uint32_t*)(utils::pattern_scan(GetModuleHandleW(L"client.dll"), "C6 87 ? ? ? ? ? 8B 06 8B CE FF 90") + 2);
		return *(bool*)((uintptr_t)this + inReload);
	}

	bool is_rifle() {
		switch (get_cs_weapondata()->weapon_type) {
		case weapon_type_rifle: return true;
		case weapon_type_submachinegun: return true;
		case weapon_type_shotgun: return true;
		case weapon_type_machinegun: return true;
		default: return false;
		}
	}

	bool is_pistol() {
		switch (get_cs_weapondata()->weapon_type) {
		case weapon_type_pistol: return true;
		default: return false;
		}
	}

	bool is_sniper() {
		switch (get_cs_weapondata()->weapon_type) {
		case weapon_type_sniper_rifle: return true;
		default: return false;
		}
	}

	bool is_gun() {
		switch (get_cs_weapondata()->weapon_type) {
		case weapon_type_c4: return false;
		case weapon_type_grenade: return false;
		case weapon_type_knife: return false;
		case weapon_type_unknown: return false;
		default: return true;
		}
	}

	float get_inaccuracy() {
		return call_vfunction<float(__thiscall*)(void*)>(this, 482)(this);
	}

	float get_spread() {
		return call_vfunction<float(__thiscall*)(void*)>(this, 452)(this);
	}

	void update_accuracy_penalty() {
		call_vfunction<void(__thiscall*)(void*)>(this, 483)(this);
	}

	c_utl_vector<c_ref_counted*>& custom_materials() {
		static auto inReload = *(uint32_t*)(utils::pattern_scan(GetModuleHandleW(L"client.dll"), "83 BE ? ? ? ? ? 7F 67") + 2) - 12;
		return *(c_utl_vector<c_ref_counted*>*)((uintptr_t)this + inReload);
	}

	bool* custom_material_initialized() {
		static auto currentCommand = *(uint32_t*)(utils::pattern_scan(GetModuleHandleW(L"client.dll"), "C6 86 ? ? ? ? ? FF 50 04") + 2);
		return (bool*)((uintptr_t)this + currentCommand);
	}

	int get_item_definition_index() {
		if (!this) return 0;
		return this->item().item_definition_index();
	}

};
