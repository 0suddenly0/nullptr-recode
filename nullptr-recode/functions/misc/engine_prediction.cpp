#include "engine_prediction.h"

namespace engine_prediction {
	float _curtime_backup = 0.f;
	float _frametime_backup = 0.f;
	c_move_data _movedata;
	c_user_cmd* _prevcmd = nullptr;
	int _fixedtick = 0;

	int32_t* _prediction_seed = nullptr;
	c_base_player*** _prediction_player = nullptr;

	void begin(c_user_cmd* cmd)
	{
		_curtime_backup = sdk::global_vars->curtime;
		_frametime_backup = sdk::global_vars->frametime;

		if (!_prevcmd || _prevcmd->hasbeenpredicted) {
			_fixedtick = sdk::local_player->tick_base();
		}
		else {
			_fixedtick++;
		}

		if (!_prediction_seed || !_prediction_player) {
			auto client = GetModuleHandle(TEXT("client.dll"));

			_prediction_seed = *(int32_t**)(utils::pattern_scan(client, "8B 0D ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 83 C4 04") + 0x2);
			_prediction_player = (c_base_player***)(utils::pattern_scan(client, "89 35 ? ? ? ? F3 0F 10 48 20") + 0x2);
		}

		if (_prediction_seed) {
			*_prediction_seed = cmd->random_seed;
		}

		if (_prediction_player) {
			**_prediction_player = sdk::local_player;
		}

		sdk::local_player->current_command() = cmd;

		sdk::global_vars->curtime = static_cast<float>(_fixedtick)* sdk::global_vars->interval_per_tick;
		sdk::global_vars->frametime = sdk::global_vars->interval_per_tick;

		bool _inpred_backup = *(bool*)((uintptr_t)sdk::prediction + 0x8);

		*(bool*)((uintptr_t)sdk::prediction + 0x8) = true;

		sdk::move_helper->set_host(sdk::local_player);

		sdk::game_movement->start_track_prediction_errors(sdk::local_player);
		sdk::prediction->setup_move(sdk::local_player, cmd, sdk::move_helper, &_movedata);
		sdk::game_movement->process_movement(sdk::local_player, &_movedata);
		sdk::prediction->finish_move(sdk::local_player, cmd, &_movedata);
		sdk::game_movement->finish_track_prediction_errors(sdk::local_player);

		*(bool*)((uintptr_t)sdk::prediction + 0x8) = _inpred_backup;

		sdk::move_helper->set_host(nullptr);

		if (_prediction_seed) {
			*_prediction_seed = -1;
		}

		if (_prediction_player) {
			**_prediction_player = nullptr;
		}

		sdk::local_player->current_command() = nullptr;

		_prevcmd = cmd;
	}

	void end() {
		sdk::global_vars->curtime = _curtime_backup;
		sdk::global_vars->frametime = _frametime_backup;
	}
}