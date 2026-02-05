//
// Created by andy on 2/4/26.
//

#pragma once
#include <flecs.h>

#include "GLFW/glfw3.h"
#include "glm/vec2.hpp"
#include "neuron/plugin.hpp"

#include <vulkan/vulkan_raii.hpp>

namespace neuron {
    namespace vulkan {
        class Instance {
          public:
            explicit Instance(Engine &engine);

            Instance(const Instance &other)                = delete;
            Instance(Instance &&other) noexcept            = delete;
            Instance &operator=(const Instance &other)     = delete;
            Instance &operator=(Instance &&other) noexcept = delete;

          private:
            vk::raii::Context  _context;
            vk::raii::Instance _instance;
        };
    } // namespace vulkan

    struct WindowProperties {
        std::optional<glm::ivec2>  size;
        std::optional<std::string> title;
    };

    class Window {
      public:
        Window(const Engine &engine, const WindowProperties &properties);

        Window(const Window &other)                = delete;
        Window(Window &&other) noexcept            = delete;
        Window &operator=(const Window &other)     = delete;
        Window &operator=(Window &&other) noexcept = delete;

        friend bool operator==(const Window &lhs, const Window &rhs) { return lhs._window == rhs._window; }
        friend bool operator!=(const Window &lhs, const Window &rhs) { return !(lhs == rhs); }

        friend std::size_t hash_value(const Window &obj) {
            std::size_t seed = 0x01034E2E;
            seed ^= (seed << 6) + (seed >> 2) + 0x743DC447 + reinterpret_cast<std::size_t>(obj._window);
            return seed;
        }

      private:
        GLFWwindow *_window;
    };

    struct AppInfo {
        std::optional<std::string>               name;
        std::optional<std::tuple<int, int, int>> version;
    };

    class GraphicsPlugin : public Plugin {
      public:
        GraphicsPlugin();
        ~GraphicsPlugin() override = default;

        void initialize(Engine &engine) override;

        flecs::entity createWindow(const WindowProperties &properties);

      private:
        Engine *_engine;
    };

    struct GlfwExists {};

} // namespace neuron
