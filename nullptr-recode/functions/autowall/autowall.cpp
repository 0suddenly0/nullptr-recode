#include "autowall.h"
#include "../../helpers/helpers.h"

#define DAMAGE_NO		0
#define DAMAGE_EVENTS_ONLY	1
#define DAMAGE_YES		2
#define DAMAGE_AIM		3
#define CHAR_TEX_ANTLION		'A'
#define CHAR_TEX_BLOODYFLESH	'B'
#define	CHAR_TEX_CONCRETE		'C'
#define CHAR_TEX_DIRT			'D'
#define CHAR_TEX_EGGSHELL		'E'
#define CHAR_TEX_FLESH			'F'
#define CHAR_TEX_GRATE			'G'
#define CHAR_TEX_ALIENFLESH		'H'
#define CHAR_TEX_CLIP			'I'
#define CHAR_TEX_PLASTIC		'L'
#define CHAR_TEX_METAL			'M'
#define CHAR_TEX_SAND			'N'
#define CHAR_TEX_FOLIAGE		'O'
#define CHAR_TEX_COMPUTER		'P'
#define CHAR_TEX_SLOSH			'S'
#define CHAR_TEX_TILE			'T'
#define CHAR_TEX_CARDBOARD		'U'
#define CHAR_TEX_VENT			'V'
#define CHAR_TEX_WOOD			'W'
#define CHAR_TEX_GLASS			'Y'
#define CHAR_TEX_WARPSHIELD		'Z'

namespace autowall {
	return_info_t get(vec3 start, vec3 end, c_base_player* from_ent, c_base_player* to_ent, int hitgroup) {
		return_info_t rt;

		fbdata_t bullet;
		bullet.start = start;
		bullet.end = end;
		bullet.pos = start;
		bullet.thickness = 0.f;
		bullet.walls = 4;

		math::angle_vector(math::calc_angle(start, end), bullet.dir);

		c_trace_filter_skipentity flt_player = c_trace_filter_skipentity(to_ent);
		c_trace_filter flt_self = c_trace_filter();
		flt_self.pSkip = from_ent;

		if (to_ent) bullet.filter = &flt_player;
		else bullet.filter = &flt_self;

		c_base_combat_weapon* wep = from_ent->active_weapon().get();
		if (!wep) return rt;

		c_cs_weapon_info* inf = wep->get_cs_weapon_data();
		if (!inf) return rt;

		end = start + bullet.dir * (wep->is_knife() ? 45.f : inf->range);
		bullet.damage = inf->damage;

		while (bullet.damage > 0 && bullet.walls > 0) {
			rt.walls = bullet.walls;

			ray_t ray;
			ray.init(bullet.pos, end);

			c_trace_filter filter;
			filter.pSkip = from_ent;

			sdk::engine_trace->trace_ray(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &bullet.trace);
			clip_trace_to_player(bullet.pos, bullet.pos + bullet.dir * 40.f, to_ent, MASK_SHOT | CONTENTS_GRATE, bullet.filter, &bullet.trace);

			bullet.damage *= powf(inf->range_modifier, (bullet.trace.endpos - start).length() / 500.f);

			if (bullet.trace.fraction == 1.f) {
				if (to_ent && hitgroup != -1) {
					scale_damage(to_ent, inf, hitgroup, bullet.damage);

					rt.damage = bullet.damage;
					rt.hitgroup = hitgroup;
					rt.end = bullet.trace.endpos;
					rt.ent = to_ent;
				}
				else {
					rt.damage = bullet.damage;
					rt.hitgroup = -1;
					rt.end = bullet.trace.endpos;
					rt.ent = nullptr;
				}
			}

			if (bullet.trace.hitgroup > 0 && bullet.trace.hitgroup <= 7) {
				if (to_ent && bullet.trace.hit_entity != to_ent) {
					rt.damage = -1;

					return rt;
				}
				c_base_player* player = (c_base_player*)bullet.trace.hit_entity;
				if (hitgroup != -1)
					scale_damage(player, inf, hitgroup, bullet.damage);
				else
					scale_damage(player, inf, bullet.trace.hitgroup, bullet.damage);

				rt.damage = bullet.damage;
				rt.hitgroup = bullet.trace.hitgroup;
				rt.end = bullet.trace.endpos;
				rt.ent = bullet.trace.hit_entity;

				break;
			}

			if (!handle_bullet_penetration(inf, bullet))
				break;

			rt.did_penetrate_wall = true;
		}

		rt.walls = bullet.walls;
		return rt;
	}

	void clip_trace_to_player(vec3& start, vec3& end, c_base_player* ent, unsigned int mask, c_base_trace_filter* filter, trace_t* trace) {
		if (!ent)
			return;

		vec3 mins = ent->get_collideable()->obb_mins();
		vec3 maxs = ent->get_collideable()->obb_maxs();

		vec3 dir(end - start);
		dir.normalized();

		vec3 center = (maxs + mins) / 2, pos(center + ent->vec_origin());

		vec3 to = pos - start;
		float range_along = dir.dot(to);

		float range;
		if (range_along < 0.f)
			range = -to.length();
		else if (range_along > dir.length())
			range = -(pos - end).length();
		else {
			vec3 ray(pos - (dir * range_along + start));
			range = ray.length();
		}

		if (range <= 60.f) {
			trace_t newtrace;

			ray_t ray;
			ray.init(start, end);

			sdk::engine_trace->clip_ray_to_entity(ray, mask, ent, &newtrace);

			if (trace->fraction > newtrace.fraction)
				*trace = newtrace;
		}
	}

	void scale_damage(c_base_player* e, c_cs_weapon_info* weapon_info, int& hitgroup, float& current_damage) {
		bool has_heavy_armor = false;
		int armor_value = e->armor_value();

		auto is_armored = [&e, &hitgroup]()->bool {
			c_base_player* target_entity = e;
			switch (hitgroup) {
			case HITGROUP_HEAD:
				return target_entity->has_helmet();
			case HITGROUP_GENERIC:
			case HITGROUP_CHEST:
			case HITGROUP_STOMACH:
			case HITGROUP_LEFTARM:
			case HITGROUP_RIGHTARM:
				return true;
			default:
				return false;
			}
		};

		switch (hitgroup) {
		case HITGROUP_HEAD:
			current_damage *= has_heavy_armor ? 2.f : 4.f;
			break;
		case HITGROUP_STOMACH:
			current_damage *= 1.25f;
			break;
		case HITGROUP_LEFTLEG:
		case HITGROUP_RIGHTLEG:
			current_damage *= 0.75f;
			break;
		default:
			break;
		}

		if (armor_value > 0 && is_armored()) {
			float bonus_value = 1.f, armor_bonus_ratio = 0.5f, armor_ratio = weapon_info->armor_ratio / 2.f;

			if (has_heavy_armor) {
				armor_bonus_ratio = 0.33f;
				armor_ratio *= 0.5f;
				bonus_value = 0.33f;
			}

			float new_damage = current_damage * armor_ratio;

			if (has_heavy_armor)
				new_damage *= 0.85f;

			if (((current_damage - (current_damage * armor_ratio)) * (bonus_value * armor_bonus_ratio)) > armor_value)
				new_damage = current_damage - (armor_value / armor_bonus_ratio);

			current_damage = new_damage;
		}
	}

	bool handle_bullet_penetration(c_cs_weapon_info* inf, fbdata_t& bullet) {
		trace_t trace;

		static convar* dbp = sdk::cvar->find_var("ff_damage_bullet_penetration");

		surfacedata_t* sdata = sdk::phys_surface->get_surface_data(bullet.trace.surface.surfaceProps);
		unsigned short mat = sdata->game.material;

		float sp_mod = *(float*)((uintptr_t)sdata + 88);
		float dmg_mod = 0.16f;
		float pnt_mod = 0.f;

		int solid_surf = (bullet.trace.contents >> 3) & CONTENTS_SOLID;
		int light_surf = (bullet.trace.surface.flags >> 7) & SURF_LIGHT;

		if (bullet.walls <= 0
			|| !bullet.walls && !light_surf && !solid_surf && mat != CHAR_TEX_GLASS && mat != CHAR_TEX_GRATE
			|| inf->penetration <= 0.f
			|| !trace_to_exit(&bullet.trace, bullet.trace.endpos, bullet.dir, &trace)
			&& !sdk::engine_trace->get_point_contents(bullet.trace.endpos, MASK_SHOT_HULL | CONTENTS_HITBOX, NULL) & (MASK_SHOT_HULL | CONTENTS_HITBOX))
			return false;

		surfacedata_t* e_sdata = sdk::phys_surface->get_surface_data(trace.surface.surfaceProps);
		unsigned short e_mat = e_sdata->game.material;
		float e_sp_mod = *(float*)((DWORD)e_sdata + 88);

		if (mat == CHAR_TEX_GRATE || mat == CHAR_TEX_GLASS) {
			pnt_mod = 3.f;
			dmg_mod = 0.05f;
		} else if (light_surf || solid_surf) {
			pnt_mod = 1.f;
			dmg_mod = 0.16f;
		} else if (mat == CHAR_TEX_FLESH) {
			pnt_mod = dbp->get_float();
			dmg_mod = 0.16f;
		} else {
			pnt_mod = (sp_mod + e_sp_mod) / 2.f;
			dmg_mod = 0.16f;
		}

		if (mat == e_mat) {
			if (e_mat == CHAR_TEX_CARDBOARD || e_mat == CHAR_TEX_WOOD) pnt_mod = 3.f;
			else if (e_mat == CHAR_TEX_PLASTIC) pnt_mod = 2.f;
		}

		float thickness = (trace.endpos - bullet.trace.endpos).length_sqr();
		float modifier = fmaxf(0.f, 1.f / pnt_mod);

		float lost_damage = fmaxf(
			((modifier * thickness) / 24.f)
			+ ((bullet.damage * dmg_mod)
				+ (fmaxf(3.75 / inf->penetration, 0.f) * 3.f * modifier)), 0.f);

		if (lost_damage > bullet.damage) return false;
		if (lost_damage > 0.f) bullet.damage -= lost_damage;
		if (bullet.damage < 1.f) return false;

		bullet.pos = trace.endpos;
		bullet.walls--;

		return true;
	}

	bool trace_to_exit(trace_t* enter_trace, vec3& start, vec3& dir, trace_t* exit_trace) {
		vec3 end = vec3();
		float distance = 0.f;
		int distance_check = 23;
		int first_contents = 0;

		do {
			distance += 4.f;
			end = start + dir * distance;

			if (!first_contents)
				first_contents = sdk::engine_trace->get_point_contents(end, MASK_SHOT | CONTENTS_GRATE, NULL);

			int point_contents = sdk::engine_trace->get_point_contents(end, MASK_SHOT | CONTENTS_GRATE, NULL);

			if (!(point_contents & (MASK_SHOT_HULL | CONTENTS_HITBOX)) || point_contents & CONTENTS_HITBOX && point_contents != first_contents) {
				vec3 new_end = end - (dir * 4.f);

				ray_t ray;
				ray.init(end, new_end);

				sdk::engine_trace->trace_ray(ray, MASK_SHOT | CONTENTS_GRATE, nullptr, exit_trace);

				if (exit_trace->startsolid && exit_trace->surface.flags & SURF_HITBOX) {
					ray_t ray1;
					ray1.init(start, end);

					c_trace_filter filter;
					filter.pSkip = exit_trace->hit_entity;

					sdk::engine_trace->trace_ray(ray, MASK_SHOT | CONTENTS_GRATE, &filter, exit_trace);

					if (exit_trace->did_hit() && !exit_trace->startsolid) return true;
					continue;
				}
				c_base_player* player = (c_base_player*)enter_trace->hit_entity;
				if (exit_trace->did_hit() && !exit_trace->startsolid) {
					if (enter_trace->surface.flags & SURF_NODRAW || !(exit_trace->surface.flags & SURF_NODRAW)) {
						if (exit_trace->plane.normal.dot(dir) <= 1.f) return true;
						continue;
					}

					if (is_breakable(player) && is_breakable(player)) return true;

					continue;
				}

				if (exit_trace->surface.flags & SURF_NODRAW) {
					if (is_breakable(player) && is_breakable(player)) return true;
					else if (!(enter_trace->surface.flags & SURF_NODRAW)) continue;
				}

				if ((!enter_trace->hit_entity || enter_trace->hit_entity->ent_index() == 0) && is_breakable(player)) {
					exit_trace = enter_trace;
					exit_trace->endpos = start + dir;
					return true;
				}

				continue;
			}

			distance_check--;
		} while (distance_check);

		return false;
	}

	bool is_breakable(c_base_player* e) {
		static auto is_breakable_fn = (bool(__fastcall*)(c_base_player*))(utils::pattern_scan(GetModuleHandleW(L"client.dll"), "55 8B EC 51 56 8B F1 85 F6 74 68 83 BE"));

		if (!e || !e->ent_index()) return false;

		char* take_damage{ (char*)((uintptr_t)e + *(size_t*)((uintptr_t)is_breakable_fn + 38)) };
		char take_damage_backup{ *take_damage };

		client_class* cclass = sdk::chl_client->get_all_classes();

		if ((cclass->network_name[1]) != 'F'
			|| (cclass->network_name[4]) != 'c'
			|| (cclass->network_name[5]) != 'B'
			|| (cclass->network_name[9]) != 'h')
			*take_damage = 2;

		bool breakable = is_breakable_fn(e);
		*take_damage = take_damage_backup;

		return breakable;
	}

	float get_estimated_point_damage(vec3 point) {
		if (!sdk::local_player) return -1.f;

		fbdata_t bullet;
		c_trace_filter filter = c_trace_filter();
		filter.pSkip = sdk::local_player;

		bullet.filter = &filter;
		bullet.start = sdk::local_player->get_eye_pos();
		bullet.end = point;
		bullet.pos = sdk::local_player->get_eye_pos();
		math::angle_vector(math::calc_angle(bullet.start, point), bullet.dir);
		bullet.trace.startpos = bullet.start;
		bullet.trace.endpos = point;

		c_base_combat_weapon* wep = sdk::local_player->active_weapon().get();
		if (!wep) return -2.f;

		bullet.walls = 1;
		bullet.thickness = 0.f;

		c_cs_weapon_info* inf = wep->get_cs_weapon_data();
		if (!inf) return -3.f;

		bullet.damage = inf->damage;

		ray_t ray;
		ray.init(bullet.start, bullet.end);

		sdk::engine_trace->trace_ray(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &bullet.trace);

		if (bullet.trace.fraction == 1.f) return -4.f;
		if (handle_bullet_penetration(inf, bullet)) return bullet.damage;

		return -5.f;
	}
}