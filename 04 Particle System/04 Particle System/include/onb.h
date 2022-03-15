#pragma once
#include "global.h"

class onb {
public:
	onb() {}

	glm::vec3 x() { return u; }
	glm::vec3 y() { return v; }
	glm::vec3 z() { return w; }

	glm::vec3 world(glm::vec3 p) {
		return p.x * u + p.y * v + p.z * w;
	}

	glm::vec3 world(float s, float t, float p) {
		return s * u + t * v + p * w;
	}

	void build(const glm::vec3& axis) {
		w = glm::normalize(axis);
		glm::vec3 a = (axis.x > 0.9) ? glm::vec3(0.0, 1.0, 0.0) : glm::vec3(1.0, 0.0, 0.0);
		u = glm::normalize(glm::cross(a, w));
		v = glm::cross(w, u);
	}
public:
	glm::vec3 u;
	glm::vec3 v;
	glm::vec3 w;
};