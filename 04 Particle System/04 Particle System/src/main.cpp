#include "../include/global.h"
#include "../include/pgenerator.h"

void render() {

}

int main() {
	/*glm::vec3 t(1.0, 1.0, 1.0);
	glm::vec3 a = glm::normalize(t);

	std::cout << a.x << "," << a.y << "," << a.z << std::endl;*/
	const int p_count = 1000;

	std::vector<shared_ptr<particle>> p_list;
	ppointgenerator generator(glm::vec3(-30.0, 50.0, -50.0), glm::vec3(1.0, 0.0, 0.0));
	bool run = true;

	while (run) {

		std::chrono::steady_clock::time_point start_t = std::chrono::steady_clock::now();

		if (p_list.size() < p_count)
		{
			generator.generateDir(p_list, 3, 10000, 4.0, 0.25 * pi);
		}

		render();

		for (shared_ptr<particle> p : p_list)
		{
			p->update();
		}

		generator.check(p_list);

		std::chrono::steady_clock::time_point end_t = std::chrono::steady_clock::now();
		int time_last = static_cast<int>(delta_t * 1000) - std::chrono::duration_cast<std::chrono::milliseconds>(end_t - start_t).count();

		if (time_last > 0)
		{
			Sleep(time_last);
		}
		else
		{
			std::cout << "simulate slowly..." << std::endl;
		}

		if (_kbhit()) {
			int ch = _getch();
			if (ch == 27) {
				run = false;
				std::cerr << " SIMULATION STOP !" << std::flush;
			}
		}
	}
	return 0;
}