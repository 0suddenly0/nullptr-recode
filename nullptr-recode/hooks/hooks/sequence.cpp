#include "../hooks.h"

enum e_sequence {
	SEQUENCE_DEFAULT_DRAW = 0,
	SEQUENCE_DEFAULT_IDLE1 = 1,
	SEQUENCE_DEFAULT_IDLE2 = 2,
	SEQUENCE_DEFAULT_LIGHT_MISS1 = 3,
	SEQUENCE_DEFAULT_LIGHT_MISS2 = 4,
	SEQUENCE_DEFAULT_HEAVY_MISS1 = 9,
	SEQUENCE_DEFAULT_HEAVY_HIT1 = 10,
	SEQUENCE_DEFAULT_HEAVY_BACKSTAB = 11,
	SEQUENCE_DEFAULT_LOOKAT01 = 12,

	SEQUENCE_BUTTERFLY_DRAW = 0,
	SEQUENCE_BUTTERFLY_DRAW2 = 1,
	SEQUENCE_BUTTERFLY_LOOKAT01 = 13,
	SEQUENCE_BUTTERFLY_LOOKAT02 = 14,
	SEQUENCE_BUTTERFLY_LOOKAT03 = 15,

	SEQUENCE_FALCHION_IDLE1 = 1,
	SEQUENCE_FALCHION_HEAVY_MISS1 = 8,
	SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP = 9,
	SEQUENCE_FALCHION_LOOKAT01 = 12,
	SEQUENCE_FALCHION_LOOKAT02 = 13,

	SEQUENCE_DAGGERS_IDLE1 = 1,
	SEQUENCE_DAGGERS_LIGHT_MISS1 = 2,
	SEQUENCE_DAGGERS_LIGHT_MISS5 = 6,
	SEQUENCE_DAGGERS_HEAVY_MISS2 = 11,
	SEQUENCE_DAGGERS_HEAVY_MISS1 = 12,

	SEQUENCE_BOWIE_IDLE1 = 1,
};

int get_new_animation(std::string model, const int sequence) {
	if (model == "models/weapons/v_knife_butterfly.mdl") {
		switch (sequence) {
		case SEQUENCE_DEFAULT_DRAW:
			return utils::random(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
		case SEQUENCE_DEFAULT_LOOKAT01:
			return utils::random(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT02);
		default:
			return sequence + 1;
		}
	}
	if (model == "models/weapons/v_knife_falchion_advanced.mdl") {
		switch (sequence) {
		case SEQUENCE_DEFAULT_IDLE2:
			return SEQUENCE_FALCHION_IDLE1;
		case SEQUENCE_DEFAULT_HEAVY_MISS1:
			return utils::random(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP);
		case SEQUENCE_DEFAULT_LOOKAT01:
			return utils::random(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02);
		case SEQUENCE_DEFAULT_DRAW:
		case SEQUENCE_DEFAULT_IDLE1:
			return sequence;
		default:
			return sequence - 1;
		}
	}
	if (model == "models/weapons/v_knife_push.mdl") {
		switch (sequence) {
		case SEQUENCE_DEFAULT_IDLE2:
			return SEQUENCE_DAGGERS_IDLE1;
		case SEQUENCE_DEFAULT_LIGHT_MISS1:
		case SEQUENCE_DEFAULT_LIGHT_MISS2:
			return utils::random(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5);
		case SEQUENCE_DEFAULT_HEAVY_MISS1:
			return utils::random(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1);
		case SEQUENCE_DEFAULT_HEAVY_HIT1:
		case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
		case SEQUENCE_DEFAULT_LOOKAT01:
			return sequence + 3;
		case SEQUENCE_DEFAULT_DRAW:
		case SEQUENCE_DEFAULT_IDLE1:
			return sequence;
		default:
			return sequence + 2;
		}
	}
	if (model == "models/weapons/v_knife_survival_bowie.mdl") {
		switch (sequence) {
		case SEQUENCE_DEFAULT_DRAW:
		case SEQUENCE_DEFAULT_IDLE1:
			return sequence;
		case SEQUENCE_DEFAULT_IDLE2:
			return SEQUENCE_BOWIE_IDLE1;
		default:
			return sequence - 1;
		}
	}
	if (model == "models/weapons/v_knife_ursus.mdl") {
		switch (sequence) {
		case SEQUENCE_DEFAULT_DRAW:
			return utils::random(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
		case SEQUENCE_DEFAULT_LOOKAT01:
			return utils::random(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT02);
		default:
			return sequence + 1;
		}
	}
	if (model == "models/weapons/v_knife_stiletto.mdl") {
		switch (sequence) {
		case SEQUENCE_DEFAULT_LOOKAT01:
			return utils::random(SEQUENCE_DEFAULT_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT01);
		}
	}
	if (model == "models/weapons/v_knife_widowmaker.mdl") {
		switch (sequence) {
		case SEQUENCE_DEFAULT_LOOKAT01:
			return utils::random(SEQUENCE_BUTTERFLY_LOOKAT02, SEQUENCE_BUTTERFLY_LOOKAT03);
		case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
			return sequence - 1;
		}
	}

	if (model == "models/weapons/v_knife_cord.mdl") {
		switch (sequence) {
		case SEQUENCE_DEFAULT_DRAW:
			return utils::random(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
		case SEQUENCE_DEFAULT_LOOKAT01:
			return utils::random(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT02);
		default:
			return sequence + 1;
		}
	}
	if (model == "models/weapons/v_knife_canis.mdl") {
		switch (sequence) {
		case SEQUENCE_DEFAULT_DRAW:
			return utils::random(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
		case SEQUENCE_DEFAULT_LOOKAT01:
			return utils::random(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT02);
		default:
			return sequence + 1;
		}
	}
	if (model == "models/weapons/v_knife_outdoor.mdl") {
		switch (sequence) {
		case SEQUENCE_DEFAULT_DRAW:
			return utils::random(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
		case SEQUENCE_DEFAULT_LOOKAT01:
			return utils::random(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT02);
		default:
			return sequence + 1;
		}
	}
	if (model == "models/weapons/v_knife_skeleton.mdl") {
		switch (sequence) {
		case SEQUENCE_DEFAULT_DRAW:
			return utils::random(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
		case SEQUENCE_DEFAULT_LOOKAT01:
			return utils::random(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT02);
		default:
			return sequence + 1;
		}
	}
	return sequence;
}

void fix_animation(c_recv_proxy_data* data, c_base_view_model* entity) {
	if (!sdk::local_player || !sdk::local_player->is_alive())
		return;

	const auto owner = sdk::entity_list->get_client_entity_from_handle(entity->owner());
	if (owner != sdk::local_player)
		return;

	const auto view_model_weapon = (c_base_attributable_item*)(sdk::entity_list->get_client_entity_from_handle(entity->weapon()));
	if (!view_model_weapon)
		return;

	const auto entry = skin_changer::deeps::weapon_models_info.find(view_model_weapon->item().item_definition_index());

	if (entry == skin_changer::deeps::weapon_models_info.end())
		return;

	if (&entry->second == nullptr)
		return;

	const auto weaponInfo = &entry->second;
	auto& sequence = data->value.m_int;
	sequence = get_new_animation(weaponInfo->model, sequence);
}

namespace hooks {
	void sequence::hook(const c_recv_proxy_data* pData, void* entity, void* output) {
		static auto o_sequence = sequence_vhook->get_original_function();

		const auto proxy_data = (c_recv_proxy_data*)(pData);
		const auto view_model = (c_base_view_model*)(entity);

		fix_animation(proxy_data, view_model);

		o_sequence(pData, entity, output);
	}
}