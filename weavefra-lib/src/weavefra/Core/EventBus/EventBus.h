#pragma once

#include "WFpch.h"

#include "weavefra/Core/CoreDefined.h"

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
    
    
    template <typename EventT>
    void EventBus::subscribe(handler<EventT> h)
    {
        auto wrapper = [=](const Event& base){
            #ifndef LIB_DEBUG
            assert(typeid(base) == typeid(EventT));
            #endif
            const auto& result = static_cast<const EventT&>(base);
            h(result);
        };
        listeners[typeid(EventT)] = { wrapper };
        std::cout<< "subscribe to " << typeid(EventT).name() << "\n";
    }
    template <typename EventT>
    void EventBus::emit(EventT e)
    {
        auto it = listeners.find(typeid(EventT));
        std::cout<< "emitting " << typeid(EventT).name() << "\n";
        if(it != listeners.end())
        {
            for(auto fn : it->second)
            {
                fn(e);
            }
        }
    }
    
    #ifndef _Create_Main_EventBus
        #define _Create_Main_EventBus
        inline EventBus Main_EventBus;
    #endif
}