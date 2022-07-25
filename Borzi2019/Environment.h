// Environment.h

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <iostream>
#include "Agent.h"


class Environment
{
    public:

        // Constructor / Destructor

        Environment();
        virtual ~Environment();

        // Initialisers

        void init_window();
        void init_variables();
        void init_agents();

        // Window Functions

        void pollEvent();
        void update();
        void render();

        void render_boids();
        void update_boid(agent_vec pos);
        void update_agents();

        bool isRunning();
        
        int getAgentSize();

    private:

        sf::Clock clock;
        sf::Time time_interval;
        sf::RenderWindow* window;
        sf::Event ev;

        //
        int num_agents;
        int num_leaders;
        std::vector<Agent> agents;
        std::vector<Agent*> agentptrs;
        Agent agent;

};

#endif // ENVIRONMENT_H