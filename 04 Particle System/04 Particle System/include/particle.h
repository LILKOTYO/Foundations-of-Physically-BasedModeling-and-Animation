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
		return (std::chrono::duration_cast<std::chrono::milliseconds>(start_t - current_t).count() >= lifetime) ? true : false;
	}
	
	
public:
	glm::vec3 pos;
	glm::vec3 v;
	glm::vec3 color;
	int lifetime; // ms 
	std::chrono::steady_clock::time_point start_t;
};