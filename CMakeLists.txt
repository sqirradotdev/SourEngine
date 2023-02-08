cmake_minimum_required(VERSION 3.24)

project(SourEngine VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)

if(WIN32)
	set(PYTHON_EXECUTABLE "python")
else()
	set(PYTHON_EXECUTABLE "python3")
endif()

if((NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/deps/.bootstrap.json") OR ("${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt" IS_NEWER_THAN "${CMAKE_CURRENT_SOURCE_DIR}/deps/.bootstrap.json"))
	execute_process(
		COMMAND ${PYTHON_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/deps/bootstrap.py"
		WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/deps"
		RESULT_VARIABLE bootstrap_result
	)
	if (NOT (${bootstrap_result} EQUAL 0))
		message(FATAL_ERROR "Cannot run bootstrap.py")
	endif()
endif()

if (WIN32)
    set(SDL2_DIR "${CMAKE_CURRENT_SOURCE_DIR}/deps/SDL2/cmake")
endif()

find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)

add_subdirectory(deps/glad)
add_subdirectory(deps/glm)
add_subdirectory(deps/spdlog)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}")

add_executable(SourEngine
    src/main.cpp

    src/core/LogManager.h
    src/core/LogManager.cpp

    src/core/resource/ResourceManager.h
    src/core/resource/ResourceManager.tpp
    src/core/resource/ResourceManager.cpp
    src/core/resource/Resource.h
    src/core/resource/Resource.cpp
    src/core/resource/Image.h
    src/core/resource/Image.cpp

    src/core/render/RenderManager.h
    src/core/render/RenderManager.cpp
    src/core/render/ShaderProgram.h
    src/core/render/ShaderProgram.cpp

    src/core/util/Types.h
    src/core/util/Error.h
)

if(TARGET SDL2::SDL2main)
    # It has an implicit dependency on SDL2 functions, so it MUST be added before SDL2::SDL2 (or SDL2::SDL2-static)
    target_link_libraries(SourEngine PRIVATE SDL2::SDL2main)
endif()

target_link_libraries(SourEngine PRIVATE SDL2::SDL2)
target_link_libraries(SourEngine PRIVATE glad)
target_link_libraries(SourEngine PRIVATE glm)
target_link_libraries(SourEngine PRIVATE spdlog::spdlog)

set_target_properties(SourEngine PROPERTIES OUTPUT_NAME "SourEngine")
set_target_properties(SourEngine PROPERTIES DEBUG_POSTFIX "-debug")

if (WIN32)
    add_custom_command(TARGET SourEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SDL2_DIR}/../lib/x64/SDL2.dll" $<TARGET_FILE_DIR:SourEngine>
    )
endif()
