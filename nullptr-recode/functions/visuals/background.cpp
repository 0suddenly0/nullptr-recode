#include "background.h"
#include "../../settings/settings.h"

std::vector<Dot*> Dots;

void backdrop::render() {
	if (!globals::show_menu) return;

	static int width = utils::get_screen_size().x;
	static int height = utils::get_screen_size().y;

	int s = rand() % 130;

	if (s == 0)      Dots.push_back(new Dot(vec2(rand() % (int)width, -16),              vec2((rand() % 7) - 3, rand() % 3 + 1)));
	else if (s == 1) Dots.push_back(new Dot(vec2(rand() % (int)width, (int)height + 16), vec2((rand() % 7) - 3, -1 * (rand() % 3 + 1))));
	else if (s == 2) Dots.push_back(new Dot(vec2(-16, rand() % (int)height),             vec2(rand() % 3 + 1, (rand() % 7) - 3)));
	else if (s == 3) Dots.push_back(new Dot(vec2((int)width + 16, rand() % (int)height), vec2(-1 * (rand() % 3 + 1), (rand() % 7) - 3)));

	for (auto i = Dots.begin(); i < Dots.end();) {
		if ((*i)->relative_position.y < -20 || (*i)->relative_position.y > height + 20 || (*i)->relative_position.x < -20 || (*i)->relative_position.x > width + 20) {
			delete (*i);
			i = Dots.erase(i);
		} else {
			(*i)->relative_position = (*i)->relative_position + (*i)->velocity * (0.3f);
			i++;
		}
	}
	for (auto i = Dots.begin(); i < Dots.end(); i++) (*i)->draw();
}