// Agent.cpp

#include "Agent.h"

Agent::Agent()
{
    this->initVariables();
}

Agent::Agent(agent_vec pos, agent_vec vel)
{
    this->pos = pos;
    this->vel = vel;
}

Agent::~Agent()
{

}

void Agent::initVariables()
{
    this->radius = 5.f;
    this->pointCount = 3;
    this->boid.setRadius(this->radius);
    // this->boid.setPointCount(this->pointCount);
    this->boid.setFillColor(sf::Color::Cyan);


    this->pos.x = static_cast<float>(rand());
    this->pos.y = static_cast<float>(rand());

    this->vel.x = static_cast<float>(rand());
    this->vel.y = static_cast<float>(rand());

    this->init_vel_min = -1;
    this->init_vel_max = 1;

    this->Ca = 20.f;
    this->Cr = 50.f;
    this->la = 100.f;
    this->lr = 2.f;
    this->sigma = .5f;
    this->gamma = 1.f;
    this->alpha = .07f;
    this->beta = .05f;
}

void Agent::setPos(agent_vec pos)
{
    this->pos = pos;
}

void Agent::setVel(agent_vec vel)
{
    this->vel = vel;
}

void Agent::setDt(float time_interval)
{
    this->time_interval = time_interval;
}

float Agent::getRadius()
{
    return this->radius;
}

float Agent::getTimeInterval()
{
    return this->time_interval;
}

int Agent::getInitVelMin()
{
    return this->init_vel_min;
}

int Agent::getInitVelMax()
{
    return this->init_vel_max;
}

agent_vec Agent::getPos()
{
    return this->pos;
}

agent_vec Agent::getVel()
{
    return this->vel;
}

void Agent::update(std::vector<Agent*>& others, sf::RenderTarget* target)
{
    this->updateVel(others);
    this->updateWindowCollisions(target);
    this->updatePos();
    this->updateBoid();
}

void Agent::updateBoid()
{
    this->boid.setPosition(sf::Vector2f(
        this->pos.x,
        this->pos.y
    ));
}

void Agent::updatePos()
{
    this->pos = this->pos + this->vel;
}

void Agent::updateVel(std::vector<Agent*>& others)
{
    this->getAcc(others);
    this->vel = this->vel + this->acc;
}

void Agent::getAcc(std::vector<Agent*>& others)
{
    int N = others.size();

    agent_vec F0 = {.0f, .0f};
    agent_vec A0 = {.0f, .0f};
    float r = .0f;
    float K = 0.f;
    for ( auto &o : others)
    {
        r = dist2(this->pos, o->pos);
        if (!( r == 0.f ))
        {
        F0 = F0 + ((this->pos-o->pos)/r) * ((this->Ca/this->la) * std::expf(-r / this->la) - (this->Cr/this->lr) * std::expf(-r / this->lr));
        K = this->gamma / std::powf((1 + std::powf(r, 2.f)), this->sigma);
        A0 = A0 + (this->vel - o->vel) * K;
        }
    }

    F0 = F0 / N;
    A0 = A0 / N;

    this->acc = (this->vel * (this->alpha - this->beta * std::powf(norm(this->vel), 2.f))) + F0 + A0;

    // std::cout << "F0 = " << F0.x << ", A0 = " << A0.x << ", acc = " << this->acc.x << ".\n";
}

// void Agent::updateInterAgentCollision(std::vector<Agent*>& others)
// {
//     // Check for inter agent collisions
//     // Reflect both agents' velocities elastically
// }

void Agent::updateWindowCollisions(sf::RenderTarget* target)
{
    // Check for Window Collisions

    // Reflect the agents' velocity elastically
    if ( this->boid.getGlobalBounds().left <= .0f )
    {
        this->vel.x = - this->vel.x;
    } 
    else if ( this->boid.getGlobalBounds().left + this->boid.getGlobalBounds().width >= target->getSize().x)
    {
        this->vel.x = - this->vel.x;
    }
    else if ( this->boid.getGlobalBounds().top <= .0f )
    {
        this->vel.y = - this->vel.y;
    }
    else if ( this->boid.getGlobalBounds().top + this->boid.getGlobalBounds().height >= target->getSize().y)
    {
        this->vel.y = - this->vel.y;
    }
        

}


void Agent::renderAgent(sf::RenderTarget* target)
{
    target->draw(this->boid);
}
