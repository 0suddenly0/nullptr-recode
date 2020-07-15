#pragma once
#include "../../helpers/helpers.h"
#include "../misc/vfunc.h"

class c_material_var {
public:
	void set_value(float value) {
		return call_vfunction<void(__thiscall*)(void*, float)>(this, 4)(this, value);
	}

	void set_value(int value) {
		return call_vfunction<void(__thiscall*)(void*, int)>(this, 5)(this, value);
	}

	void set_value(char const* value) {
		return call_vfunction<void(__thiscall*)(void*, char const*)>(this, 6)(this, value);
	}

	void set_value(const vec3& value) {
		return call_vfunction<void(__thiscall*)(void*, float, float, float)>(this, 11)(this, value.x, value.y, value.z);
	}
};