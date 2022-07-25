cmake_minimum_required(VERSION 3.0.0)
project(kozuazua VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(kozuazua
            main.cpp 
            Environment.cpp
            Environment.h 
            Agent.cpp 
            Agent.h 
            AgentStructs.h
            HelperFunctions.cpp
            HelperFunctions.h)

add_subdirectory(external/SFML)

target_include_directories(${PROJECT_NAME}
    PUBLIC external/SFML/include/SFML
)

target_link_directories(${PROJECT_NAME}
    PRIVATE external/SFML/src/SFML
)

target_link_libraries(${PROJECT_NAME}
    sfml-graphics
    sfml-system
    sfml-window
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
