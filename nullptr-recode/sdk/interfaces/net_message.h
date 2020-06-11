#pragma once

class c_net_message {
public:
	virtual ~c_net_message() {};

	int get_type() {
		return call_vfunction<int(__thiscall*)(void*)>(this, 6)(this);
	}

	int get_group() {
		return call_vfunction<int(__thiscall*)(void*)>(this, 7)(this);
	}

	const char* get_name() {
		return call_vfunction<const char* (__thiscall*)(void*)>(this, 8)(this);
	}
};