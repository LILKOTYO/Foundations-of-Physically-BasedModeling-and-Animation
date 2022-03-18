#pragma once
#include "onb.h"
#include "particle.h"
#include "global.h"


class pgenerator {
public:
	pgenerator() {}
	void check(std::vector<shared_ptr<particle>>& list) {
		for (int i = 0; i < list.size(); i++)
		{
			if (list[i]->checkanddeactive())
				list.erase(list.begin() + i);
		}
	}
public:
	onb uvw;

};

class ppointgenerator : public pgenerator {
public:
	ppointgenerator() {}
	ppointgenerator(glm::vec3 pos, glm::vec3 w) {
		uvw.build(w);
		x0 = pos;
	}

	void generateOmni(std::vector<shared_ptr<particle>>& list, int frequency, int lifetime, float speed);
	void generateDir(std::vector<shared_ptr<particle>>& list, int frequency, int lifetime, float speed, float sigma);

public:
	glm::vec3 x0;
};

void ppointgenerator::generateOmni(std::vector<shared_ptr<particle>>& list, int frequency, int lifetime, float speed) {
	for (int i = 0; i < frequency; i++)
	{
		float theta;
		float h;
		glm::vec3 color;
		int time;
		float v;
		
		theta = UniformList[randomUniint(0, randomcount)] * pi;
		h = UniformList[randomUniint(0, randomcount)];
		v = speed * (UniformList[randomUniint(0, randomcount)] + 1.0);
		color = ColorList[randomUniint(0, randomcount)];
		time = lifetime + static_cast<int>(0.25 * lifetime * GaussianList[randomUniint(0, randomcount)]);
		
		float r = sqrt(1 - pow(h, 2));
		glm::vec3 local_v = v * uvw.world(glm::vec3(r * cos(theta), h, -r * sin(theta)));
		shared_ptr<particle> p = make_shared<particle>(x0, local_v, color, time);
		list.push_back(p);
	}
}

void ppointgenerator::generateDir(std::vector<shared_ptr<particle>>& list, int frequency, int lifetime, float speed, float sigma) {
	/*
	*  emit particle rays
	*  sigema: The upper bound of the offset of the emission direction
	*/
	for (int i = 0; i < frequency; i++)
	{
		float theta;
		float h;
		float phi;
		glm::vec3 color;
		int time;
		float v;

		theta = UniformList[randomUniint(0, randomcount)] * pi;
		h = 0.5 * (UniformList[randomUniint(0, randomcount)] + 1.0);
		v = speed * (UniformList[randomUniint(0, randomcount)] + 1.0);
		color = ColorList[randomUniint(0, randomcount)];
		time = lifetime + static_cast<int>(0.25 * lifetime * GaussianList[randomUniint(0, randomcount)]);

		phi = sqrt(h) * sigma;
		glm::vec3 local_v = v * uvw.world(glm::vec3(sin(theta) * sin(phi), cos(theta) * sin(phi), cos(phi)));
		shared_ptr<particle> p = make_shared<particle>(x0, local_v, color, time);
		list.push_back(p);
	}
}

class pdiscgenerator : public pgenerator {
public:
	pdiscgenerator() {}
	pdiscgenerator(glm::vec3 x, float radius, glm::vec3 n) : c(x), R(radius),normal(n) {
		uvw.build(normal);
	}

	void generate(std::vector<shared_ptr<particle>>& list, int frequency, int lifetime, float speed, float sigma);
public:
	// centor, radius of the disc and the normal.
	glm::vec3 c; 
	float R;
	glm::vec3 normal;
};

void pdiscgenerator::generate(std::vector<shared_ptr<particle>>& list, int frequency, int lifetime, float speed, float sigma) {
	for (int i = 0; i < frequency; i++)
	{
		float theta_disc;
		float f;
		float offset;
		float theta;
		float h;
		float phi;
		glm::vec3 color;
		int time;
		float v;

		theta = UniformList[randomUniint(0, randomcount)] * pi;
		h = 0.5 * (UniformList[randomUniint(0, randomcount)] + 1.0);
		v = speed * (UniformList[randomUniint(0, randomcount)] + 1.0);
		theta_disc = UniformList[randomUniint(0, randomcount)] * pi;
		f = 0.5 * (UniformList[randomUniint(0, randomcount)] + 1.0);
		color = ColorList[randomUniint(0, randomcount)];
		time = lifetime + static_cast<int>(0.25 * lifetime * GaussianList[randomUniint(0, randomcount)]);

		phi = sqrt(h) * sigma;
		offset = sqrt(f) * R;
		glm::vec3 local_v = v * uvw.world(glm::vec3(cos(theta) * sin(phi), sin(theta) * sin(phi), cos(phi)));
		glm::vec3 local_x0 = c + uvw.world(glm::vec3(offset * cos(theta_disc), offset * sin(theta_disc), 0.0));
		shared_ptr<particle> p = make_shared<particle>(local_x0, local_v, color, time);

		list.push_back(p);
	}
}

class pspheregenerator : public pgenerator {
public:
	pspheregenerator() {}
	pspheregenerator(glm::vec3 x, float radius) : c(x), R(radius) {}

	void generate(std::vector<shared_ptr<particle>>& list, int frequency, int lifetime, float speed, float sigma);
public:
	glm::vec3 c;
	float R;
};

void pspheregenerator::generate(std::vector<shared_ptr<particle>>& list, int frequency, int lifetime, float speed, float sigma) {
	for (int i = 0; i < frequency; i++)
	{
		float theta_sphere;
		float y;
		float offset;
		float theta;
		float h;
		float phi;
		glm::vec3 color;
		int time;
		float v;

		theta = UniformList[randomUniint(0, randomcount)] * pi;
		h = 0.5 * (UniformList[randomUniint(0, randomcount)] + 1.0);
		v = speed * (UniformList[randomUniint(0, randomcount)] + 1.0);
		theta_sphere = UniformList[randomUniint(0, randomcount)] * pi;
		y = UniformList[randomUniint(0, randomcount)];
		color = ColorList[randomUniint(0, randomcount)];
		time = lifetime + static_cast<int>(0.25 * lifetime * GaussianList[randomUniint(0, randomcount)]);

		phi = sqrt(h) * sigma;
		float r = sqrt(1 - pow(y, 2));
		glm::vec3 normal = glm::vec3(r * cos(theta_sphere), y, -r * sin(theta_sphere));
		uvw.build(normal);
		glm::vec3 local_v = v * uvw.world(glm::vec3(cos(theta) * sin(phi), sin(theta) * sin(phi), cos(phi)));
		glm::vec3 local_x0 = c + R * normal;
		shared_ptr<particle> p = make_shared<particle>(local_x0, local_v, color, time);

		list.push_back(p);
	}
}