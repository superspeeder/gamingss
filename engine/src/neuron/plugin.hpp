//
// Created by andy on 2/4/26.
//

#pragma once

namespace neuron {
    class Engine;

    class Plugin {
      public:
        Plugin() = default;
        virtual ~Plugin() = default;

        Plugin(const Plugin &other)                = delete;
        Plugin(Plugin &&other) noexcept            = delete;
        Plugin &operator=(const Plugin &other)     = delete;
        Plugin &operator=(Plugin &&other) noexcept = delete;

        virtual void initialize(Engine &engine) = 0;
    };

} // namespace neuron
