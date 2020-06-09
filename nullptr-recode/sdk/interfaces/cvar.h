#pragma once

#include "app_system.h"
#include "convar.h"

class con_command_base;
class con_command;
class convar;

typedef int c_var_dll_identifier_t;

class c_console_display_func {
public:
    virtual void color_print(const uint8_t* clr, const char *pMessage) = 0;
    virtual void print(const char *pMessage) = 0;
    virtual void dprint(const char *pMessage) = 0;
};

class c_cvar : public c_app_system {
public:
    virtual c_var_dll_identifier_t     allocate_dll_identifier() = 0; // 9
    virtual void                       register_con_command(con_command_base *pCommandBase) = 0; //10
    virtual void                       unregister_con_command(con_command_base *pCommandBase) = 0;
    virtual void                       unregister_con_commands(c_var_dll_identifier_t id) = 0;
    virtual const char*                get_Command_line_value(const char *pVariableName) = 0;
    virtual con_command_base*          find_command_base(const char *name) = 0;
    virtual const con_command_base*    find_command_base(const char *name) const = 0;
    virtual convar*                    find_var(const char *var_name) = 0; //16
    virtual const convar*              find_var(const char *var_name) const = 0;
    virtual con_command*               find_command(const char *name) = 0;
    virtual const con_command*         find_command(const char *name) const = 0;
    virtual void                       install_global_change_callback(FnChangeCallback_t callback) = 0;
    virtual void                       remove_global_change_callback(FnChangeCallback_t callback) = 0;
    virtual void                       call_global_change_callbacks(convar *var, const char *pOldString, float flOldValue) = 0;
    virtual void                       install_console_display_func(c_console_display_func* pDisplayFunc) = 0;
    virtual void                       remove_console_display_func(c_console_display_func* pDisplayFunc) = 0;
    virtual void                       console_color_printf(const color& clr, const char *pFormat, ...) const = 0;
    virtual void                       console_printf(const char *pFormat, ...) const = 0;
    virtual void                       console_dprintf(const char *pFormat, ...) const = 0;
    virtual void                       revert_flagged_convars(int nFlag) = 0;
};
