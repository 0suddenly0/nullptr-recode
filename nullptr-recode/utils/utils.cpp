#include "utils.h"
#include "../hooks/hooks.h"
#include "../sdk/structures/structures.h"
#include <codecvt>
#include <Tlhelp32.h>

HANDLE _out = NULL, _old_out = NULL;
HANDLE _err = NULL, _old_err = NULL;
HANDLE _in = NULL, _old_in = NULL;

HWND window;

namespace utils {
	vec2 get_screen_size() {
		int w, h;
		sdk::engine_client->get_screen_size(w, h);
		return vec2(w, h);
	}

	std::string snprintf(const char* text, ...) {
		char buf[1024];
		va_list va;

		va_start(va, text);
		_vsnprintf_s(buf, 1024, text, va);
		va_end(va);

		return std::string(buf);
	}

    void detach_console() {
        if (_out && _err && _in) {
            FreeConsole();

            if (_old_out)
                SetStdHandle(STD_OUTPUT_HANDLE, _old_out);
            if (_old_err)
                SetStdHandle(STD_ERROR_HANDLE, _old_err);
            if (_old_in)
                SetStdHandle(STD_INPUT_HANDLE, _old_in);
        }
    }

    bool console_print(const char* fmt, ...) {
        if (!_out)
            return false;

        char buf[1024];
        va_list va;

        va_start(va, fmt);
        _vsnprintf_s(buf, 1024, fmt, va);
        va_end(va);

        return !!WriteConsoleA(_out, buf, (DWORD)(strlen(buf)), nullptr, nullptr);
    }

    char console_read_key() {
        if (!_in) return false;

        auto key = char{ 0 };
        auto keysread = DWORD{ 0 };

        ReadConsoleA(_in, &key, 1, &keysread, nullptr);

        return key;
    }

    void attach_console() {
        _old_out = GetStdHandle(STD_OUTPUT_HANDLE);
        _old_err = GetStdHandle(STD_ERROR_HANDLE);
        _old_in = GetStdHandle(STD_INPUT_HANDLE);

        ::AllocConsole() && ::AttachConsole(GetCurrentProcessId());

        _out = GetStdHandle(STD_OUTPUT_HANDLE);
        _err = GetStdHandle(STD_ERROR_HANDLE);
        _in = GetStdHandle(STD_INPUT_HANDLE);

        SetConsoleMode(_out,
            ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT);

        SetConsoleMode(_in,
            ENABLE_INSERT_MODE | ENABLE_EXTENDED_FLAGS |
            ENABLE_PROCESSED_INPUT | ENABLE_QUICK_EDIT_MODE);
    }

    unsigned int find_in_data_map(datamap_t* p_map, const char* name) {
        while (p_map) {
            for (int i = 0; i < p_map->dataNumFields; i++) {
                if (p_map->dataDesc[i].fieldName == NULL) continue;

                if (strcmp(name, p_map->dataDesc[i].fieldName) == 0) return p_map->dataDesc[i].fieldOffset[TD_OFFSET_NORMAL];

                if (p_map->dataDesc[i].fieldType == FIELD_EMBEDDED) {
                    if (p_map->dataDesc[i].td) {
                        unsigned int offset;

                        if ((offset = find_in_data_map(p_map->dataDesc[i].td, name)) != 0)  return offset;
                    }
                }
            }
            p_map = p_map->baseMap;
        }

        return 0;
    }

    int wait_for_modules(std::int32_t timeout, const std::initializer_list<std::wstring>& modules) {
        bool signaled[32] = { 0 };
        bool success = false;

        std::uint32_t totalSlept = 0;

        if (timeout == 0) {
            for (auto& mod : modules) {
                if (GetModuleHandleW(std::data(mod)) == NULL) return WAIT_TIMEOUT;
            }
            return WAIT_OBJECT_0;
        }

        if (timeout < 0) timeout = INT32_MAX;

        while (true) {
            for (auto i = 0u; i < modules.size(); ++i) {
                auto& module = *(modules.begin() + i);
                if (!signaled[i] && GetModuleHandleW(std::data(module)) != NULL) {
                    signaled[i] = true;

                    //
                    // Checks if all modules are signaled
                    //
                    bool done = true;
                    for (auto j = 0u; j < modules.size(); ++j) {
                        if (!signaled[j]) {
                            done = false;
                            break;
                        }
                    }
                    if (done) {
                        success = true;
                        goto exit;
                    }
                }
            }
            if (totalSlept > std::uint32_t(timeout)) {
                break;
            }
            Sleep(10);
            totalSlept += 10;
        }

    exit:
        return success ? WAIT_OBJECT_0 : WAIT_TIMEOUT;
    }

    std::uint8_t* pattern_scan(void* module, const char* signature) {
        static auto pattern_to_byte = [](const char* pattern) {
            auto bytes = std::vector<int>{};
            auto start = (char*)(pattern);
            auto end = (char*)(pattern) + strlen(pattern);

            for (auto current = start; current < end; ++current) {
                if (*current == '?') {
                    ++current;
                    if (*current == '?')
                        ++current;
                    bytes.push_back(-1);
                } else {
                    bytes.push_back(strtoul(current, &current, 16));
                }
            }
            return bytes;
        };

        auto dosHeader = (PIMAGE_DOS_HEADER)module;
        auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)module + dosHeader->e_lfanew);

        auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
        auto patternBytes = pattern_to_byte(signature);
        auto scanBytes = (std::uint8_t*)(module);

        auto s = patternBytes.size();
        auto d = patternBytes.data();

        for (auto i = 0ul; i < sizeOfImage - s; ++i) {
            bool found = true;
            for (auto j = 0ul; j < s; ++j) {
                if (scanBytes[i + j] != d[j] && d[j] != -1) {
                    found = false;
                    break;
                }
            }
            if (found)  {
                return &scanBytes[i];
            }
        }
        return nullptr;
    }

	std::string current_date_time() {
		time_t     now = time(0);
		struct tm  tstruct;
		char       buf[80];
		tstruct = *localtime(&now);
		strftime(buf, sizeof(buf), "date: %d-%m-%Y | time: %X", &tstruct);

		return buf;
	}

	int epoch_time() {
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}


	float lerp(float a, float b, float f) {
		return a + f * (b - a);
	}

    float get_curtime(c_user_cmd* ucmd) {
        if (!sdk::local_player) return 0;

        int g_tick = 0;
        c_user_cmd* last_cmd = nullptr;
        if (!last_cmd || last_cmd->hasbeenpredicted) {
            g_tick = (float)sdk::local_player->tick_base();
        }
        else {
            ++g_tick;
        }
        last_cmd = ucmd;

        float curtime = g_tick * sdk::global_vars->interval_per_tick;
        return curtime;
    }

    void set_clantag(std::string tag, bool anim) {
        static std::string old_tag;
        std::string p_tag = "";

        if (sdk::player_resource && sdk::local_player) {
            p_tag = std::string((*sdk::player_resource)->clantag()[sdk::local_player->get_index()]);
        }

        if (old_tag != tag || (!anim && (p_tag != "" && p_tag != tag))) {
            static auto set_clantag_fn = (int(__fastcall*)(const char*, const char*))pattern_scan(GetModuleHandleW(L"engine.dll"), "53 56 57 8B DA 8B F9 FF 15");
            set_clantag_fn(tag.c_str(), tag.c_str());

            old_tag = tag;
        }
    }

    std::wstring s2ws(const std::string& str) {
        using convert_typeX = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_typeX, wchar_t> converterX;
        return converterX.from_bytes(str);
    }

    std::string ws2s(const std::wstring& wstr) {
        using convert_typeX = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_typeX, wchar_t> converterX;
        return converterX.to_bytes(wstr);
    }

    int random(const int& min, const int& max) {
        return rand() % (max - min + 1) + min;
    }

    float random(const float& min, const float& max) {
        return ((max - min) * ((float)rand() / RAND_MAX)) + min;
    }
}