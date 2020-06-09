#pragma once
#include "../../render/render.h"
#include "../../helpers/helpers.h"
#include <vector>

class Dot;
extern std::vector<Dot*> Dots;

class Dot {
public:
	Dot(vec2 p, vec2 v) {
		relative_position = p;
		velocity = v;
	}
	void draw() {
		render::draw_box_filled(relative_position.x - 2, relative_position.y - 2, relative_position.x + 2, relative_position.y + 2, color(255, 255, 255, 255));
		auto t = std::find(Dots.begin(), Dots.end(), this);
		if (t == Dots.end()) return;
		for (auto i = t; i != Dots.end(); i++) {
			if ((*i) == this) continue;
			vec2 Pos = relative_position;
			float Dist = Pos.dist_to((*i)->relative_position);
			if (Dist < 200) {
				vec2 Dir = ((*i)->relative_position - relative_position).normalized();
				render::draw_line(Pos.x, Pos.y, (Pos + Dir * Dist).x, (Pos + Dir * Dist).y, color(255, 255, 255, 255));
			}
		}
	}
	vec2 relative_position = vec2(0, 0);
	vec2 velocity;
};

namespace backdrop {
	void render();
}