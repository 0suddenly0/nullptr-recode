#include "../hooks.h"
#include "../../functions/changers/profile changer/profile_changer.h"

namespace hooks {
	game_coordinator_results __stdcall retrieve_message::hook(uint32_t* punMsgType, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize) {
		auto o_retrieve_message = game_coordinator_vhook.get_original<fn>(indexes::retrieve_message);

		const auto result = o_retrieve_message(sdk::game_coordinator, punMsgType, pubDest, cubDest, pcubMsgSize);
		if (result != game_coordinator_results::gc_result_ok)
			return result;

		uint32_t messageType = *punMsgType & 0x7FFFFFFF;

#ifdef _DEBUG
		utils::console_print("[->] message received from GC [%d]\n", messageType);
#endif

		void* thisPtr = nullptr;
		__asm mov thisPtr, ebx;
		auto oldEBP = *(void**)((uint32_t)_AddressOfReturnAddress() - 4);

		if (globals::profile_update_request) {
			profile_changer::receive_message(thisPtr, oldEBP, messageType, pubDest, cubDest, pcubMsgSize);
		}

		return result;
	}
}