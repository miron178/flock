cmake_minimum_required(VERSION 3.0)

project(Flock VERSION 1.0.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (MSVC)
    # multi-process build
    add_compile_options(/MP)
endif()

#Disable shared libs (assimp and glfw)
SET(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libs")

add_compile_definitions(
	NOMINMAX
	_DEBUG
	_CONSOLE
	GLM_FORCE_SWIZZLE
	GLM_FORCE_RADIANS
	GLM_FORCE_PURE
	GLM_ENABLE_EXPERIMENTAL
)

add_subdirectory(deps/assimp)
add_subdirectory(deps/glfw)
add_subdirectory(deps/glm)

# GLAD
add_library(glad STATIC
	deps/glad/src/glad.c
)
target_include_directories(glad PUBLIC
	deps/glad/include
)

#IMGUI
add_library(imgui STATIC
	deps/imgui/imgui.cpp 
	deps/imgui/imgui_demo.cpp 
	deps/imgui/imgui_draw.cpp 
	deps/imgui/imgui_tables.cpp 
	deps/imgui/imgui_widgets.cpp

	deps/imgui/backends/imgui_impl_glfw.cpp
	deps/imgui/backends/imgui_impl_glfw.h
	deps/imgui/backends/imgui_impl_opengl3.cpp
	deps/imgui/backends/imgui_impl_opengl3.h
)
target_include_directories(imgui PUBLIC
	deps/imgui
	deps/imgui/backends
)
target_link_libraries(imgui PUBLIC glad glfw)

add_library(learnopengl INTERFACE
	deps/learnopengl/camera.h
	deps/learnopengl/mesh.h
	deps/learnopengl/model.h
	deps/learnopengl/shader.h
)

target_include_directories(learnopengl INTERFACE deps)

#Main executable
add_executable(Flock
	src/BrainComponent.cpp
	src/BrainComponent.h
	src/Component.cpp
	src/Component.h
	src/Entity.cpp
	src/Entity.h
	src/main.cpp
	src/ModelComponent.cpp
	src/ModelComponent.h
	src/PhysicsComponent.cpp
	src/PhysicsComponent.h
	src/Raycast.cpp
	src/Raycast.h
	src/Scene.cpp
	src/Scene.h
	src/TransformComponent.cpp
	src/TransformComponent.h

	#base
	src/behaviours/Behaviour.cpp
	src/behaviours/Behaviour.h

	# steering
	src/behaviours/Arrive.cpp
	src/behaviours/Arrive.h
	src/behaviours/Avoid.cpp
	src/behaviours/Avoid.h
	src/behaviours/Evade.cpp
	src/behaviours/Evade.h
	src/behaviours/Flee.cpp
	src/behaviours/Flee.h
	src/behaviours/Pursue.cpp
	src/behaviours/Pursue.h
	src/behaviours/Seek.cpp
	src/behaviours/Seek.h
	src/behaviours/Wander.cpp
	src/behaviours/Wander.h

	#flocking
	src/behaviours/Alignment.cpp
	src/behaviours/Alignment.h
	src/behaviours/Cohesion.cpp
	src/behaviours/Cohesion.h
	src/behaviours/Separation.cpp
	src/behaviours/Separation.h
)

if (MSVC)
    # warning level 4 and all warnings as errors
    #target_compile_options(Flock PRIVATE /W4 /WX)
    target_compile_options(Flock PRIVATE /W3)
else()
    # lots of warnings and all warnings as errors
    #target_compile_options(Flock PRIVATE -Wall -Wextra -pedantic -Werror)
    target_compile_options(Flock PRIVATE -Wall -Wextra -pedantic)
endif()

target_include_directories(Flock PUBLIC
	deps/glad/include
	src
	src/behaviours
)

target_link_libraries(Flock PUBLIC assimp glad glm glfw imgui learnopengl)
