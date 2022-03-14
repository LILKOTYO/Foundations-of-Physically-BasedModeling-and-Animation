#pragma once
#include <vector>
#include <iostream>
#include <random>
float randomUni() {
	static std::uniform_real_distribution<float> distribution(0.0, 1.0);
	static std::mt19937 generator;
	return distribution(generator);
}

float randomUni(float min, float max) {
	return min + (max - min) * randomUni();
}

float randomGas() {
	static std::normal_distribution<float> distribution;
}

float randomGas(float min, float max) {
	float scale = (max > min) ? max - min : min - max;
	float sigma = scale / 6.0f;
	float mu = (min + max) * 0.5f;
	return scale * randomGas() + mu;
}

std::vector<float> randomUniList(int size, float min, float max) {
	std::vector<float> result;
	for (int i = 0; i < size; i++)
	{
		result.push_back(randomUni(min, max));
	}
	return result;
}

