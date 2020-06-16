#pragma once 

class c_animation_layer {
public:
	char  pad_0000[20];
	// These should also be present in the padding, don't see the use for it though
	//float	m_flLayerAnimtime;
	//float	m_flLayerFadeOuttime;
	uint32_t order; //0x0014
	uint32_t sequence; //0x0018
	float_t prev_cycle; //0x001C
	float_t weight; //0x0020
	float_t weight_delta_rate; //0x0024
	float_t playback_rate; //0x0028
	float_t cycle; //0x002C
	void* owner; //0x0030 // player's thisptr
	char  pad_0038[4]; //0x0034
}; //Size: 0x0038