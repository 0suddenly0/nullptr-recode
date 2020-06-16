#pragma once

class c_game_rules {
public:
	NETVAR(int, match_stats_players_alive_t, "DT_CSGameRulesProxy", "m_iMatchStats_PlayersAlive_T");
	NETVAR(int, match_stats_Players_alive_ct, "DT_CSGameRulesProxy", "m_iMatchStats_PlayersAlive_CT");
	NETVAR(int, round_time, "DT_CSGameRulesProxy", "m_iRoundTime");
	NETVAR(bool, freeze_period, "DT_CSGameRulesProxy", "m_bBombDropped");
	NETVAR(bool, is_valve_ds, "DT_CSGameRulesProxy", "m_bIsValveDS");
	NETVAR(bool, bomb_dropped, "DT_CSGameRulesProxy", "m_bBombDropped");
	NETVAR(bool, bomb_planted, "DT_CSGameRulesProxy", "m_bBombPlanted");
	NETVAR(float, survival_start_time, "DT_CSGameRulesProxy", "m_flSurvivalStartTime");
};