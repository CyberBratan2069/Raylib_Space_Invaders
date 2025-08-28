cmake_minimum_required(VERSION 3.16)   # 4.0 doesn’t exist; latest is 3.x
project(SpaceInvaders C)

set(CMAKE_C_STANDARD 11)

add_executable(SpaceInvaders
        src/main.c
        src/spaceship.h
        src/spaceship.c
        src/game.c
        src/game.h
        src/laser.h
        src/laser.c
        src/laser_vec.h
        src/obstacle.c
        src/obstacle.h
        src/alien.h
        src/alien.c
        src/mysteryship.h
        src/mysteryship.c
)

find_package(raylib 4.0 REQUIRED)

target_link_libraries(SpaceInvaders raylib)

if(APPLE)
    target_link_libraries(SpaceInvaders
            "-framework OpenGL"
            "-framework Cocoa"
            "-framework IOKit"
            "-framework CoreVideo"
    )
endif()


if(UNIX AND NOT APPLE)
    target_link_libraries(SpaceInvaders m pthread dl GL rt X11)
endif()
