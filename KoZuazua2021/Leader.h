// Leader.h

#ifndef LEADER_H
#define LEADER_H

#include "Agent.h"

class Leader : private Agent
{
    public:

    // Constructors / Destructors
    Leader() : Agent() { this->initLeaderVar(); };
    virtual ~Leader();

    void initLeaderVar();
    void getAcc(std::vector<Agent*>& others);

    private:

};


#endif // LEADER_H