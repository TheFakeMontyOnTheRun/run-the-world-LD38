cmake_minimum_required(VERSION 3.7)
project(run_the_world_LD38)

set(CMAKE_CXX_STANDARD 11)

find_package(SDL REQUIRED)


include_directories(include
        ${SDL_INCLUDE_DIRS} )

set(SOURCE_FILES
        include/CGame.h
        include/CGameSnapshot.cpp
        include/CGameSnapshot.h
        include/Common.h
        include/CRenderer.h
        SDLVersion/CSDLRenderer.cpp
        SDLVersion/main.cpp
        src/CGame.cpp)

find_package(OpenGL REQUIRED)

include (FindPkgConfig)


add_executable(run_the_world_LD38 ${SOURCE_FILES})
target_link_libraries(run_the_world_LD38 ${SDL_LIBRARY} ${OPENGL_LIBRARIES} )
