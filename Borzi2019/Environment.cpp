// Environment.cpp

#include "Environment.h"

Environment::Environment()
{
    this->init_variables();
    this->init_window();
    this->init_agents();
}

Environment::~Environment()
{
    delete this->window;
}

void Environment::init_window()
{
    this->window = new sf::RenderWindow(sf::VideoMode(sf::Vector2u(1024, 768)), "Leader Flocking System", sf::Style::Titlebar | sf::Style::Close);
    this->window->setFramerateLimit(60);

}

void Environment::init_variables()
{
    this->window = nullptr;
    this->num_agents = 75;
    this->num_leaders = 1;
    this->time_interval = clock.restart();
    this->agent.setDt(time_interval.asSeconds());
}

void Environment::init_agents()
{
    int c_leader = 0;

    for ( int cA = 0; cA < this->num_leaders + this->num_agents; cA++ )
    {
        int rad = static_cast<int>(this->agent.getRadius());
        int xrange = static_cast<int>(this->window->getSize().x - 2.f * this->agent.getRadius());
        int yrange = static_cast<int>(this->window->getSize().y - 2.f * this->agent.getRadius());
        
        agent_vec pos;
        pos.x = static_cast<float>(rand() % xrange + rad);
        pos.y = static_cast<float>(rand() % yrange + rad);
        this->agent.setPos(pos);

        agent_vec vel;
        int range = agent.getInitVelMax() - agent.getInitVelMin();
        vel.x = static_cast<float>(rand() % range + agent.getInitVelMin());
        vel.y = static_cast<float>(rand() % range + agent.getInitVelMin());
        this->agent.setVel(vel);
        this->agent.updateBoid();

        // The first num_leader entries of the Agents will be leaders

        if ( c_leader < num_leaders ) { this->agent.setLeader(); c_leader++; }
        else { this->agent.setFollower(); }
        // std::cout << this->agent.IsLeader();

        // std::cout << c_leader << std::endl;
        this->agents.push_back(agent);
        // this->agentptrs.push_back(&(this->agents[cA]));
        this->agentptrs.push_back(&agent);
    }

    // std::cout << this->getAgentSize() << std::endl;
}

bool Environment::isRunning()
{
    return this->window->isOpen();
}

void Environment::pollEvent()
{
        while (this->window->pollEvent(this->ev)) 
        {
            switch (this->ev.type)
            {
            case sf::Event::Closed:
                this->window->close();
                break;
            case sf::Event::KeyPressed:
                if (this->ev.key.code == sf::Keyboard::Escape)
                {
                    this->window->close();
                }
                break;
            default:
                break;
            }
        }
}

void Environment::update_agents()
{
for ( auto &a : this->agents )
    {
        // Update position and velocity
        a.update(this->agentptrs, this->window, this->num_leaders);
    }
}

void Environment::update()
{
    this->pollEvent();
    this->update_agents();

}

void Environment::render_boids()
{
    for ( auto &a : this->agents )
    {
        a.renderAgent(this->window);
    }
}

void Environment::render()
{
         //Clear the old frame
    this->window->clear();
    this->render_boids();

    //Draw the new frame

    this->window->display(); // Display the new drawing
}

int Environment::getAgentSize()
{
    return (this->agentptrs).size();
}