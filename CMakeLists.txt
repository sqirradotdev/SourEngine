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

add_executable(SourEngine
    src/main.cpp

    # Managers
    src/core/RenderManager.h    src/core/RenderManager.cpp
    src/core/LogManager.h       src/core/LogManager.cpp

    src/core/ShaderProgram.h    src/core/ShaderProgram.cpp

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

if (WIN32)
    add_custom_target(copy_dlls ALL
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SDL2_DIR}/../lib/x64/SDL2.dll" $<TARGET_FILE_DIR:SourEngine>
    )
    add_dependencies(SourEngine copy_dlls)
endif()
