#pragma once

#include "WFpch.h"

#include "weavefra/Core/CoreDefined.h"
#include "weavefra/Core/logger.h"
#include "weavefra/Core/Asserts.h"

#include "EventType.h"

#include "weavefra/platform/platform.h"


namespace weavefra
{

    template<typename EventT>
    using EventCallbackFn = std::function<void(const EventT&)>;

    class WF_API GlobalEventBus
    {
    private:
        struct handler
        {
            EventCallbackFn<Event> handle;
            u64 ID;
        };
        static inline std::unordered_map<std::type_index, std::vector<handler>> listeners;
        static inline std::unordered_map<u64, std::type_index> id2type;
        static inline std::atomic<u64> NextGlobalHandleID{1};

        static inline std::shared_mutex s_mtx;
        static inline std::atomic<bool> s_strict{true};

    public:
        template<typename EventT>
        static u64 subscribe(EventCallbackFn<EventT>);
        template<typename EventT>
        static void emit(const EventT&);
        template<typename EventT>
        static void unsubscribe(const u64);
    };

    template <typename EventT>
    inline u64 GlobalEventBus::subscribe(EventCallbackFn<EventT> h)
    {
        static_assert(std::is_base_of_v<Event, EventT>, "EventT must derive from Event");
        if(s_strict)
        {
            auto wrapper = [h = std::move(h)](const Event& base)
            {
                if constexpr (std::is_polymorphic_v<Event>)
                {
                    #if _DEBUG
                    WF_ASSERT(typeid(base) == typeid(EventT),
                              "Event type mismatch");
                    #endif
                }
                const auto& result = static_cast<const EventT&>(base);
                h(result);
            };
            std::unique_lock lk(s_mtx);
            const u64 subscribe_ID = NextGlobalHandleID.fetch_add(1, std::memory_order_relaxed);
            listeners[typeid(EventT)].push_back(handler{wrapper, subscribe_ID});
            id2type.emplace(subscribe_ID, typeid(EventT));
            WF_DEBUG_LOG("subscribe to %s \n", typeid(EventT).name());
            return subscribe_ID;
        }
        else
        {
            auto wrapper = [h = std::move(h)](const Event& base)
            {
                if (auto p = dynamic_cast<const EventT*>(&base)) h(*p);
            };

            std::unique_lock lk(s_mtx);
            const u64 subscribe_ID = NextGlobalHandleID.fetch_add(1, std::memory_order_relaxed);
            listeners[typeid(EventT)].push_back(handler{wrapper, subscribe_ID});
            id2type.emplace(subscribe_ID, typeid(EventT));
            WF_DEBUG_LOG("subscribe to %s \n", typeid(EventT).name());
            return subscribe_ID;
        }
    }
    template <typename EventT>
    inline void GlobalEventBus::emit(const EventT& e)
    {
        std::shared_lock lk(s_mtx);
        auto it = GlobalEventBus::listeners.find(typeid(EventT));
        WF_DEBUG_LOG("emitting %s \n", typeid(EventT).name());
        if(it != GlobalEventBus::listeners.end())
        {
            auto handlers = it->second;
            for(const auto& fn : handlers)
            {
                fn.handle(e);
            }
        }
    }

    template <typename EventT>
    inline void GlobalEventBus::unsubscribe(const u64 HandleID)
    {
        std::unique_lock lk(s_mtx);
        auto itType = id2type.find(HandleID);
        if (itType == id2type.end()) { WF_ERROR_LOG("... not found Event"); return; }
        auto it = listeners.find(itType->second);
        if (it != listeners.end())
        {
            auto& vec = it->second;
            vec.erase(std::remove_if(vec.begin(), vec.end(),
                        [HandleID](const handler& h){ return h.ID == HandleID; }),
                      vec.end());
            if (vec.empty()) listeners.erase(it);
        }
        id2type.erase(itType);
        WF_DEBUG_LOG("unsubscribe id=%" PRIu64 "\n", HandleID);
    }
}