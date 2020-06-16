#pragma once 

class c_hud_texture {
public:
	char short_name[64];    //0x0000
	char texture_file[64];  //0x0040
	bool render_using_font;   //0x0080
	bool precached;         //0x0081
	int8_t character_in_font; //0x0082
	uint8_t pad_0083[1];     //0x0083
	uint32_t font;          //0x0084
	int32_t texture_id;      //0x0088
	float tex_coords[4];    //0x008C
	uint8_t pad_009C[16];    //0x009C
};