#include "../include/global.h"
#include "../include/pgenerator.h"
#include "../include/render.h"

int main() {
	/*glm::vec3 t(1.0, 1.0, 1.0);
	glm::vec3 a = glm::normalize(t);

	std::cout << a.x << "," << a.y << "," << a.z << std::endl;*/
	std::vector<shared_ptr<particle>> p_list;
	ppointgenerator generator(glm::vec3(0.0, 0, -5.0), glm::vec3(0.0, 1.0, 0.0));
	//pdiscgenerator generator(glm::vec3(0.0, 0.0, -3.0), 2.0, glm::vec3(0.0, 0.1, 0.0));
	render(generator, p_list);
	return 0;
}