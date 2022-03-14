#include <iostream>
#include <vector>
#include <chrono>
#include <Windows.h>
#include "../include/random.h"

void ParticleGenerator() {
	
}

int main() {
	/*std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
	std::cout << "Hello World\n";
	std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
	std::cout << "Printing took "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
		<< "ms.\n";
	Sleep(1000 - std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count());*/
	std::vector<float> UniList = randomUniList(1000, 0.0f, 5.0f);
	for (int i = 0; i < UniList.size(); i++)
	{
		std::cout << UniList[i] << std::endl;
	}
	return 0;
}