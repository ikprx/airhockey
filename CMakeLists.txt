cmake_minimum_required(VERSION 2.8)
project(airhockey)

# includes cmake/FindSDL2.cmake
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

include_directories(${CMAKE_SOURCE_DIR}/deps/stb_image/)

set(ASSIMP_LIBRARIES assimp)


set(SOURCE_FILES src/main.cpp src/Game.cpp src/Playstate.cpp src/Shader.cpp src/Mesh.cpp src/Model.cpp src/ResourceManager.cpp )

add_executable(bin/${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(bin/${PROJECT_NAME} ${SDL2_LIBRARY}  ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES}  ${ASSIMP_LIBRARIES})
