#pragma once

class c_csplayer_resource
{
private:
	using int_65 = int[65];
public:
	NETPROP(iteam, "DT_CSPlayerResource", "m_iTeam");
	NETVAR(int_65, iping, "DT_CSPlayerResource", "m_iPing");
};

class c_player_resource : c_csplayer_resource {
private:
	using int_65 = int[65];
	using char6516 = char[65][16];
	using str_32 = char[32];
	using unsigned65 = unsigned[65];
public:
	NETVAR(int_65, ranking, "DT_CSPlayerResource", "m_iCompetitiveRanking");
	NETVAR(int_65, wins, "DT_CSPlayerResource", "m_iCompetitiveWins");
	NETVAR(int_65, team_color,"DT_CSPlayerResource", "m_iCompTeammateColor");
	NETVAR(char6516, clantag, "DT_CSPlayerResource" /*"CCSPlayerResource"*/, "m_szClan");
	/*const char* clantag(int index) {
		static int _m_flPoseParameter = netvar::get_offset("DT_CSPlayerResource", "m_szClan");
		return (const char*)((uintptr_t)this + _m_flPoseParameter + index * 16);
	}*/
	NETVAR(unsigned65, coin, "DT_CSPlayerResource", "m_nActiveCoinRank");
	NETVAR(unsigned65, musik_kit, "DT_CSPlayerResource", "m_nMusicID");
};