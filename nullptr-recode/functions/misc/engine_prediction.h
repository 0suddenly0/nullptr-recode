#pragma once
#include "../../sdk/structures/structures.h"

namespace engine_prediction {
	extern float _curtime_backup;
	extern float _frametime_backup;
	extern c_move_data _movedata;
	extern c_user_cmd* _prevcmd;
	extern int _fixedtick;

	extern int32_t* _prediction_seed;
	extern c_base_player*** _prediction_player;

	void begin(c_user_cmd* cmd);
	void end();
}