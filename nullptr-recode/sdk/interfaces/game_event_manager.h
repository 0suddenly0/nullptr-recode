#pragma once

#include <cstdint>

#define EVENT_DEBUG_ID_INIT 42 
#define EVENT_DEBUG_ID_SHUTDOWN 13  

class bf_write;
class bf_read;
class c_game_event {
public:
	virtual ~c_game_event() {};
	virtual const char*    get_name() const = 0;
	virtual bool           is_reliable() const = 0;
	virtual bool           is_local() const = 0;
	virtual bool           is_empty(const char* keyname = nullptr) = 0;
	virtual bool           get_bool(const char* keyname = nullptr, bool default_value = false) = 0;
	virtual int            get_int(const char* keyname = nullptr, int default_value = 0) = 0;
	virtual uint64_t       get_uint64(const char* keyname = nullptr, uint64_t default_value = 0) = 0;
	virtual float          get_float(const char* keyname = nullptr, float default_value = 0.0f) = 0;
	virtual const char*    get_string(const char* keyname = nullptr, const char* default_value = "") = 0;
	virtual const wchar_t* get_wstring(const char* keyname = nullptr, const wchar_t* default_value = L"") = 0;
	virtual const void*    get_ptr(const char* keyname = nullptr, const void* default_values = nullptr) = 0;
	virtual void           set_bool(const char* keyname, bool value) = 0;
	virtual void           set_int(const char* keyname, int value) = 0;
	virtual void           set_uint64(const char* keyname, uint64_t value) = 0;
	virtual void           set_float(const char* keyname, float value) = 0;
	virtual void           set_string(const char* keyname, const char* value) = 0;
	virtual void           set_wstring(const char* keyname, const wchar_t* value) = 0;
	virtual void           set_ptr(const char* keyname, const void* value) = 0;
};

class c_game_event_listener {
public:
    virtual ~c_game_event_listener(void) {}

    virtual void fire_game_event(c_game_event* event) = 0;
    virtual int  get_event_debug_id(void) = 0;

public:
    int m_iDebugId;
};

class c_game_event_manager {
public:
    virtual             ~c_game_event_manager() = 0;
    virtual int         load_events_from_file(const char *filename) = 0;
    virtual void        reset() = 0;
    virtual bool        add_listener(c_game_event_listener*listener, const char *name, bool bServerSide) = 0;
    virtual bool        find_listener(c_game_event_listener*listener, const char *name) = 0;
    virtual int         remove_listener(c_game_event_listener*listener) = 0;
    virtual c_game_event* create_event(const char *name, bool bForce, unsigned int dwUnknown) = 0;
    virtual bool        fire_event(c_game_event*event, bool bDontBroadcast = false) = 0;
    virtual bool        fire_event_client_side(c_game_event*event) = 0;
    virtual c_game_event* duplicate_event(c_game_event*event) = 0;
    virtual void        free_event(c_game_event*event) = 0;
    virtual bool        serialize_event(c_game_event*event, bf_write *buf) = 0;
    virtual c_game_event* unserialize_event(bf_read *buf) = 0;
};