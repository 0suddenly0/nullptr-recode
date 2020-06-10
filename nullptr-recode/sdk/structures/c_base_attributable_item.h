#pragma once
#include "structures.h"

class c_base_attributable_item : public c_base_entity {
public:
	NETVAR(uint64_t, original_owner_xuid, "DT_BaseAttributableItem", "m_OriginalOwnerXuidLow");
	NETVAR(int32_t, original_owner_xuid_low, "DT_BaseAttributableItem", "m_OriginalOwnerXuidLow");
	NETVAR(int32_t, original_owner_xuid_high, "DT_BaseAttributableItem", "m_OriginalOwnerXuidHigh");
	NETVAR(int32_t, fallback_stat_trak, "DT_BaseAttributableItem", "m_nFallbackStatTrak");
	NETVAR(int32_t, fallback_paint_kit, "DT_BaseAttributableItem", "m_nFallbackPaintKit");
	NETVAR(int32_t, fallback_seed, "DT_BaseAttributableItem", "m_nFallbackSeed");
	NETVAR(float_t, fallback_wear, "DT_BaseAttributableItem", "m_flFallbackWear");

	c_econ_item_view& item() {
		return *(c_econ_item_view*)this;
	}

	void set_glove_model_index(int modelIndex) {
		return call_vfunction<void(__thiscall*)(void*, int)>(this, 75)(this, modelIndex);
	}

	void set_model_index(const int index) {
		return call_vfunction<void(__thiscall*)(c_base_entity*, int)>(this, 75)(this, index);
	}

};