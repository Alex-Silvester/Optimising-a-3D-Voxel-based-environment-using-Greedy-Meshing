# CMakeList.txt : CMake project for Dissertation project, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(PROJECT_NAME  Dissertation_project)
project (${PROJECT_NAME})

include(FetchContent)
FetchContent_Declare(
	GLFW
	GIT_REPOSITORY https://github.com/glfw/glfw.git
	GIT_TAG 7b6aead9fb88b3623e3b3725ebb42670cbe4c579
)
FetchContent_MakeAvailable(GLFW)

include(FetchContent)
FetchContent_Declare(
	GLM
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG 0af55ccecd98d4e5a8d1fad7de25ba429d60e863
)
FetchContent_MakeAvailable(GLM)

# Add source to this project's executable.
add_executable (${PROJECT_NAME} "main.cpp"  
"src/Simulation.h"
"src/Simulation.cpp" 
"src/Window/WindowBase"
"src/Window/DrawWindow.h" 
"src/camera.h" 
"src/Interfaces/IDrawable.h"
"src/shader.h"
"src/shapes/triangle.h" 
"src/shapes/Rect.h"
"src/shapes/Cube.h"  
"src/Axis.h")

target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)

target_include_directories(${PROJECT_NAME} PRIVATE "glad/include")
target_include_directories(${PROJECT_NAME} PRIVATE "glad/src")

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Data/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Data/)

if (CMAKE_VERSION VERSION_GREATER 3.12)
	set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)
	message("-- Using C++23 --")
endif()

# TODO: Add tests and install targets if needed.
