#include "profile_changer.h"
#include "../../../helpers/proto/pbwrap.h"
#include "../../../settings/settings.h"
#include "../../../helpers/proto/protoparser.h"
#include "../../../sdk/sdk.h"
#include "../../../helpers/proto/protomessages.h"


#define CAST(cast, address, add) (cast)((uint32_t)address + (uint32_t)add)

std::string change_main(void* pubDest, uint32_t* pcubMsgSize) {
	matchmaking_client_hello_t msg((void*)((DWORD)pubDest + 8), *pcubMsgSize - 8);
	matchmaking_client_hello_t::player_commendation_info_t commendations;
	commendations.cmd_friendly().set(settings::changers::profile::friendly);
	commendations.cmd_teaching().set(settings::changers::profile::teacher);
	commendations.cmd_leader().set(settings::changers::profile::leader);
	msg.commendation().set(commendations);

	matchmaking_client_hello_t::player_ranking_info_t ranking;
	ranking.rank_type_id().set(rank_type::mm);
	ranking.account_id().set(sdk::steam_user->get_steam_id().get_account_id());
	ranking.rank_id().set(settings::changers::profile::profile_items[mode_settigns::matchmaking].rank_id);
	ranking.wins().set(settings::changers::profile::profile_items[mode_settigns::matchmaking].wins);
	msg.ranking().set(ranking);

	msg.player_level().set(settings::changers::profile::private_id + 1);
	msg.player_level_xp().set(settings::changers::profile::private_xp);

	if (settings::changers::profile::ban_type != 0 && settings::changers::profile::ban_time != 0) {
		msg.penalty_reason().set(settings::changers::profile::ban_type);
		static int num;
		if (settings::changers::profile::ban_time_type == 0) {
			num = settings::changers::profile::ban_time;
		}
		else if (settings::changers::profile::ban_time_type == 1) {
			num = settings::changers::profile::ban_time * 60;
		}
		else if (settings::changers::profile::ban_time_type == 2) {
			num = settings::changers::profile::ban_time * 3600;
		}
		else if (settings::changers::profile::ban_time_type == 3) {
			num = settings::changers::profile::ban_time * 86400;
		}
		msg.penalty_seconds().set(num);
	}

	return msg.serialize();
}

std::string change_other(void* pubDest, uint32_t* pcubMsgSize) {

	msg_client_rank_update_t msg((void*)((DWORD)pubDest + 8), *pcubMsgSize - 8);

	auto ranking = msg.ranking().get_all();

	for (int i = 0; i < ranking.size(); i++) {
		auto _ranking = ranking[i];

		if (_ranking.rank_type_id().get() == rank_type::wingmans) {
			_ranking.rank_id().set(settings::changers::profile::profile_items[mode_settigns::wingman].rank_id);
			_ranking.wins().set(settings::changers::profile::profile_items[mode_settigns::wingman].wins);

			msg.ranking().set(_ranking, i);
		}

		if (_ranking.rank_type_id().get() == rank_type::danger_zone) {
			_ranking.rank_id().set(settings::changers::profile::profile_items[mode_settigns::dangerzone].rank_id);
			_ranking.wins().set(settings::changers::profile::profile_items[mode_settigns::dangerzone].wins);

			msg.ranking().set(_ranking, i);
		}
	}

	return msg.serialize();
}

namespace profile_changer {
	void write_packet(std::string packet, void* thisPtr, void* oldEBP, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize) {
		if ((uint32_t)packet.size() <= cubDest - 8)
		{
			memcpy((void*)((DWORD)pubDest + 8), (void*)packet.data(), packet.size());
			*pcubMsgSize = packet.size() + 8;
		} else if (sdk::mem_alloc) {
			auto memPtr = *CAST(void**, thisPtr, 0x18);
			auto memPtrSize = *CAST(uint32_t*, thisPtr, 0x1C);
			auto newSize = (memPtrSize - cubDest) + packet.size() + 8;

			auto memory = sdk::mem_alloc->realloc(memPtr, newSize + 4);

			*CAST(void**, thisPtr, 0x18) = memory;
			*CAST(uint32_t*, thisPtr, 0x1C) = newSize;
			*CAST(void**, oldEBP, -0x14) = memory;

			memcpy(CAST(void*, memory, 0x1C), (void*)packet.data(), packet.size());

			*pcubMsgSize = packet.size() + 8;
		}
	}

	void receive_message(void* thisPtr, void* oldEBP, uint32_t messageType, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize) {
		static bool standart_upd;
		static bool other_upd;

		switch (messageType)
		{
		case msg_matchmaking_gc_client_hello:
			write_packet(change_main(pubDest, pcubMsgSize), thisPtr, oldEBP, pubDest, cubDest, pcubMsgSize);
			standart_upd = true;
			break;
		case msg_client_rank_update:
			write_packet(change_other(pubDest, pcubMsgSize), thisPtr, oldEBP, pubDest, cubDest, pcubMsgSize);
			other_upd = true;
			break;
		}

		if (other_upd && standart_upd) {
			other_upd = false;
			standart_upd = false;
			globals::profile_update_request = false;
		}
	}

	void send_update_messages() {
		globals::profile_update_request = true;
		send_client_hello();
		send_client_rank_update();
		send_matchmaking_client_hello();
	}

	bool send_client_hello() {
		msg_client_hello_t msg;
		msg.client_session_need().set(1);
		auto packet = msg.serialize();

		void* ptr = malloc(packet.size() + 8);
		if (!ptr) return false;

		((uint32_t*)ptr)[0] = msg_client_hello | ((DWORD)1 << 31);
		((uint32_t*)ptr)[1] = 0;

		memcpy((void*)((DWORD)ptr + 8), (void*)packet.data(), packet.size());
		bool result = sdk::game_coordinator->gc_send_message(msg_client_hello | ((DWORD)1 << 31), ptr, packet.size() + 8) == game_coordinator_results::gc_result_ok;
		free(ptr);

		return result;
	}

	bool send_matchmaking_client_hello() {
		proto_writer msg(0);
		auto packet = msg.serialize();
		void* ptr = malloc(packet.size() + 8);
		if (!ptr) return false;

		((uint32_t*)ptr)[0] = msg_matchmaking_client_hello | ((DWORD)1 << 31);
		((uint32_t*)ptr)[1] = 0;

		memcpy((void*)((DWORD)ptr + 8), (void*)packet.data(), packet.size());
		bool result = sdk::game_coordinator->gc_send_message(msg_matchmaking_client_hello | ((DWORD)1 << 31), ptr, packet.size() + 8) == game_coordinator_results::gc_result_ok;
		free(ptr);

		return result;
	}

	bool send_client_rank_update() {
		matchmaking_client_hello_t::player_ranking_info_t ranking_wingman;
		matchmaking_client_hello_t::player_ranking_info_t ranking_dangerzone;
		ranking_wingman.rank_type_id().set(rank_type::wingmans);
		ranking_dangerzone.rank_type_id().set(rank_type::danger_zone);

		msg_client_rank_update_t msg;
		msg.ranking().add(ranking_wingman);
		msg.ranking().add(ranking_dangerzone);

		auto packet = msg.serialize();

		void* ptr = malloc(packet.size() + 8);
		if (!ptr) return false;

		((uint32_t*)ptr)[0] = msg_client_rank_update | ((DWORD)1 << 31);
		((uint32_t*)ptr)[1] = 0;

		memcpy((void*)((DWORD)ptr + 8), (void*)packet.data(), packet.size());
		bool result = sdk::game_coordinator->gc_send_message(msg_client_rank_update | ((DWORD)1 << 31), ptr, packet.size() + 8) == game_coordinator_results::gc_result_ok;
		free(ptr);

		return result;
	}
}