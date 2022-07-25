// HelperFunctions.cpp

#include "HelperFunctions.h"

float dist2(agent_vec vec1, agent_vec vec2)
{
    return std::sqrt(std::pow(vec1.x - vec2.x, 2.f) + std::pow(vec1.y - vec2.y, 2.f));
}

float norm(agent_vec vec)
{
    return std::sqrt(vec.x * vec.x + vec.y * vec.y);
}

float dot(agent_vec vec1, agent_vec vec2)
{
    return vec1.x * vec2.x + vec1.y * vec2.y;
}

agent_vec operator-(agent_vec vec1, agent_vec vec2)
{
    agent_vec new_vec = {vec1.x - vec2.x, vec1.y - vec2.y};
    return new_vec;
}

agent_vec operator+(agent_vec vec1, agent_vec vec2)
{
    agent_vec new_vec = {vec1.x + vec2.x, vec1.y + vec2.y};
    return new_vec;
}

agent_vec operator*(agent_vec vec, float a)
{
    agent_vec new_vec = {vec.x * a, vec.y * a};
    return new_vec;
}

agent_vec operator/(agent_vec vec, float a)
{
    agent_vec new_vec = {vec.x / a, vec.y / a};
    return new_vec;
}

agent_vec operator/(agent_vec vec, int a)
{
    agent_vec new_vec = {vec.x / a, vec.y / a};
    return new_vec;
}