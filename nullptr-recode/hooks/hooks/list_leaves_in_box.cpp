#include "../hooks.h"

struct renderable_info_t {
	c_client_renderable* m_pRenderable;
	void* m_pAlphaProperty;
	int m_EnumCount;
	int m_nRenderFrame;
	unsigned short m_FirstShadow;
	unsigned short m_LeafList;
	short m_Area;
	uint16_t m_Flags;   // 0x0016
	uint16_t m_Flags2; // 0x0018
	vec3 m_vecBloatedAbsMins;
	vec3 m_vecBloatedAbsMaxs;
	vec3 m_vecAbsMins;
	vec3 m_vecAbsMaxs;
	int pad;
};

#define MAX_COORD_FLOAT ( 16384.0f )
#define MIN_COORD_FLOAT ( -MAX_COORD_FLOAT )

namespace hooks {
	int __fastcall list_leaves_in_box::hook(void* _this, int, vec3& mins, vec3& maxs, unsigned short* pList, int listMax) {
		static auto o_list_leaves_in_box = bsp_query_vhook.get_original<fn>(indexes::list_leaves_in_box);
		static uint8_t* return_address = utils::pattern_scan(GetModuleHandleW(L"client.dll"), "52 8B 55 0C 8B 01 56 52 FF 50 18 89 44 24 14") + 0xB;

		if (_ReturnAddress() != return_address)
			return o_list_leaves_in_box(_this, mins, maxs, pList, listMax);

		auto info = *(renderable_info_t**)(uintptr_t(_AddressOfReturnAddress()) + 0x14);
		if (!info || !info->m_pRenderable)
			return o_list_leaves_in_box(_this, mins, maxs, pList, listMax);

		auto ent = info->m_pRenderable->get_client_unknown()->get_base_entity();
		if (ent && ent->is_player()) {
			info->m_Flags &= ~0x100;
			info->m_Flags2 |= 0xC0;

			static vec3 map_min = vec3(MIN_COORD_FLOAT, MIN_COORD_FLOAT, MIN_COORD_FLOAT);
			static vec3 map_max = vec3(MAX_COORD_FLOAT, MAX_COORD_FLOAT, MAX_COORD_FLOAT);

			return o_list_leaves_in_box(_this, map_min, map_max, pList, listMax);
		}
		return o_list_leaves_in_box(_this, mins, maxs, pList, listMax);
	}
}