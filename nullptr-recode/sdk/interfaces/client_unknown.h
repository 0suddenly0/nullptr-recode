#pragma once
#include "handle_entity.h"
#include "collideable.h"

//class ICollideable;
class c_client_networkable;
class c_client_renderable;
class c_client_entity;
class c_base_entity;
class c_client_thinkable;
class c_clientalphaproperty;

class c_client_unknown : public c_handle_entity {
public:
    virtual c_collideable*              get_collideable() = 0;
    virtual c_client_networkable*        get_client_networkable() = 0;
    virtual c_client_renderable*         get_client_renderable() = 0;
    virtual c_client_entity*             get_iclient_entity() = 0;
    virtual c_base_entity*              get_base_entity() = 0;
    virtual c_client_thinkable*         get_client_thinkable() = 0;
    virtual c_clientalphaproperty*     get_client_alpha_property() = 0;
};