cmake_minimum_required(VERSION 3.0)

project(Flock VERSION 1.0.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

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
	deps/imgui/backends/imgui_impl_glfw.cpp
	deps/imgui/backends/imgui_impl_glfw.h
	deps/imgui/backends/imgui_impl_opengl3.cpp
	deps/imgui/backends/imgui_impl_opengl3.h
)
target_include_directories(imgui PUBLIC
	deps/imgui
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
	src/Scene.cpp
	src/Scene.h
	src/TransformComponent.cpp
	src/TransformComponent.h

)

target_include_directories(Flock PUBLIC
	deps/glad/include
)

target_link_libraries(Flock PUBLIC assimp glad glm glfw imgui learnopengl)
