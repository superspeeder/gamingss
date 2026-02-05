//
// Created by andy on 2/4/26.
//

#include "graphics_plugin.hpp"

#include "GLFW/glfw3.h"
#include "neuron/engine.hpp"

#include <spdlog/spdlog.h>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE;

namespace neuron {
    namespace vulkan {
        Instance::Instance(Engine &engine) : _instance{nullptr} {
            vk::InstanceCreateInfo createInfo{};
            vk::ApplicationInfo    appInfo{};

            if (engine.world().has<AppInfo>()) {
                const auto &[name, version] = engine.world().get<AppInfo>();
                if (name.has_value()) {
                    appInfo.setPApplicationName(name.value().c_str());
                }

                if (version.has_value()) {
                    auto [major, minor, patch] = version.value();
                    appInfo.setApplicationVersion(VK_MAKE_VERSION(major, minor, patch));
                }
            }

            appInfo.setApiVersion(VK_API_VERSION_1_4);
            createInfo.ppEnabledExtensionNames = glfwGetRequiredInstanceExtensions(&createInfo.enabledExtensionCount);
            createInfo.pApplicationInfo        = &appInfo;

            _instance = vk::raii::Instance(_context, createInfo);
            spdlog::info("Created Instance");
        }
    } // namespace vulkan

    Window::Window(const Engine &engine, const WindowProperties &properties) {
        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        const auto size  = properties.size.value_or(glm::ivec2(800, 600));
        const auto title = properties.title.value_or("Window");
        _window          = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, nullptr);
    }

    Window::~Window() {
        glfwDestroyWindow(_window);
    }

    bool Window::should_close() const {
        return glfwWindowShouldClose(_window);
    }

    GraphicsPlugin::GraphicsPlugin() = default;

    void GraphicsPlugin::initialize(Engine &engine) {
        _engine = &engine;

        phases::PreUpdatePostEvents = engine.world().entity().add(flecs::Phase).depends_on(flecs::PreUpdate);

        glfwInit();
        engine.set_flag<GlfwExists>();

        VULKAN_HPP_DEFAULT_DISPATCHER.init();

        engine.world().emplace<vulkan::Instance>(engine);

        {
            const auto      *window_properties = engine.world().try_get<WindowProperties>();
            WindowProperties properties{};
            if (window_properties) {
                properties = *window_properties;
            }
            create_window(properties);
        }


        engine.world()
            .system<const Window>("Check Window Status")
            .kind(phases::PreUpdatePostEvents)
            .each([](const flecs::entity entity, const Window &window) {
                if (window.should_close()) {
                    entity.remove<Window>();
                }
            });

        flecs::query window_query = engine.world().query<const Window>();

        engine.world().system<>("Check Open Windows").kind(flecs::PreUpdate).run([window_query](flecs::iter &it) {
            if (window_query.count() == 0) {
                it.world().quit();
            }
        });

        engine.world().system("Poll Events").kind(flecs::PreUpdate).run([](flecs::iter &) { glfwPollEvents(); });
    }

    flecs::entity GraphicsPlugin::create_window(const WindowProperties &properties) {
        const auto entity = _engine->world().entity().emplace<Window>(*_engine, properties);
        return entity;
    }

    namespace phases {
        flecs::entity PreUpdatePostEvents;
    }
} // namespace neuron
