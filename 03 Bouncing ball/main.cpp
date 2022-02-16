#include <iostream>
#include <vector>
#include <conio.h>
#include <vec2.hpp>
#include <geometric.hpp>
#define h 0.1f
#define EPSILON 0.0001f
class Ball{
public:
    float m_mass = 1.0f;
    float r = 0;
    glm::vec2 m_pos;
    glm::vec2 m_v;
    glm::vec2 m_a;
    glm::vec2 m_f;

    Ball(float x, float y, glm::vec2 v): m_pos({x,y}), m_v(v), m_a(0.0f){}

    void AddForce()
    {
        //do something
    }
};

class Box{
public:
    glm::vec2 p_Min;
    glm::vec2 p_Max;
    std::vector<glm::vec2> normal;
    float mu = 0.1f;

    Box(glm::vec2 min, glm::vec2 max): p_Min(min), p_Max(max){
        normal.push_back({0.0f, 1.0f});
        normal.push_back({1.0f, 0.0f});
        normal.push_back({0.0f, -1.0f});
        normal.push_back({-1.0f, 0.0f});
    }
};

bool ifCollision(glm::vec2 pos, glm::vec2 pos_before, Box box, int & idx, float & f)
{
    std::vector<int> idx_list;
    std::vector<float> f_list;
    float tmp;
    glm::vec2 min_to_pos = pos - box.p_Min;
    glm::vec2 min_to_posb = pos_before - box.p_Min;
    glm::vec2 max_to_pos = pos - box.p_Max;
    glm::vec2 max_to_posb = pos_before - box.p_Max;
    //Since all normals point inside the box, only the case where the distance is positive is considered, if it is negative, the ball has passed through the box
    if (glm::dot(min_to_pos, box.normal[0]) * glm::dot(min_to_posb, box.normal[0]) < 0.0f)
    {
        idx_list.push_back(1);
        tmp = (pos_before.y - box.p_Min.y) / (pos_before.y - pos.y);
        f_list.push_back(tmp);
    }
    if (glm::dot(min_to_pos, box.normal[1]) * glm::dot(min_to_posb, box.normal[1]) < 0.0f)
    {
        idx_list.push_back(2);
        tmp = (pos_before.x - box.p_Min.x) / (pos_before.x - pos.x);
        f_list.push_back(tmp);
    }
    if (glm::dot(max_to_pos, box.normal[2]) * glm::dot(max_to_posb, box.normal[2]) < 0.0f)
    {
        idx_list.push_back(3);
        tmp = (pos_before.y - box.p_Max.y) / (pos_before.y - pos.y);
        f_list.push_back(tmp);
    }
    if (glm::dot(max_to_pos, box.normal[3]) * glm::dot(max_to_posb, box.normal[3]) < 0.0f)
    {
        idx_list.push_back(4);
        tmp = (pos_before.x - box.p_Max.x) / (pos_before.x - pos.x);
        f_list.push_back(tmp);
    }
    if (idx_list.size() != f_list.size())
    {
        std::cerr << "something wrong!" << std::flush;
        return false;
    }
    if (!idx_list.empty())
    {
        if (idx_list.size() == 1)
        {
            idx = idx_list[0];
            f = f_list[0];
            return true;
        }
        std::cout << "multiple collision" << std::endl;
        int min_f_idx = 0;
        f = std::numeric_limits<float>::max();
        for (int i = 0; i < f_list.size(); ++i) {
            if(f_list[i] < f)
            {
                f = f_list[i];
                min_f_idx = i;
            }
        }
        idx = min_f_idx + 1;
        return true;
    }
    return false;
}

glm::vec2 CollisionResponse(glm::vec2 v, Box box, int idx, float bounce, float d_wall)
{
    glm::vec2 v_n;
    glm::vec2 v_t;
    switch(idx)
    {
        case 1:
            v_n = glm::dot(v, box.normal[0]) * box.normal[0];
            v_t = v - v_n;
            break;
        case 2:
            v_n = glm::dot(v, box.normal[1]) * box.normal[1];
            v_t = v - v_n;
            break;
        case 3:
            v_n = glm::dot(v, box.normal[2]) * box.normal[2];
            v_t = v - v_n;
            break;
        case 4:
            v_n = glm::dot(v, box.normal[3]) * box.normal[3];
            v_t = v - v_n;
            break;
    }
    v_n *= -bounce;
    v_t *= 1 - d_wall;
    return v_n + v_t;
}

void simulation(Ball & ball, Box box, float d_air, float d_wall, float bounce)
{
    float TimeStepRemaining = h;

    while (TimeStepRemaining > EPSILON)
    {
        int idx = 0;
        // gravity
        ball.m_f = {0.0f, -9.8f * ball.m_mass};
        //wind
        glm::vec2 F_wind(0.0f, 0.0f);
        ball.m_f += F_wind;
        //air resistance
        ball.m_f += - d_air * ball.m_v;
        //calculate a
        ball.m_a = ball.m_f / ball.m_mass;

        float TimeStep = TimeStepRemaining;
        glm::vec2 speed = ball.m_v + TimeStepRemaining * ball.m_a;
        glm::vec2 pos = ball.m_pos + TimeStepRemaining * ball.m_v;
        float f = 0.0f;
        if (ifCollision(pos, ball.m_pos, box, idx, f))
        {
            //std::cout << idx << std::endl;
//            switch(idx)
//            {
//                case 1:
//                    f = (ball.m_pos.y - box.p_Min.y) / (ball.m_pos.y - pos.y);
//                    break;
//                case 2:
//                    f = (ball.m_pos.x - box.p_Min.x) / (ball.m_pos.x - pos.x);
//                    break;
//                case 3:
//                    f = (ball.m_pos.y - box.p_Max.y) / (ball.m_pos.y - pos.y);
//                    break;
//                case 4:
//                    f = (ball.m_pos.x - box.p_Max.x) / (ball.m_pos.x - pos.x);
//                    break;
//                default:
//                    std::cout << "something wrong!" << std::endl;
//            }
            TimeStep *= f;
            //std::cout << "!!!" << pos.x << "," << pos.y << std::endl;
            speed = ball.m_v + TimeStep * ball.m_a;
            pos = ball.m_pos + TimeStep * ball.m_v;
            //std::cout << "???" << pos.x << "," << pos.y << std::endl;
            speed = CollisionResponse(speed, box, idx, bounce, d_wall);
        }
        TimeStepRemaining -= TimeStep;
        //std::cout << pos.x << "," << pos.y << std::endl;
        ball.m_v = speed;
        ball.m_pos = pos;
    }
}

bool CheckStop(Ball ball, Box box)
{
    //Check if the speed is small enough
    if (glm::length(ball.m_v) > EPSILON)
        return false;

    //Check for contact with a plane
    glm::vec2 min_to_ball = ball.m_pos - box.p_Min;
    glm::vec2 max_to_ball = ball.m_pos - box.p_Max;
    float min_dis = std::numeric_limits<float>::max();
    int min_idx = 0;
    for (int i = 0; i < 4; ++i) {
        float tmp = 0.0f;
        if (i < 3)
        {
            tmp = glm::dot(min_to_ball, box.normal[i]);
            if (tmp > 0)            //Since all normals point inside the box, only the case where the distance is positive is considered, if it is negative, the ball has passed through the box
                tmp -= ball.r;
            else
            {
                std::cerr << "something wrong!" << std::flush;
                return false;
            }
            if (tmp < min_dis)
            {
                min_dis = tmp;
                min_idx = i;
            }
        }
        else
        {
            tmp = glm::dot(max_to_ball, box.normal[i]);
            if (tmp > 0)
                tmp -= ball.r;
            else
            {
                std::cerr << "something wrong!" << std::flush;
                return false;
            }
            if (tmp < min_dis)
            {
                min_dis = tmp;
                min_idx = i;
            }
        }
    }
    if (min_dis > EPSILON)
        return false;

    //Detect if the force is pointing to the nearest plane
    glm::vec2 F_n;
    glm::vec2 F_t;
    if (glm::dot(ball.m_f, box.normal[min_idx]) >= 0.0f)
        return false;

    //Check if the tangential force is less than the static friction force
    F_n = glm::dot(ball.m_f, box.normal[min_idx]) * box.normal[min_idx];
    F_t = ball.m_f - F_n;
    if (glm::length(F_t) >= box.mu * glm::length((F_n)))
        return false;
    std::cout << "stop" << std::endl;
    return true;
}

int main() {
    Ball entity(20.0f, 20.0f, {3.0f, 5.0f});
    Box box({-50.0f, -50.0f},{50.0f,50.0f});
    int T_max = 100;
    float d_air = 0.1f;
    float d_wall = 0.1f;
    float bounce = 0.6f;
    //std::cin >> T_max;

    while(1){
        if (_kbhit()){
            break;
        }
        simulation(entity, box, d_air, d_wall, bounce);
        std::cout << entity.m_pos.x << " , " <<entity.m_pos.y << std::endl;
        if (CheckStop(entity, box))
            break;
    }
    system("pause");
    return 0;
}
