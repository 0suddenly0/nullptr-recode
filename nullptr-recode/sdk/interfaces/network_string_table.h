#pragma once
class c_network_string_table {
public:
	virtual				~c_network_string_table(void) {};
	virtual const char* get_table_name(void) const = 0;
	virtual int			get_table_id(void) const = 0;
	virtual int			get_num_strings(void) const = 0;
	virtual int			get_max_strings(void) const = 0;
	virtual int			get_entry_bits(void) const = 0;
	virtual void		set_tick(int tick) = 0;
	virtual bool		changed_since_tick(int tick) const = 0;
	virtual int			add_string(bool bIsServer, const char* value, int length = -1, const void* userdata = 0) = 0;
	virtual const char* get_string(int stringNumber) = 0;
	virtual void		set_string_user_data(int stringNumber, int length, const void* userdata) = 0;
	virtual const void* get_string_user_data(int stringNumber, int* length) = 0;
	virtual int			find_string_index(char const* string) = 0;

};

class c_network_string_table_container {
public:
	virtual					        ~c_network_string_table_container(void) {};
	virtual c_network_string_table* create_string_table(const char* tableName, int maxentries, int userdatafixedsize = 0, int userdatanetworkbits = 0) = 0;
	virtual void				    remove_all_tables(void) = 0;
	virtual c_network_string_table* find_table(const char* tableName) const = 0;
	virtual c_network_string_table* get_table(int stringTable) const = 0;
	virtual int					    get_num_tables(void) const = 0;
	virtual c_network_string_table* create_string_table_ex(const char* tableName, int maxentries, int userdatafixedsize = 0, int userdatanetworkbits = 0, bool bIsFilenames = false) = 0;
	virtual void				    set_allow_client_side_add_string(c_network_string_table* table, bool bAllowClientSideAddString) = 0;
};