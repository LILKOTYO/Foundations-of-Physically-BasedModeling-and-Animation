#pragma once
#include "random.h"
#include <glm/glm.hpp>
#include <Windows.h>
#include <chrono>
#include <iostream>
#include <cmath>
#include <conio.h>


using std::shared_ptr;
using std::make_shared;
using std::sqrt;

const int p_count = 5000;
const int randomcount = 10000;
float pi = 3.14159265f;
glm::vec3 background(0.2, 0.3, 0.3);
float delta_t = 0.1f;
glm::vec3 gravity(0.0, -9.8, 0.0);

std::vector<float> UniformList = randomUniList(randomcount, -1.0, 1.0);
std::vector<float> GaussianList = randomGasList(randomcount, -1.0, 1.0);
std::vector<glm::vec3> ColorList = randomcolorList(randomcount);

