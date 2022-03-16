#pragma once
#include "global.h"

class particle {
public:
	particle(glm::vec3 x0, glm::vec3 v0, glm::vec3 col, int life) :
		pos(x0), v(v0), color(col), lifetime(life) {
		start_t = std::chrono::steady_clock::now();
	}
	
	bool checkanddeactive() {
		std::chrono::steady_clock::time_point current_t = std::chrono::steady_clock::now();
		return (std::chrono::duration_cast<std::chrono::milliseconds>(current_t - start_t).count() >= lifetime) ? true : false;
	}
	
	void update();
	void update_gravity();
	
public:
	glm::vec3 pos;
	glm::vec3 v;
	glm::vec3 color;
	int lifetime; // ms 
	std::chrono::steady_clock::time_point start_t;
};

void particle::update() {
	pos += delta_t * v;
	std::chrono::steady_clock::time_point current_t = std::chrono::steady_clock::now();
	float f = std::chrono::duration_cast<std::chrono::milliseconds>(current_t - start_t).count() / lifetime;
	color = (1 - f) * color + f * background;
	v = (1 - f) * v;
}

void particle::update_gravity() {
	pos += delta_t * v;
	std::chrono::steady_clock::time_point current_t = std::chrono::steady_clock::now();
	float f = std::chrono::duration_cast<std::chrono::milliseconds>(current_t - start_t).count() / lifetime;
	color = (1 - f) * color + f * background;
	v += delta_t * gravity;
}