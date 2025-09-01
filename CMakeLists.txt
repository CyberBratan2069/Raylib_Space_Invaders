cmake_minimum_required(VERSION 3.16)
project(SpaceInvaders C)
set(CMAKE_C_STANDARD 11)

find_package(raylib 5.0 REQUIRED)

file(GLOB SRC_FILES CONFIGURE_DEPENDS "src/*.c")
add_executable(${PROJECT_NAME} ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE src)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

if(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE
            "-framework OpenGL"
            "-framework Cocoa"
            "-framework IOKit"
            "-framework CoreVideo"
    )
elseif(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE m pthread dl GL rt X11)
endif()