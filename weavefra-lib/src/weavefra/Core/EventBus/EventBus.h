#pragma once

#include "WFpch.h"

#include "weavefra/Core/CoreDefined.h"
#include "weavefra/Core/logger.h"
#include "weavefra/Core/Asserts.h"

namespace weavefra
{

    struct WF_API Event
    {
    };

    template<typename EventT>
    using EventCallbackFn = std::function<void(const EventT&)>;

    class WF_API EventBus
    {
    private:
        struct handler
        {
            EventCallbackFn<Event> handle;
            const uint64_t ID;
            handler(EventCallbackFn<Event> Handle,uint64_t id) : handle(Handle), ID(id){}
            handler operator&=(const handler& other) = delete;
        };
        static std::atomic<uint64_t> NextHandlerID;
        
        std::unordered_map<std::type_index, std::vector<handler>> listeners;
    public:
        template<typename EventT>
        uint64_t subscribe(EventCallbackFn<EventT>);
        template<typename EventT>
        void emit(const EventT&);
        template<typename EventT>
        void unsubscribe(const uint64_t&);
    };
    
    
    template <typename EventT>
    inline uint64_t EventBus::subscribe(EventCallbackFn<EventT> h)
    {
        auto wrapper = [h = std::move(h)](const Event& base){
            #ifdef LIB_DEBUG
            WF_ASSERT(typeid(base) == typeid(EventT), "this is not a Event type");
            #endif
            const auto& result = static_cast<const EventT&>(base);
            h(result);
        };
        uint64_t subscribe_ID = this->NextHandlerID.fetch_add(1, std::memory_order_acq_rel);
        listeners[typeid(EventT)].push_back( {wrapper, subscribe_ID} );
        WF_DEBUG_LOG("subscribe to %s \n", typeid(EventT).name());

        return subscribe_ID;
    }
    template <typename EventT>
    inline void EventBus::emit(const EventT& e)
    {
        auto it = listeners.find(typeid(EventT));
        WF_DEBUG_LOG("emitting %s \n", typeid(EventT).name());
        if(it != listeners.end())
        {
            for(auto fn : it->second)
            {
                fn.handle(e);
            }
        }
    }

    template <typename EventT>
    inline void EventBus::unsubscribe(const uint64_t& HandleID)
    {
        auto it = listeners.find(typeid(EventT));
        if(it != listeners.end())
        {
            it->second.erase(std::remove_if(it->second.begin(), it->second.end(), 
            [&HandleID](EventBus::handler HandleCopy){
                return (HandleCopy.ID == HandleID);
            }));
            WF_DEBUG_LOG("unsubscribe %s type of event [Event ID]: %d \n", typeid(EventT).name(), HandleID);
        }
        else
        {
            WF_ERROR_LOG("fail unsubscribe \n no subscribe for %s type of event \n", typeid(EventT).name());
        }
    }

#ifndef _Create_Main_EventBus
        #define _Create_Main_EventBus
        inline EventBus Main_EventBus;
    #endif
}