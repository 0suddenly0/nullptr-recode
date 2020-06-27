#pragma once

#include "global_vars.h"
#include "client_class.h"

enum class frame_stage_t {
	undefined = -1,
	start,
	net_update_start,
	net_update_post_data_update_start,
	net_update_post_data_update_end,
	net_update_end,
	render_start,
	render_end
};

// Used by RenderView
enum render_view_info_t {
	RENDERVIEW_UNSPECIFIED = 0,
	RENDERVIEW_DRAWVIEWMODEL = (1 << 0),
	RENDERVIEW_DRAWHUD = (1 << 1),
	RENDERVIEW_SUPPRESSMONITORRENDERING = (1 << 2),
};

class c_base_client_dll {
public:
	virtual int              connect(create_interface_fn appSystemFactory, c_global_vars_base*pGlobals) = 0;
	virtual int              disconnect(void) = 0;
	virtual int              init(create_interface_fn appSystemFactory, c_global_vars_base*pGlobals) = 0;
	virtual void             post_init() = 0;
	virtual void             shutdown(void) = 0;
	virtual void             level_init_pre_entity(char const* pMapName) = 0;
	virtual void             level_init_post_entity() = 0;
	virtual void             level_shutdown(void) = 0;
	virtual client_class*    get_all_classes(void) = 0;

	bool dispatch_user_message(int messageType, int arg, int arg1, void* data)
	{
		using DispatchUserMessage_t = bool* (__thiscall*)(void*, int, int, int, void*);
		return call_vfunction<DispatchUserMessage_t>(this, 38)(this, messageType, arg, arg1, data);
	}
};