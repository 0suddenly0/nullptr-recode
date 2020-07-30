#pragma once

class c_base_player : public c_base_entity {
public:
	static c_base_player* get_spectating_player() {
		if (sdk::local_player->is_alive()) {
			return sdk::local_player;
		} else {
			if (sdk::local_player->observer_mode() == observer_mode_t::obs_roaming) {
				return sdk::local_player;
			} else {
				c_base_player* spectated_ent = sdk::local_player->observer_target();
				if (spectated_ent) return spectated_ent;
			}
		}

		return sdk::local_player;
	}

	static __forceinline c_base_player* get_player_by_user_id(int id) {
		return (c_base_player*)(get_entity_by_index(sdk::engine_client->get_player_for_user_id(id)));
	}
	static __forceinline c_base_player* get_player_by_index(int i) {
		return (c_base_player*)(get_entity_by_index(i));
	}

	NETVAR(observer_mode_t, observer_mode, "DT_BasePlayer", "m_iObserverMode")
	NETVAR(bool, is_defusing, "DT_CSPlayer", "m_bIsDefusing");
	NETVAR(bool, has_defuser_kit, "DT_CSPlayer", "m_bHasDefuser")
	NETVAR(bool, gun_game_immunity, "DT_CSPlayer", "m_bGunGameImmunity");
	NETVAR(int32_t, shots_fired, "DT_CSPlayer", "m_iShotsFired");
	NETVAR(qangle, eye_angles, "DT_CSPlayer", "m_angEyeAngles[0]");
	NETVAR(int, armor_value, "DT_CSPlayer", "m_ArmorValue");
	NETVAR(bool, has_heavy_armor, "DT_CSPlayer", "m_bHasHeavyArmor");
	NETVAR(bool, has_helmet, "DT_CSPlayer", "m_bHasHelmet");
	NETVAR(int, ragdoll, "DT_CSPlayer", "m_hRagdoll");
	NETVAR(bool, is_scoped, "DT_CSPlayer", "m_bIsScoped");;
	NETVAR(float, lower_body_yaw_target, "DT_CSPlayer", "m_flLowerBodyYawTarget");
	NETVAR(int32_t, health, "DT_BasePlayer", "m_iHealth");
	NETVAR(int32_t, life_state, "DT_BasePlayer", "m_lifeState");
	NETVAR(int32_t, m_flags, "DT_BasePlayer", "m_fFlags");
	NETVAR(int32_t, tick_base, "DT_BasePlayer", "m_nTickBase");
	NETVAR(vec3, view_offset, "DT_BasePlayer", "m_vecViewOffset[0]");
	NETVAR(qangle, view_punch_angle, "DT_BasePlayer", "m_viewPunchAngle");
	NETVAR(qangle, aim_punch_angle, "DT_BasePlayer", "m_aimPunchAngle");
	NETVAR(c_handle<c_base_view_model>, view_model, "DT_BasePlayer", "m_hViewModel[0]");
	NETVAR(vec3, velocity, "DT_BasePlayer", "m_vecVelocity[0]");
	NETVAR(float, max_speed, "DT_BasePlayer", "m_flMaxspeed");
	NETVAR(c_handle<c_base_player>, observer_target, "DT_BasePlayer", "m_hObserverTarget");
	NETVAR(float, flash_max_alpha, "DT_CSPlayer", "m_flFlashMaxAlpha");
	NETVAR(float, next_attack, "DT_BaseCombatCharacter", "m_flNextAttack");
	NETVAR(c_handle<c_base_combat_weapon>, active_weapon, "DT_BaseCombatCharacter", "m_hActiveWeapon");
	NETVAR(int32_t, account, "DT_CSPlayer", "m_iAccount");
	NETVAR(float, flash_duration, "DT_CSPlayer", "m_flFlashDuration");
	NETVAR(float, simulation_time, "DT_BaseEntity", "m_flSimulationTime");
	NETVAR(float, cycle, "DT_ServerAnimationData", "m_flCycle");
	NETVAR(int, sequence, "DT_BaseViewModel", "m_nSequence");
	PNETVAR(char, last_place_name, "DT_BasePlayer", "m_szLastPlaceName");
	NETPROP(lower_body_yaw_target_prop, "DT_CSPlayer", "m_flLowerBodyYawTarget");
	NETVAR(int32_t, fov, "DT_BasePlayer", "m_iFOV");
	NETVAR(int32_t, default_fov, "DT_BasePlayer", "m_iDefaultFOV");
	NETVAR(int, survival_team, "DT_CSPlayer", "m_nSurvivalTeam");
	NETVAR(qangle, abs_angles, "DT_BaseEntity", "m_angAbsAngles");
	NETVAR(vec3, abs_origin, "DT_BaseEntity", "m_angAbsOrigin");
	NETVAR(float, duck_speed, "DT_BaseEntity", "m_flDuckSpeed");
	NETVAR(float, duck_amount, "DT_BasePlayer", "m_flDuckAmount");
	PNETVAR(c_handle<c_base_combat_weapon>, my_weapons, "DT_BaseCombatCharacter", "m_hMyWeapons");
	PNETVAR(c_handle<c_base_attributable_item>, my_wearables, "DT_BaseCombatCharacter", "m_hMyWearables");

	bool is_on_ground() {
		return m_flags() & entity_flags::on_ground;
	}

	int get_fov() {
		if (fov() != 0) return fov();
		return default_fov();
	}

	bool in_dangerzone() {
		static auto game_type = sdk::cvar->find_var("game_type");
		return game_type->get_int() == 6;
	}

	bool is_enemy() {
		if (in_dangerzone()) {
			return this->survival_team() != sdk::local_player->survival_team() || sdk::local_player->survival_team() == -1;
		} else {
			return this->team_num() != sdk::local_player->team_num();
		}
	}

	void set_module_index(int index) {
		return call_vfunction<void(__thiscall*)(void*, int)>(this, 75)(this, index);
	}

	void set_abs_angles(vec3 angle) {
		using SetAbsAnglesFn = void(__thiscall*)(void*, const vec3 & angle);
		static SetAbsAnglesFn set_abs_angles;

		if (!set_abs_angles)
			set_abs_angles = (SetAbsAnglesFn)(utils::pattern_scan(GetModuleHandleW(L"client.dll"), "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8"));

		set_abs_angles(this, angle);
	}

	void set_abs_angles(const qangle& angles) {
		using SetAbsAnglesFn = void(__thiscall*)(void*, const qangle & angles);
		static SetAbsAnglesFn set_abs_angles = (SetAbsAnglesFn)utils::pattern_scan(GetModuleHandleW(L"client.dll"), "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8");

		set_abs_angles(this, angles);
	}

	void set_abs_original(vec3 origin) {
		using SetAbsOriginFn = void(__thiscall*)(void*, const vec3 & origin);
		static SetAbsOriginFn SetAbsOrigin;
		if (!SetAbsOrigin)
		{
			SetAbsOrigin = (SetAbsOriginFn)((DWORD)utils::pattern_scan(GetModuleHandleW(L"client.dll"), "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8"));
		}
		SetAbsOrigin(this, origin);
	}

	std::array<float, 24> pose_parameter() const {
		static int _pose_parameter = netvar::get_offset("DT_BaseAnimating", "m_flPoseParameter");
		return *(std::array<float, 24>*)((uintptr_t)this + _pose_parameter);
	}

	c_utl_vector<client_hit_verify_t>& get_client_impacts() {
		return *(c_utl_vector<client_hit_verify_t>*)((uintptr_t)((c_base_player*)this) + 0xBC00);
	}

	c_user_cmd*& current_command() {
		static uint32_t currentCommand = *(uint32_t*)(utils::pattern_scan(GetModuleHandleW(L"client.dll"), "89 BE ? ? ? ? E8 ? ? ? ? 85 FF") + 2);
		return *(c_user_cmd**)((uintptr_t)this + currentCommand);
	}

	void invalidate_bone_cache() {
		static DWORD addr = (DWORD)utils::pattern_scan(GetModuleHandleA("client.dll"), "80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81");

		*(int*)((uintptr_t)this + 0xA30) = sdk::global_vars->framecount; //we'll skip occlusion checks now
		*(int*)((uintptr_t)this + 0xA28) = 0;//clear occlusion flags

		unsigned long g_iModelBoneCounter = **(unsigned long**)(addr + 10);
		*(unsigned int*)((DWORD)this + 0x2924) = 0xFF7FFFFF; // m_flLastBoneSetupTime = -FLT_MAX;
		*(unsigned int*)((DWORD)this + 0x2690) = (g_iModelBoneCounter - 1); // m_iMostRecentModelBoneCounter = g_iModelBoneCounter - 1;
	}

	int get_num_anim_overlays() {
		return *(int*)((DWORD)this + 0x298C);
	}

	vec3& get_abs_angles2() {
		return call_vfunction<vec3 & (__thiscall*)(void*)>(this, 11)(this);
	}

	vec3& get_abs_origin() {
		return call_vfunction<vec3 & (__thiscall*)(void*)>(this, 10)(this);
	}

	void set_angle2(vec3 wantedang) {
		typedef void(__thiscall* SetAngleFn)(void*, const vec3&);
		static SetAngleFn SetAngle = (SetAngleFn)((DWORD)utils::pattern_scan(GetModuleHandleW(L"client.dll"), "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1"));
		SetAngle(this, wantedang);
	}

	c_animation_layer* get_anim_overlays() {
		return *(c_animation_layer**)((DWORD)this + 0x2980);
	}

	c_animation_layer* get_anim_overlay(int i) {
		if (i < 15)
			return &get_anim_overlays()[i];
		return nullptr;
	}

	int get_sequence_activity(int sequence) {
		auto hdr = sdk::mdl_info->get_studiomodel(this->get_model());

		if (!hdr)
			return -1;

		// sig for stuidohdr_t version: 53 56 8B F1 8B DA 85 F6 74 55
		// sig for C_BaseAnimating version: 55 8B EC 83 7D 08 FF 56 8B F1 74 3D
		// c_csplayer vfunc 242, follow calls to find the function.

		static auto get_sequence_activity = (int(__fastcall*)(void*, studiohdr_t*, int))(utils::pattern_scan(GetModuleHandleA("client.dll"), "55 8B EC 53 8B 5D 08 56 8B F1 83"));

		return get_sequence_activity(this, hdr, sequence);
	}

	c_csgo_player_animstate* get_player_anim_state() {
		return *(c_csgo_player_animstate**)((DWORD)this + 0x3900);
	}

	static void update_animation_state(c_csgo_player_animstate* state, qangle angle) {
		static uint8_t* UpdateAnimState = utils::pattern_scan(GetModuleHandleA("client.dll"), "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24");

		if (!UpdateAnimState)
			return;

		__asm {
			push 0
		}

		__asm
		{
			mov ecx, state

			movss xmm1, dword ptr[angle + 4]
			movss xmm2, dword ptr[angle]

			call UpdateAnimState
		}
	}
	static void reset_animation_state(c_csgo_player_animstate* state) {
		static auto reset_anim_state = (void(__thiscall*)(c_csgo_player_animstate*))utils::pattern_scan(GetModuleHandleA("client.dll"), "56 6A 01 68 ? ? ? ? 8B F1");
		if (!reset_anim_state)
			return;

		reset_anim_state(state);
	}
	void create_animation_state(c_csgo_player_animstate* state) {
		static auto CreateAnimState = (void(__thiscall*)(c_csgo_player_animstate*, c_base_player*))utils::pattern_scan(GetModuleHandleA("client.dll"), "55 8B EC 56 8B F1 B9 ? ? ? ? C7 46");
		if (!CreateAnimState)
			return;

		CreateAnimState(state, this);
	}

	float_t& surface_friction() {
		static unsigned int _m_surfaceFriction = utils::find_in_data_map(get_pred_desc_map(), "m_surfaceFriction");
		return *(float_t*)((uintptr_t)this + _m_surfaceFriction);
	}

	vec3& base_velocity() {
		static unsigned int _m_vecBaseVelocity = utils::find_in_data_map(get_pred_desc_map(), "m_vecBaseVelocity");
		return *(vec3*)((uintptr_t)this + _m_vecBaseVelocity);
	}

	vec3 get_eye_pos() {
		auto duckAmount = this->duck_amount();
		return vec_origin() + vec3(0.0f, 0.0f, ((1.0f - duckAmount) * 18.0f) + 46.0f);
	}

	player_info_t get_player_info() {
		player_info_t info;
		sdk::engine_client->get_player_info(ent_index(), &info);
		return info;
	}

	bool is_alive() {
		return life_state() == life_state::alive;
	}

	bool is_flashed(int min_alpha) {
		return flash_duration() > (float)min_alpha;
	}

	bool has_c4() {
		static auto fnHasC4 = (bool(__thiscall*)(void*))(utils::pattern_scan(GetModuleHandleW(L"client.dll"), "56 8B F1 85 F6 74 31"));
		return fnHasC4(this);
	}
	vec3 get_hitbox_pos(int hitbox_id) {
		matrix3x4 boneMatrix[MAXSTUDIOBONES];

		if (setup_bones(boneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0.0f)) {
			auto studio_model = sdk::mdl_info->get_studiomodel(get_model());
			if (studio_model) {
				auto hitbox = studio_model->get_hitbox_set(0)->get_hitbox(hitbox_id);
				if (hitbox) {
					auto
						min = vec3{},
						max = vec3{};

					math::vector_transform(hitbox->bbmin, boneMatrix[hitbox->bone], min);
					math::vector_transform(hitbox->bbmax, boneMatrix[hitbox->bone], max);

					return (min + max) / 2.0f;
				}
			}
		}
		return vec3{};
	}

	bool get_optimized_hitbox_pos(int hitbox, vec3& output) {
		bool can_use_precached = false;
		matrix3x4 precached_matrix[MAXSTUDIOBONES];
		mstudiohitboxset_t* precached_studio_box_set;

		can_use_precached = false;
		const model_t* model = this->get_model();

		if (!model) return false;

		studiohdr_t* studioHdr = sdk::mdl_info->get_studiomodel(model);

		if (!studioHdr) return false;

		if (!this->setup_bones(precached_matrix, MAXSTUDIOBONES, 0x100, 0)) return false;

		precached_studio_box_set = studioHdr->get_hitbox_set(0);

		if (!precached_studio_box_set) return false;

		can_use_precached = true;

		if (!can_use_precached) return false;
		
		if (hitbox >= hitbox_max) return false;

		vec3 min, max;

		mstudiobbox_t* studioBox = precached_studio_box_set->get_hitbox(hitbox);
		if (!studioBox) return false;

		math::vector_transform(studioBox->bbmin, precached_matrix[studioBox->bone], min);
		math::vector_transform(studioBox->bbmax, precached_matrix[studioBox->bone], max);

		output = (min + max) * 0.5f;

		return true;
	}

	mstudiobbox_t* get_hitbox(int hitbox_id) {
		matrix3x4 boneMatrix[MAXSTUDIOBONES];

		if (setup_bones(boneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0.0f)) {
			auto studio_model = sdk::mdl_info->get_studiomodel(get_model());
			if (studio_model) {
				auto hitbox = studio_model->get_hitbox_set(0)->get_hitbox(hitbox_id);
				if (hitbox) {
					return hitbox;
				}
			}
		}
		return nullptr;
	}

	vec3 get_bone_pos(int bone) {
		matrix3x4 boneMatrix[MAXSTUDIOBONES];
		if (setup_bones(boneMatrix, MAXSTUDIOBONES, BONE_USED_BY_ANYTHING, 0.0f)) {
			return boneMatrix[bone].at(3);
		}
		return vec3{};
	}

	bool can_see_player(c_base_player* player, int hitbox) {
		c_game_trace tr;
		ray_t ray;
		c_trace_filter filter;
		filter.pSkip = this;

		auto endpos = player->get_hitbox_pos(hitbox);

		ray.init(get_eye_pos(), endpos);
		sdk::engine_trace->trace_ray(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);

		return tr.hit_entity == player || tr.fraction > 0.97f;
	}

	bool can_see_player(c_base_player* player, const vec3& pos) {
		c_game_trace tr;
		ray_t ray;
		c_trace_filter filter;
		filter.pSkip = this;

		ray.init(get_eye_pos(), pos);
		sdk::engine_trace->trace_ray(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);

		return tr.hit_entity == player || tr.fraction > 0.9f;
	}

	float can_see_player(const vec3& pos) {
		c_game_trace tr;
		ray_t ray;
		c_trace_filter filter;
		filter.pSkip = this;

		ray.init(get_eye_pos(), pos);
		sdk::engine_trace->trace_ray(ray, MASK_SHOT, &filter, &tr);

		return tr.fraction /*> 0.9f*/;
	}

	void update_client_side_animation() {
		return call_vfunction< void(__thiscall*)(void*) >(this, 223)(this);
	}

	float get_max_desync_delta() {
		auto animstate = uintptr_t(this->get_player_anim_state());

		float duckammount = *(float*)(animstate + 0xA4);
		float speedfraction = std::fmax(0, std::fmin(*(float*)(animstate + 0xF8), 1));

		float speedfactor = std::fmax(0, std::fmin(1, *(float*)(animstate + 0xFC)));

		float unk1 = ((*(float*)(animstate + 0x11C) * -0.30000001) - 0.19999999)* speedfraction;
		float unk2 = unk1 + 1.f;
		float unk3;

		if (duckammount > 0) {

			unk2 += ((duckammount * speedfactor) * (0.5f - unk2));

		}

		unk3 = *(float*)(animstate + 0x334) * unk2;

		return unk3;
	}

	int& move_type() {
		return *(int*)((uintptr_t)this + 0x25c);
	}

	qangle* get_vangles() {
		static auto deadflag = netvar::get_offset("DT_BasePlayer", "deadflag");
		return (qangle*)((uintptr_t)this + deadflag + 0x4);
	}

	float_t spawn_time() {
		return *(float_t*)((uintptr_t)this + 0xA360);
	}

	std::string get_name() {
		bool console_safe;
		// Cleans player's name so we don't get new line memes. Use this everywhere you get the players name.
		// Also, if you're going to use the console for its command and use the players name, set console_safe.
		player_info_t pinfo = this->get_player_info();

		char* pl_name = pinfo.szName;
		char buf[128];
		int c = 0;

		for (int i = 0; pl_name[i]; ++i) {
			if (c >= sizeof(buf) - 1)
				break;

			switch (pl_name[i])
			{
			case '"': if (console_safe) break;
			case '\\':
			case ';': if (console_safe) break;
			case '\n':
				break;
			default:
				buf[c++] = pl_name[i];
			}
		}

		buf[c] = '\0';
		return std::string(buf);
	}

	bool is_not_target() {
		if (!this || this == sdk::local_player) return true;
		if (health() <= 0) return true;
		if (gun_game_immunity()) return true;
		if (m_flags() & entity_flags::frozen) return true;

		int _ent_index = ent_index();
		return _ent_index > sdk::global_vars->max_clients;
	}

	c_base_player* get_local_player() {
		return (c_base_player*)sdk::local_player;
	}

};