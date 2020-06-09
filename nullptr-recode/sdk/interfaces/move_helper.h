#pragma once
class c_client_entity;

class c_move_helper
{
public:
    virtual	void _vpad() = 0;
    virtual void set_host(c_client_entity* host) = 0;
};
