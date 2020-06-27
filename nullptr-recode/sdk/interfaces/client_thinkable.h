#pragma once

class   c_client_unknown;
class   c_clientthinkhandle_ptr;
typedef c_clientthinkhandle_ptr* clientthinkhandle_t;

class c_client_thinkable {
public:
    virtual c_client_unknown*     get_iclient_unknown() = 0;
    virtual void                client_think() = 0;
    virtual clientthinkhandle_t get_think_handle() = 0;
    virtual void                set_think_handle(clientthinkhandle_t hThink) = 0;
    virtual void                release() = 0;
};