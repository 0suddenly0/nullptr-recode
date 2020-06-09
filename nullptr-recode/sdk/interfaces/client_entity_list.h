#pragma once
#include "handle_entity.h"

class c_client_networkable;
class c_client_entity;

class c_client_entity_list {
public:
    virtual c_client_networkable*  get_client_networkable(int entnum) = 0;
    virtual void*                 vtablepad0x1(void) = 0;
    virtual void*                 vtablepad0x2(void) = 0;
    virtual c_client_entity*       get_client_entity(int entNum) = 0;
    virtual c_client_entity*       get_client_entity_from_handle(c_base_handle hEnt) = 0;
    virtual int                   number_of_entities(bool bIncludeNonNetworkable) = 0;
    virtual int                   get_highest_entity_index(void) = 0;
    virtual void                  set_max_entities(int maxEnts) = 0;
    virtual int                   get_max_entities() = 0;
};