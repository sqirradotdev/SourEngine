cmake_minimum_required(VERSION 3.24)

project(SourEngine VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)

#get_filename_component(SDL2_DIR "./deps/SDL2/cmake" REALPATH)

find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)

add_subdirectory(deps/glad)

add_executable(SourEngine
    src/main.cpp

    src/core/RenderManager.h
    src/core/RenderManager.cpp

    src/core/ShaderProgram.h
    src/core/ShaderProgram.cpp

    src/core/util/Types.h
    src/core/util/Error.h
)

if(TARGET SDL2::SDL2main)
    # It has an implicit dependency on SDL2 functions, so it MUST be added before SDL2::SDL2 (or SDL2::SDL2-static)
    target_link_libraries(SourEngine PRIVATE SDL2::SDL2main)
endif()

target_link_libraries(SourEngine PRIVATE SDL2::SDL2)
target_link_libraries(SourEngine PRIVATE glad)
