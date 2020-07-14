#pragma once 
#include "../../render/render.h"

extern std::map<int, std::string> fixed_weapon_names_by_id;

namespace visuals {
	std::array<float, 65> player_alpha;
	std::array<float, 65> player_alpha_offscreen;
	std::array<float, 65> last_view_time;
	std::array<float, 65> last_view_time_offscreen;

	void render();
	void draw_watermark();
	void spread_circle();
	void draw_capsule_hitbox(c_base_player* player, bool dead);
	void grenade_names(c_base_entity* entity);
	void grenades();
	void thirdperson();
	void impact();
	void spectator_list();
	void night_mode();
	void asus_props();
	void bomb_indicator();
	void damage_indicator();
	void hitmarker();
	void offscreen(c_base_player* player);
	void entity_loop();

	enum act {
		act_none,
		act_throw,
		act_lob,
		act_drop,
	};

	namespace grenade_prediction {
		int act = 0;
		item_definition_index type = item_definition_index::none;
		std::vector<vec3> path;
		std::vector<std::pair<vec3, qangle>> other_collisions;
		color tracer_color = color(105, 158, 244, 255);
		bool firegrenade_didnt_hit = false;

		void tick(int buttons);
		void view();
		void paint();

		void setup(c_base_player* pl, vec3& vecSrc, vec3& vecThrow, const qangle& angeyeangles);
		void simulate(qangle& angles, c_base_player* plocal);
		int  step(vec3& vecSrc, vec3& vecThrow, int tick, float interval);
		bool check_detonate(const vec3& vecThrow, const trace_t& tr, int tick, float interval);
		void trace_hull(vec3& src, vec3& end, trace_t& tr);
		void add_gravity_move(vec3& move, vec3& vel, float frametime, bool onground);
		void push_entity(vec3& src, const vec3& move, trace_t& tr);
		void resolve_fly_collision_custom(trace_t& tr, vec3& vecVelocity, float interval);
		int  physics_clip_velocity(const vec3& in, const vec3& normal, vec3& out, float overbounce);
		void draw_box(vec3 position, color clr, float size);
		float get_radius(item_definition_index type_locl = type);

		enum {
			act_none,
			act_lob,
			act_drop,
			act_throw
		};
	};
}