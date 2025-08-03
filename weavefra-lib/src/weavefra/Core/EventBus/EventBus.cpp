#include "EventBus.h"

namespace weavefra
{
    template <typename EventT>
    void EventBus::subscribe(handler<EventT> h)
    {
        std::cout<< "subscribe: " << typeid(EventT).name() << "\n";
        auto it = listeners.find(typeid(EventT));
        
        if(it != listeners.end())
        {
            auto wrapper = [=](const Event& base){
                #ifndef LIB_DEBUG
                    assert(typeid(base) == typeid(EventT));
                #endif
                const auto& result = std::any_cast<const EventT>(base);
            };
            it->second.push_back(wrapper);
        }
        else
        {
            listeners[typeid(EventT)] = { h };
        }
    }
    template <typename EventT>
    void EventBus::emit(EventT e)
    {
        std::cout<< "emit: " << typeid(EventT).name() << "\n";
        auto it = listeners.find(typeid(EventT));
        if(it != listeners.end())
        {
            for(auto fn : it->second)
            {
                fn(e);
            }
        }
    }
}