#include "WFpch.h"
#include "EventBus.h"

namespace weavefra
{
    std::atomic<uint64_t> EventBus::NextHandlerID{1};
}