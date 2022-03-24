#pragma once
#include <vector>
#include <random>
#include <glm/glm.hpp>

float randomUni() {
	static std::uniform_real_distribution<float> distribution(0.0, 1.0);
	static std::mt19937 generator;
	return distribution(generator);
}

float randomUni(float min, float max) {
	return min + (max - min) * randomUni();
}

int randomUniint(int min, int max) {
	return static_cast<int>(min + (max - min) * randomUni());
}

float randomGas() {
	static std::normal_distribution<float> distribution;
	static std::mt19937 generator;
	return distribution(generator);
}

float randomGas(float min, float max) {
	float scale = (max > min) ? max - min : min - max;
	float sigma = scale / 6.0f;
	float mu = (min + max) * 0.5f;
	return sigma * randomGas() + mu;
}

glm::vec3 randomunitVec3() {
	glm::vec3 tmp(randomUni(-1.0, 1.0), randomUni(-1.0, 1.0), randomUni(-1.0, 1.0));
	return glm::normalize(tmp);
}

glm::vec3 randomColor() {
	return glm::vec3(randomUni(), randomUni(), randomUni());
}

std::vector<float> randomUniList(int size, float min, float max) {
	std::vector<float> result;
	for (int i = 0; i < size; i++)
	{
		result.push_back(randomUni(min, max));
	}
	return result;
}

std::vector<float> randomGasList(int size, float min, float max) {
	std::vector<float> result;
	for (int i = 0; i < size; i++)
	{
		result.push_back(randomGas(min, max));
	}
	return result;
}

std::vector<glm::vec3> randomunitVec3List(int size) {
	std::vector<glm::vec3> result;
	for (int i = 0; i < size; i++)
	{
		result.push_back(randomunitVec3());
	}
	return result;
}

std::vector<glm::vec3> randomcolorList(int size) {
	std::vector<glm::vec3> result;
	for (int i = 0; i < size; i++)
	{
		result.push_back(randomColor());
	}
	return result;
}