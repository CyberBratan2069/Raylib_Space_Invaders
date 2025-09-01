cmake_minimum_required(VERSION 3.16)
project(SpaceInvaders C)
set(CMAKE_C_STANDARD 11)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
add_subdirectory(external/raylib)

file(GLOB SOURCES src/*.c)
add_executable(SpaceInvaders ${SOURCES})
target_link_libraries(SpaceInvaders PRIVATE raylib)

foreach(dir font graphics sounds)
    if(EXISTS "${CMAKE_SOURCE_DIR}/${dir}")
        add_custom_command(TARGET SpaceInvaders POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_SOURCE_DIR}/${dir}"
                "$<TARGET_FILE_DIR:SpaceInvaders>/${dir}"
        )
    endif()
endforeach()

set_target_properties(SpaceInvaders PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/run"
)




#if(APPLE)
#    target_link_libraries(SpaceInvaders
#            "-framework OpenGL"
#            "-framework Cocoa"
#            "-framework IOKit"
#            "-framework CoreVideo"
#    )
#endif()
#
#
#if(UNIX AND NOT APPLE)
#    target_link_libraries(SpaceInvaders m pthread dl GL rt X11)
#endif()
