#pragma once

#include "../misc/recv.h"

class client_class;
class i_client_networkable;

typedef i_client_networkable* (*create_client_class_fn)(int entnum, int serialNum);
typedef i_client_networkable* (*create_event_fn)();

class client_class {
public:
    create_client_class_fn      create;
    create_event_fn             create_event;
    char*                    network_name;
    recv_table*               recv_table;
    client_class*             next;
    class_id                  class_id;
};