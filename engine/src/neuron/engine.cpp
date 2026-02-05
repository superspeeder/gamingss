//
// Created by andy on 2/4/26.
//

#include "engine.hpp"

#include "spdlog/spdlog.h"

namespace neuron {
    Engine::Engine() {
        spdlog::set_level(spdlog::level::debug);

        phases::Physics  = _world.entity().add(flecs::Phase).depends_on(flecs::OnUpdate);
        phases::Graphics = _world.entity().add(flecs::Phase).depends_on(phases::Physics);
#ifdef NDEBUG
        tags::DebugOnly = _world.entity().depends_on(flecs::Disabled);
#else
        tags::DebugOnly = _world.entity();
#endif
        _world.system("Print Delta Time")
            .kind(flecs::OnUpdate)
            .run([](const flecs::iter &it) { spdlog::info("FPS: {}", 1.0f / it.delta_time()); })
            .add(tags::DebugOnly);

        disable_debug_systems();
    }

    void Engine::disable_debug_systems() const {
        _world.defer_begin();
        _world.query_builder<>().with(tags::DebugOnly).build().each([](flecs::entity entity) {
            entity.add(flecs::Disabled);
        });
        _world.defer_end();
    }

    void Engine::enable_debug_systems() const {
        _world.defer_begin();
        _world.query_builder<>().with(tags::DebugOnly).with(flecs::Disabled).build().each([](flecs::entity entity) {
            entity.remove(flecs::Disabled);
        });
        _world.defer_end();
    }

    Engine::~Engine() = default;

    void Engine::run() {
        _world.app().ctx(this).target_fps(0).enable_stats(true).run();
    }

    namespace phases {
        flecs::entity Physics, Graphics;
    }

    namespace tags {
        flecs::entity DebugOnly;
    }
} // namespace neuron
