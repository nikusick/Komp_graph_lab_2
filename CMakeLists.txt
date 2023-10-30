cmake_minimum_required(VERSION 3.26)
project(field2)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
add_subdirectory(glfw)

include_directories(glfw/include)

add_executable(field2 main.cpp)

target_link_libraries(${PROJECT_NAME} glfw
        ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES})
