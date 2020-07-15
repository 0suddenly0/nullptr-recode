#pragma once
#include "../../utils/utils.h"

class c_key_values {
public:
	void init(const char* key_name) {
		static const auto init_fn = (void(__thiscall*)(void*, const char*))utils::pattern_scan(GetModuleHandleA("client.dll"), "55 8B EC 51 33 C0 C7 45");
		init_fn(this, key_name);
	}

	void load_from_buffer(const char* resource_name, const char* buf, void* file_sys = nullptr, const char* path_id = nullptr, void* eval_sym_proc = nullptr, void* unk = nullptr) {
		static const auto load_from_buffer_fn = (void(__thiscall*)(void*, const char*, const char*, void*, const char*, void*, void*))utils::pattern_scan(GetModuleHandleA("client.dll"), "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89");
		return load_from_buffer_fn(this, resource_name, buf, file_sys, path_id, eval_sym_proc, unk);
	}

	c_key_values* find_key(const char* name, bool create) {
		static const auto find_key_fn = (c_key_values*(__thiscall*)(void*, const char*, bool))utils::pattern_scan(GetModuleHandleA("client.dll"), "55 8B EC 83 EC 1C 53 8B D9 85 DB");
		return find_key_fn(this, name, create);
	}

	void set_int(const char* name, int value) {
		auto key = find_key(name, true);
		if (!key)
			return;

		*reinterpret_cast<char*>(reinterpret_cast<uintptr_t>(key) + 0x10) = 2;
		*reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(key) + 0xC) = value;
	}

	void set_string(const char* name, const char* a2) {
		auto key = find_key(name, true);
		if (!key) return;
		static const auto set_string_fn = (void(__thiscall*)(void*, const char*))utils::pattern_scan(GetModuleHandleA("client.dll"), "55 8B EC A1 ? ? ? ? 53 56 57 8B F9 8B 08 8B 01");
		set_string_fn(key, a2);
	}
};