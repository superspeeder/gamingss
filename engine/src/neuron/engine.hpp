//
// Created by andy on 2/4/26.
//

#pragma once

#include <flecs.h>

#include "plugin.hpp"
#include <unordered_set>
#include <typeindex>

#include <vector>

namespace neuron {

    class Engine {
      public:
        Engine();
        ~Engine();

        Engine(const Engine &other)                = delete;
        Engine(Engine &&other) noexcept            = delete;
        Engine &operator=(const Engine &other)     = delete;
        Engine &operator=(Engine &&other) noexcept = delete;

        [[nodiscard]] inline flecs::world       &world() noexcept { return _world; };
        [[nodiscard]] inline const flecs::world &world() const noexcept { return _world; };

        void run();

        template<std::derived_from<Plugin> P>
        requires (std::constructible_from<P>)
        void add_plugin() {
            P* plugin = new P();
            plugin->initialize(*this);
            _plugins.push_back(plugin);
        }

        template<typename T>
        void set_flag() {
            _flags.insert(std::type_index(typeid(T)));
        }

        template<typename T>
        void clear_flag() {
            _flags.erase(std::type_index(typeid(T)));
        }


        template<typename T>
        [[nodiscard]] bool test_flag() const {
            return _flags.contains(std::type_index(typeid(T)));
        }



      private:
        flecs::world _world{};
        std::vector<Plugin*> _plugins;
        std::unordered_set<std::type_index> _flags;
    };

    namespace phases {
        extern flecs::entity Physics, Graphics;
    } // namespace phases

    namespace tags {
        extern flecs::entity DebugOnly;
    }

} // namespace neuron
