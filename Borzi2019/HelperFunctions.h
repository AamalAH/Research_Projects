// HelperFunctions.h

#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include <cmath>
#include <vector>
#include "AgentStructs.h"

float dist2(agent_vec vec1, agent_vec vec2);
float norm(agent_vec vec);
agent_vec operator-(agent_vec vec1, agent_vec vec2);
agent_vec operator+(agent_vec vec1, agent_vec vec2);
agent_vec operator*(agent_vec vec, float x);
agent_vec operator/(agent_vec vec, float x);
agent_vec operator/(agent_vec vec, int a);

#endif // HELPERFUNCTIONS_H