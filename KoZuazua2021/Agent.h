// Agent.h

#ifndef AGENT_H
#define AGENT_H

#include "HelperFunctions.h"
#include <cstdlib>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

class Agent
{
    public:
        // Constructor / Destructor
        Agent();
        Agent(agent_vec pos, agent_vec vel);
        virtual ~Agent();

        // initialise

        void initVariables();

        // Setters

        void setPos(agent_vec pos);
        void setVel(agent_vec vel);
        void setDt(float time_interval);

        agent_vec getPos();
        agent_vec getVel();
        float     getRadius();
        float     getTimeInterval();
        int       getInitVelMin();
        int       getInitVelMax();

        void update(std::vector<Agent*>& others, sf::RenderTarget* target);
        void updatePos();
        void updateBoid();
        void updateVel(std::vector<Agent*>& others);
        void getAcc(std::vector<Agent*>& others);
        void updateWindowCollisions(sf::RenderTarget* target);

        void renderAgent(sf::RenderTarget* target);
        

    protected:
        sf::CircleShape boid;
        float radius;
        int pointCount;

        agent_vec pos;
        agent_vec vel;
        agent_vec acc;

        int init_vel_min;
        int init_vel_max;

        float time_interval;

        float Ca;
        float Cr;
        float la;
        float lr;
        float sigma;
        float gamma;
        float alpha;
        float beta;
};

#endif //AGENT_H