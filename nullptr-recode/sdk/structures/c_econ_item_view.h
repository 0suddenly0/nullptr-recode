#pragma once

class c_econ_item_view {
private:
	using str_32 = char[32];
public:
	NETVAR(int32_t, initialized, "DT_BaseAttributableItem", "m_bInitialized");
	NETVAR(int, item_definition_index, "DT_BaseAttributableItem", "m_iItemDefinitionIndex");
	NETVAR(int32_t, entity_level, "DT_BaseAttributableItem", "m_iEntityLevel");
	NETVAR(int32_t, account_id, "DT_BaseAttributableItem", "m_iAccountID");
	NETVAR(int32_t, item_id_low, "DT_BaseAttributableItem", "m_iItemIDLow");
	NETVAR(int32_t, item_id_high, "DT_BaseAttributableItem", "m_iItemIDHigh");
	NETVAR(int32_t, entity_quality, "DT_BaseAttributableItem", "m_iEntityQuality");
	NETVAR(str_32, custom_name, "DT_BaseAttributableItem", "m_szCustomName");
};