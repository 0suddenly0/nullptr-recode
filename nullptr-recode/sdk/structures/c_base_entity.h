#pragma once
#include "structures.h"

class c_base_entity : public c_client_entity {
public:
	datamap_t* get_data_desc_map() {
		typedef datamap_t* (__thiscall* o_GetPredDescMap)(void*);
		return call_vfunction<o_GetPredDescMap>(this, 15)(this);
	}

	datamap_t* get_pred_desc_map() {
		typedef datamap_t* (__thiscall* o_GetPredDescMap)(void*);
		return call_vfunction<o_GetPredDescMap>(this, 17)(this);
	}
	static __forceinline c_base_entity* get_entity_by_index(int index) {
		return static_cast<c_base_entity*>(sdk::entity_list->get_client_entity(index));
	}
	static __forceinline c_base_entity* get_entity_from_handle(c_base_handle h) {
		return static_cast<c_base_entity*>(sdk::entity_list->get_client_entity_from_handle(h));
	}

	NETVAR(int32_t, model_index, "DT_BaseEntity", "m_nModelIndex");
	NETVAR(int32_t, team_num, "DT_BaseEntity", "m_iTeamNum");
	NETVAR(vec3, vec_origin, "DT_BaseEntity", "m_vecOrigin");
	NETVAR(vec3, vec_angles, "DT_BaseEntity", "m_vecAngles");
	NETVAR(bool, should_glow, "DT_DynamicProp", "m_bShouldGlow");
	NETVAR(int32_t, hitbox_set, "DT_BaseAnimating", "m_nHitboxSet");
	NETVAR(int32_t, life_state, "DT_BasePlayer", "m_lifeState");
	NETVAR(c_handle<c_base_player>, owner_entity, "DT_BaseEntity", "m_hOwnerEntity");
	NETVAR(bool, spotted, "DT_BaseEntity", "m_bSpotted");
	NETVAR(float_t, c4_blow, "DT_PlantedC4", "m_flC4Blow");
	NETVAR_EX(int, get_index, "DT_BaseEntity", "m_bIsAutoaimTarget", +0x4);

	void set_module_index(int index) {
		return call_vfunction<void(__thiscall*)(void*, int)>(this, 75)(this, index);
	}

	const matrix3x4& coordinate_frame() {
		static auto _m_rgflCoordinateFrame = netvar::get_offset("DT_BaseEntity", "m_CollisionGroup") - 0x30;
		return *(matrix3x4*)((uintptr_t)this + _m_rgflCoordinateFrame);
	}

	bool is_player() {
		return call_vfunction<bool(__thiscall*)(c_base_entity*)>(this, 157)(this);
	}
	bool is_loot() {
		return (get_client_class()->class_id == ClassId_CPhysPropAmmoBox ||
			get_client_class()->class_id == ClassId_CPhysPropLootCrate ||
			get_client_class()->class_id == ClassId_CPhysPropRadarJammer ||
			get_client_class()->class_id == ClassId_CPhysPropWeaponUpgrade ||
			get_client_class()->class_id == ClassId_CDrone ||
			get_client_class()->class_id == ClassId_CDronegun ||
			get_client_class()->class_id == ClassId_CItem_Healthshot ||
			get_client_class()->class_id == ClassId_CItemCash);
	}
	bool is_weapon() {
		return call_vfunction<bool(__thiscall*)(c_base_entity*)>(this, 165)(this);
	}
	bool is_planted_c4() {
		return get_client_class()->class_id == 128;
	}
	bool is_defuse_kit() {
		return get_client_class()->class_id == ClassId_CBaseAnimating;
	}
	matrix3x4 get_bone_matrix(int BoneID) {
		matrix3x4 matrix;

		uintptr_t m_dwBoneMatrix = 0x26A8;
		auto offset = *reinterpret_cast<uintptr_t*>(uintptr_t(this) + m_dwBoneMatrix);
		if (offset)
			matrix = *reinterpret_cast<matrix3x4*>(offset + 0x30 * BoneID);

		return matrix;
	}

	vec3& get_abs_origin() {
		return call_vfunction<vec3 & (__thiscall*)(void*)>(this, 10)(this);
	}

	void set_abs_original(vec3 origin) {
		using SetAbsOriginFn = void(__thiscall*)(void*, const vec3 & origin);
		static SetAbsOriginFn set_abs_origin;
		if (!set_abs_origin)
		{
			set_abs_origin = (SetAbsOriginFn)((DWORD)utils::pattern_scan(GetModuleHandleW(L"client.dll"), "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8"));
		}
		set_abs_origin(this, origin);
	}
};
