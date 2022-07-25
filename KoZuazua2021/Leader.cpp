// Leader.cpp

#include "Leader.h"

void Leader::getAcc(std::vector<Agent*>& others)
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

}
