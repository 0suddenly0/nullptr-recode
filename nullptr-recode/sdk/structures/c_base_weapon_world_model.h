#pragma once 

class c_base_weapon_world_model : public c_base_entity
{
public:
	NETVAR(int32_t, model_index, "DT_BaseWeaponWorldModel", "m_nModelIndex");
};