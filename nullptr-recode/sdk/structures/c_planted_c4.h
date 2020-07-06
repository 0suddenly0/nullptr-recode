#pragma once 

class c_planted_c4 {
public:
	static float get_defuse_time(c_planted_c4* bomb) {
		static float defuse_time = -1;
		if (!bomb) return 0;
		if (!bomb->bomb_defuser()) defuse_time = -1;
		else if (defuse_time == -1) defuse_time = sdk::global_vars->curtime + bomb->defuse_length();
		if (defuse_time > -1 && bomb->bomb_defuser()) return defuse_time - sdk::global_vars->curtime;
		return 0;
	}

	static c_base_player* get_bomb_player() {
		for (int i = 1; i <= sdk::entity_list->get_highest_entity_index(); ++i) {
			c_base_player* entity = c_base_player::get_player_by_index(i);
			if (!entity || entity->is_player() || entity->is_dormant() || entity == sdk::local_player)
				continue;

			if (entity->is_planted_c4())
				return entity;
		}

		return nullptr;
	}

	static c_planted_c4* get_bomb() {
		c_base_entity* entity;
		for (int i = 1; i <= sdk::entity_list->get_max_entities(); ++i) {
			entity = c_base_entity::get_entity_by_index(i);
			if (entity && !entity->is_dormant() && entity->is_planted_c4())
				return (c_planted_c4*)(entity);
		}

		return nullptr;
	}

	static float get_bomb_damage() {
		c_base_player* bomb = c_base_player::get_spectating_player();

		if (!bomb)
			return 0.f;

		float armor = c_base_player::get_spectating_player()->armor_value();
		float distance = c_base_player::get_spectating_player()->get_eye_pos().dist_to(bomb->get_abs_origin());

		float a = 450.7f;
		float b = 75.68f;
		float c = 789.2f;
		float d = ((distance - b) / c);
		float damage = a * exp(-d * d);

		float dmg = damage;

		if (armor > 0)
		{
			float _new = dmg * 0.5f;
			float armor = (dmg - _new) * 0.5f;

			if (armor > (float)(armor))
			{
				armor = float(armor) * (1.f / 0.5f);
				_new = dmg - armor;
			}

			damage = _new;
		}
		return damage;
	}

	NETVAR(bool, bomb_ticking, "DT_PlantedC4", "m_bBombTicking");
	NETVAR(bool, bomb_defused, "DT_PlantedC4", "m_bBombDefused");
	NETVAR(float, c4_blow, "DT_PlantedC4", "m_flC4Blow");
	NETVAR(float, timer_length, "DT_PlantedC4", "m_flTimerLength");
	NETVAR(float, defuse_length, "DT_PlantedC4", "m_flDefuseLength");
	NETVAR(float, defuse_count_down, "DT_PlantedC4", "m_flDefuseCountDown");
	NETVAR(c_handle<c_base_player>, bomb_defuser, "DT_PlantedC4", "m_hBombDefuser");
};