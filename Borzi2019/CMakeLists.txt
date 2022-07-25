cmake_minimum_required(VERSION 3.22.1)

project(LeaderFlocking)

add_executable(${PROJECT_NAME} 
                main.cpp 
                Environment.cpp
                Environment.h 
                Agent.cpp 
                Agent.h 
                AgentStructs.h
                HelperFunctions.cpp
                HelperFunctions.h)

add_subdirectory(external/sfml)

target_include_directories(${PROJECT_NAME}
    PUBLIC external/sfml/include/SFML
)

target_link_directories(${PROJECT_NAME}
    PRIVATE external/sfml/src/SFML
)

target_link_libraries(${PROJECT_NAME}
    sfml-graphics
    sfml-system
    sfml-window
)