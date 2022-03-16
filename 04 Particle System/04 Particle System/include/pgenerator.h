#pragma once
#include "onb.h"
#include "particle.h"
#include "global.h"


class pgenerator {
public:
	pgenerator() {}
	virtual void check(std::vector<shared_ptr<particle>>& list) {
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

	std::vector<shared_ptr<particle>> generateOmni(int frequency, int dur_s, int lifetime);
	std::vector<shared_ptr<particle>> generateDir(int frequency, int dur_s, int lifetime, float sigma);

public:
	glm::vec3 x0;
};

std::vector<shared_ptr<particle>> ppointgenerator::generateOmni(int frequency, int dur_s, int lifetime) {
	int count = frequency * dur_s;
	int step_ms = static_cast<int>(dur_s * 1000 / frequency);
	std::vector<shared_ptr<particle>> result;
	for (int i = 0; i < count; i++)
	{
		std::chrono::steady_clock::time_point start_t = std::chrono::steady_clock::now();
		float theta;
		float h;
		glm::vec3 color;
		int time;
		int time_cost;

		if (i > randomcount - 1)
		{
			theta = UniformList[i - randomcount] * pi;
			h = UniformList[2 * randomcount - 1 - i];
			color = ColorList[i - randomcount];
			time = lifetime + static_cast<int>(0.25 * lifetime * GaussianList[i - randomcount]);
		}
		else
		{
			theta = UniformList[i] * pi;
			h = UniformList[randomcount - 1 - i];
			color = ColorList[i];
			time = lifetime + static_cast<int>(0.25 * lifetime * GaussianList[i]);
		}

		glm::vec3 local_v = uvw.world(glm::vec3(cos(theta), h, -sin(theta)));
		shared_ptr<particle> p = make_shared<particle>(x0, local_v, color, time);
		result.push_back(p);

		std::chrono::steady_clock::time_point end_t = std::chrono::steady_clock::now();
		
		time_cost = std::chrono::duration_cast<std::chrono::milliseconds>(end_t - start_t).count();
		if (time_cost < step_ms)
		{
			Sleep(step_ms - time_cost);
		}
		else
		{
			std::cout << "generate slowly ..." << std::endl;
		}
	}
	return result;
}

std::vector<shared_ptr<particle>> ppointgenerator::generateDir(int frequency, int dur_s, int lifetime, float sigma) {
	/*
	*  emit particle rays
	*  sigema: The upper bound of the offset of the emission direction
	*/
	int count = frequency * dur_s;
	int step_ms = static_cast<int>(dur_s * 1000 / frequency);
	std::vector<shared_ptr<particle>> result;
	for (int i = 0; i < count; i++)
	{
		std::chrono::steady_clock::time_point start_t = std::chrono::steady_clock::now();
		float theta;
		float h;
		float phi;
		glm::vec3 color;
		int time;
		int time_cost;
		if (i > randomcount - 1)
		{
			theta = UniformList[i - randomcount] * pi;
			h = 0.5 * (UniformList[2 * randomcount - 1 - i] + 1.0);
			color = ColorList[i - randomcount];
			time = lifetime + static_cast<int>(0.25 * lifetime * GaussianList[i - randomcount]);
		}
		else
		{
			theta = UniformList[i] * pi;
			h = 0.5 * (1.0 + UniformList[randomcount - 1 - i]);
			color = ColorList[i];
			time = lifetime + static_cast<int>(0.25 * lifetime * GaussianList[i]);
		}

		phi = sqrt(h) * sigma;
		glm::vec3 local_v = uvw.world(glm::vec3(cos(theta) * sin(phi), sin(theta) * sin(phi), cos(phi)));
		shared_ptr<particle> p = make_shared<particle>(x0, local_v, color, time);
		result.push_back(p);

		std::chrono::steady_clock::time_point end_t = std::chrono::steady_clock::now();
		
		time_cost = std::chrono::duration_cast<std::chrono::milliseconds>(end_t - start_t).count();
		if (time_cost < step_ms)
		{
			Sleep(step_ms - time_cost);
		}
		else
		{
			std::cout << "generate slowly ..." << std::endl;
		}
	}
	return result;
}

class pdiscgenerator : public pgenerator {
public:
	pdiscgenerator() {}
	pdiscgenerator(glm::vec3 x, float radius, glm::vec3 n) : c(x), r(radius),normal(n) {
		uvw.build(normal);
	}

	std::vector<shared_ptr<particle>> generate(int frequency, int dur_s, int lifetime, float sigma);
public:
	// centor, radius of the disc and the normal.
	glm::vec3 c; 
	float r;
	glm::vec3 normal;
};

std::vector<shared_ptr<particle>> pdiscgenerator::generate(int frequency, int dur_s, int lifetime, float sigma) {
	int count = frequency * dur_s;
	int step_ms = static_cast<int>(dur_s * 1000 / frequency);
	std::vector<shared_ptr<particle>> result;

	for (int i = 0; i < count; i++)
	{
		std::chrono::steady_clock::time_point start_t = std::chrono::steady_clock::now();
		float theta_disc;
		float f;
		float offset;
		float theta;
		float h;
		float phi;
		glm::vec3 color;
		int time;
		int time_cost;
		if (i > randomcount - 1)
		{
			theta = UniformList[i - randomcount] * pi;
			h = 0.5 * (UniformList[2 * randomcount - 1 - i] + 1.0);
			theta_disc = UniformList[2 * randomcount - 1 - i] * pi;
			f = 0.5 * (UniformList[i - randomcount] + 1.0);
			color = ColorList[i - randomcount];
			time = lifetime + static_cast<int>(0.25 * lifetime * GaussianList[i - randomcount]);
		}
		else
		{
			theta = UniformList[i] * pi;
			h = 0.5 * (1.0 + UniformList[randomcount - 1 - i]);
			theta_disc = UniformList[randomcount - 1 - i] * pi;
			f = 0.5 * (UniformList[i] + 1.0);
			color = ColorList[i];
			time = lifetime + static_cast<int>(0.25 * lifetime * GaussianList[i]);
		}
		phi = sqrt(h) * sigma;
		offset = sqrt(f) * r;
		glm::vec3 local_v = uvw.world(glm::vec3(cos(theta) * sin(phi), sin(theta) * sin(phi), cos(phi)));
		glm::vec3 local_x0 = c + uvw.world(glm::vec3(offset * cos(theta_disc), offset * sin(theta_disc), 0.0));
		shared_ptr<particle> p = make_shared<particle>(local_x0, local_v, color, time);

		result.push_back(p);

		std::chrono::steady_clock::time_point end_t = std::chrono::steady_clock::now();

		time_cost = std::chrono::duration_cast<std::chrono::milliseconds>(end_t - start_t).count();
		if (time_cost < step_ms)
		{
			Sleep(step_ms - time_cost);
		}
		else
		{
			std::cout << "generate slowly ..." << std::endl;
		}
	}
	return result;
}