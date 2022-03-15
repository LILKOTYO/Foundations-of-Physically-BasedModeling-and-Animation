#pragma once
#include "random.h"
#include <glm/glm.hpp>
#include <Windows.h>
#include <chrono>
#include <iostream>
#include <cmath>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

const int randomcount = 10000;
float pi = 3.14159265f;

std::vector<float> UniformList = randomUniList(randomcount, -1.0, 1.0);
std::vector<float> GaussianList = randomGasList(randomcount, -1.0, 1.0);
std::vector<glm::vec3> ColorList = randomcolorList(randomcount);