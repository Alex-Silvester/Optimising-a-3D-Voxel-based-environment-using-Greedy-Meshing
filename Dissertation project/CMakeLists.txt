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
"src/Window/WindowBase"
"src/Window/DrawWindow.h" 
"src/camera.h" 
"src/Interfaces/IDrawable.h"
"src/Shader Types/shader.h"
"src/shapes/triangle.h" 
"src/shapes/Rect.h"
"src/shapes/Cube.h"  
"src/Axis.h" 
"src/Timer.h"
"src/Noise/PerlinNoise.h"
"src/Shader Types/CubeShader.h"
"src/Helpers/Settings.h" 
"src/imgui-1.92.5/imconfig.h"
"src/imgui-1.92.5/imgui.cpp" 
"src/imgui-1.92.5/imgui.h"
"src/imgui-1.92.5/imgui_demo.cpp"
"src/imgui-1.92.5/imgui_draw.cpp" 
"src/imgui-1.92.5/imgui_internal.h"
"src/imgui-1.92.5/imgui_tables.cpp"
"src/imgui-1.92.5/imgui_widgets.cpp" 
"src/imgui-1.92.5/imstb_rectpack.h" 
"src/imgui-1.92.5/imstb_textedit.h" 
"src/imgui-1.92.5/imstb_truetype.h"
"src/imgui-1.92.5/backends/imgui_impl_glfw.cpp"
"src/imgui-1.92.5/backends/imgui_impl_glfw.h"
"src/imgui-1.92.5/backends/imgui_impl_opengl3.cpp" 
"src/imgui-1.92.5/backends/imgui_impl_opengl3.h"
"src/imgui-1.92.5/backends/imgui_impl_opengl3_loader.h"
"src/imgui-1.92.5/misc/cpp/imgui_stdlib.cpp"
"src/imgui-1.92.5/misc/cpp/imgui_stdlib.h"
"src/imgui-1.92.5/misc/fonts/binary_to_compressed_c.cpp"
"src/imgui-1.92.5/misc/single_file/imgui_single_file.h" "src/Shader Types/HUDShader.h" "src/Helpers/HelperFunctions.h" "src/shapes/WireFrame.h" "src/shapes/Particle.h" "src/Shader Types/BillboardShader.h" "src/Helpers/Definitions.h")

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
