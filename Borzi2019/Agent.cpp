// Agent.cpp

#include "Agent.h"

Agent::Agent()
{
    this->initVariables();
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
    // this->boid.setFillColor(sf::Color::Cyan);

    this->pos.x = static_cast<float>(rand());
    this->pos.y = static_cast<float>(rand());

    this->vel.x = static_cast<float>(rand());
    this->vel.y = static_cast<float>(rand());

    this->init_vel_min = -1;
    this->init_vel_max = 1;

    this->is_leader = false;

    this->Ca0 = 10.f;
    this->Cr0 = 25.f;
    this->lr0 = 1.f;
    this->la0 = 50;
    this->gamma_1 = 10.f;

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

void Agent::setLeader()
{
    this->is_leader = true;
    this->boid.setFillColor(sf::Color::Red);
}

void Agent::setFollower()
{
    this->is_leader = false;
    this->boid.setFillColor(sf::Color::Cyan);
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

bool Agent::IsLeader()
{
    return this->is_leader;
}

void Agent::update(std::vector<Agent*>& others, sf::RenderTarget* target, int num_leaders)
{
    this->updateVel(others, num_leaders);
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

void Agent::updateVel(std::vector<Agent*>& others, int num_leaders)
{
    this->getAcc(others, num_leaders);
    this->vel = this->vel + this->acc;
}

void Agent::getAcc(std::vector<Agent*>& others, int num_leaders)
{
    float N = 0.;
    if ( this->is_leader )
        { N = others.size() - num_leaders; }
    else
        { N = others.size(); }

    agent_vec FB = {.0f, .0f};
    agent_vec F0 = {.0f, .0f};
    // std::cout << F0.x << std::endl;
    agent_vec AB = {.0f, .0f};
    float r = .0f;
    float K = 0.f;

    // We want a leader to ignore all the other leaders. Therefore, for a leader we start from the first agent who is not a leader
    for ( auto &o : others)
    {   
        // If this is a leader, ignore all other leaders
        if ( this->is_leader && o->is_leader ) { continue; }
        r = dist2(this->pos, o->pos);
        if ( !(this == o) )
        {
            K = this->gamma / std::powf((1 + std::powf(r, 2.f)), this->sigma);
            AB = AB + (this->vel - o->vel) * K;
            // If other is a leader then use the leader parameters
            if ( o->is_leader )
            {
                F0 = F0 + ((this->pos-o->pos)/r) * ((this->Ca0/this->la0) * std::expf(-r / this->la0) - (this->Cr0/this->lr0) * std::expf(-r / this->lr0)) * this->gamma_1;
            }
            else
            {
                FB = FB + ((this->pos-o->pos)/r) * ((this->Ca/this->la) * std::expf(-r / this->la) - (this->Cr/this->lr) * std::expf(-r / this->lr));
            }
        }
    }

    FB = (FB * -1.f) / N;
    AB = AB / N;
    if ( !(num_leaders == 0) ) 
        F0 = (F0 * -1.f) / static_cast<float>(num_leaders);

    // std::cout << F0.x << std::endl;

    this->acc = (this->vel * (this->alpha - this->beta * std::powf(norm(this->vel), 2.f))) + FB + F0 + AB;

    // std::cout << "F0 = " << F0.x << "FB = " << FB.x << "AB = " << AB.x << ", acc = " << this->acc.x << ".\n";
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
