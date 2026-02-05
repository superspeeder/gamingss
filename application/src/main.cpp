//
// Created by andy on 2/4/26.
//

#include "neuron/graphics/graphics_plugin.hpp"
#include "spdlog/spdlog.h"


#include <neuron/engine.hpp>

int main() {
    neuron::Engine engine{};

    spdlog::info("GLFW Exists: {}", engine.test_flag<neuron::GlfwExists>());

    engine.add_plugin<neuron::GraphicsPlugin>();

    spdlog::info("GLFW Exists: {}", engine.test_flag<neuron::GlfwExists>());
    engine.run();
    return 0;
}