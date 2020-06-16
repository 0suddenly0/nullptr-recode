#pragma once

#include "../misc/recv.h"

class client_class;
class i_client_networkable;

typedef i_client_networkable* (*Createclient_classFn)(int entnum, int serialNum);
typedef i_client_networkable* (*CreateEventFn)();

class client_class
{
public:
    Createclient_classFn      create_fn;
    CreateEventFn            create_event_fn;
    char*                    network_name;
    recv_table*               recv_table;
    client_class*             next;
    class_id                  class_id;
};