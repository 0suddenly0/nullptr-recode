#pragma once
#include "../../sdk/sdk.h"
/*
class c_event_manager : public c_game_event_listener {
public:
	void initialization();
	void shutdown();

private:
	virtual void fire_game_event(c_game_event* event);
	virtual int  get_event_debug_id(void);
};

extern c_event_manager event_manager;*/

namespace event_manager {
	void initialization();
	void shutdown();
}