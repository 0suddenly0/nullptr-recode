#pragma once
#include "structures.h"

class c_base_view_model : public c_base_entity {
public:
	NETVAR(int32_t, modeli_ndex, "DT_BaseViewModel", "m_nModelIndex");
	NETVAR(int32_t, view_model_index, "DT_BaseViewModel", "m_nViewModelIndex");
	NETVAR(c_handle<c_base_combat_weapon>, weapon, "DT_BaseViewModel", "m_hWeapon");
	NETVAR(c_handle<c_base_player>, owner, "DT_BaseViewModel", "m_hOwner");
	NETPROP(sequence, "DT_BaseViewModel", "m_nSequence");

	void send_view_model_matching_sequence(int sequence) {
		return call_vfunction<void(__thiscall*)(void*, int)>(this, 241)(this, sequence);
	}
};