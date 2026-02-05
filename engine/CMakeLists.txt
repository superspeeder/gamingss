set(CMAKE_CXX_STANDARD 26)

include(FetchContent)
FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG 3.4)
FetchContent_Declare(glm GIT_REPOSITORY https://github.com/g-truc/glm.git GIT_TAG 1.0.3)
FetchContent_Declare(spdlog GIT_REPOSITORY https://github.com/gabime/spdlog.git GIT_TAG v1.17.0)
FetchContent_Declare(VulkanHeaders GIT_REPOSITORY https://github.com/KhronosGroup/Vulkan-Headers.git GIT_TAG v1.4.342)
FetchContent_Declare(flecs GIT_REPOSITORY https://github.com/SanderMertens/flecs.git GIT_TAG v4.1.4)
FetchContent_MakeAvailable(glfw glm spdlog VulkanHeaders flecs)

add_library(neuron src/neuron/engine.cpp src/neuron/engine.hpp
        src/neuron/plugin.cpp
        src/neuron/plugin.hpp
        src/neuron/graphics/graphics_plugin.cpp
        src/neuron/graphics/graphics_plugin.hpp)
target_include_directories(neuron PUBLIC src/)
target_link_libraries(neuron PUBLIC glfw glm::glm spdlog::spdlog Vulkan::Headers flecs::flecs)
target_compile_definitions(neuron PUBLIC VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1 GLM_ENABLE_EXPERIMENTAL)
add_library(neuron::neuron ALIAS neuron)
