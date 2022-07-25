// main.cpp

#include <iostream>
#include <ctime>
#include "Environment.h"

int main(){
    
    std::srand( static_cast<unsigned>(time(NULL)) );

    // Initialise all agents
    // At each frame
        // Clear the frame
        // Update each agents' position and velocity
        // Draw the frame


    // Initialise Sim Environment
    Environment sim;
    // std::cout << sim.getAgentSize() << std::endl;

    while (sim.isRunning())
    {
        sim.update();

        sim.render();
        
    }
    return 0;
    
}