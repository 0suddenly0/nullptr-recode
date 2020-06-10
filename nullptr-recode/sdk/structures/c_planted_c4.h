#pragma once 
#include "structures.h"

class c_planted_c4 {
public:
	NETVAR(bool, bomb_ticking, "DT_PlantedC4", "m_bBombTicking");
	NETVAR(bool, bomb_defused, "DT_PlantedC4", "m_bBombDefused");
	NETVAR(float, c4_blow, "DT_PlantedC4", "m_flC4Blow");
	NETVAR(float, timer_length, "DT_PlantedC4", "m_flTimerLength");
	NETVAR(float, defuse_length, "DT_PlantedC4", "m_flDefuseLength");
	NETVAR(float, defuse_count_down, "DT_PlantedC4", "m_flDefuseCountDown");
	NETVAR(c_handle<c_base_player>, m_hBombDefuser, "DT_PlantedC4", "m_hBombDefuser");
};