//
// Created by andy on 2/4/26.
//

#include "graphics_plugin.hpp"

#include "GLFW/glfw3.h"
#include "neuron/engine.hpp"

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
        }
    } // namespace vulkan

    GraphicsPlugin::GraphicsPlugin() = default;

    void GraphicsPlugin::initialize(Engine &engine) {
        glfwInit();
        engine.set_flag<GlfwExists>();

        VULKAN_HPP_DEFAULT_DISPATCHER.init();
    }

    flecs::entity GraphicsPlugin::createWindow(const WindowProperties &properties) {
        const auto entity = _engine->world().entity().emplace<Window>(*_engine, properties);
        return entity;
    }
} // namespace neuron
