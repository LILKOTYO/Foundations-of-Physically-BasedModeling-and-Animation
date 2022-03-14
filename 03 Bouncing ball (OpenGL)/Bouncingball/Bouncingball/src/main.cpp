#include "../include/render.h"
#include <conio.h>

int main() {
    Ball entity(20.0f, 20.0f, { 20.0f, 18.0f });
    Box box({ -50.0f, -50.0f }, { 50.0f,50.0f });
    render(entity, box);
    //while (1) {
    //    if (_kbhit()) {
    //        break;
    //    }
    //    /*simulation(entity, box, d_air, d_wall, bounce);*/
    //    
    //    
    //    if (CheckStop(entity, box))
    //        break;
    //}
    system("pause");
    return 0;
}