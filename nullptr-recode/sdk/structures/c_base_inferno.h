#pragma once
class c_base_inferno : c_base_entity {
private:
	using int64 = int[64];
public:
	NETVAR(vec3, vec_origin, "DT_BaseEntity", "m_vecOrigin");
	NETVAR(int, fire_count, "DT_Inferno", "m_fireCount");
	NETVAR(int64, fire_x, "DT_Inferno", "m_fireXDelta");
	NETVAR(int64, fire_y, "DT_Inferno", "m_fireYDelta");
	NETVAR(int64, fire_z, "DT_Inferno", "m_fireZDelta");
	NETVAR(vec3, mins, "DT_CollisionProperty", "m_vecMins");
	NETVAR(vec3, maxs, "DT_CollisionProperty", "m_vecMaxs");
};