#include <string>
#include <functional>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <iostream>
#include <any>
#include "weavefra/Core/CoreDefined.h"
#pragma once


namespace weavefra
{

    struct WF_API Event
    {
        virtual ~Event() = default;
    };

    class WF_API EventBus
    {
    private:
        template<typename EventT>
        using handler = std::function<void(const EventT&)>;
        std::unordered_map<std::type_index, std::vector<std::function<void(const Event&)>>> listeners;
    public:
        template<typename EventT>
        void subscribe(handler<EventT>);
        template<typename EventT>
        void emit(EventT);
    };
}