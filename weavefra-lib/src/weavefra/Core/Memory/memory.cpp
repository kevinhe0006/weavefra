#include "memory.h"

#include "weavefra/platform/platform.h"

bool WFMemory_init(WFMemory_state &state)
{
    state.current_bytes.store(0, std::memory_order_relaxed);
    state.peak_bytes.store(0, std::memory_order_relaxed);
    state.alloc_count.store(0, std::memory_order_relaxed);

    for (auto& a : state.by_tag) 
    {
        a.store(0, std::memory_order_relaxed);
    }
    {
        std::lock_guard<std::mutex> lock(state.live_mutex);
        state.live.clear();
        state.live.reserve(1024);
    }

    return true;
    return true;
}

bool WFMemory_shutdown(WFMemory_state &state)
{
    // todo: memory check woring
    return true;
}

// Conventions:
// - ptr == nullptr && new_size > 0 => malloc
// - ptr != nullptr && new_size > 0 => realloc
// - ptr != nullptr && new_size == 0 => free (and returns nullptr)
WF_API void *WF_Allocate(WFMemory_state &state, void *ptr, size_t new_size, size_t alignment, MemoryTag tag, MemoryFlags flags)
{
    void* n_ptr = nullptr;

    if(ptr == nullptr && new_size > 0)
    {
        n_ptr = platform_allocate(new_size, alignment, (u32(flags) & u32(MemoryFlags::WF_MEM_ZeroInit)) != 0);
    }
    else if(ptr != nullptr && new_size > 0)
    {
        n_ptr = platform_reallocate(ptr, new_size, alignment, (u32(flags) & u32(MemoryFlags::WF_MEM_ZeroInit)) != 0);
    }
    else if(ptr != nullptr && new_size == 0)
    {
        WF_Free(state, ptr, alignment);
        return nullptr;
    }
    else 
    {
        return n_ptr;
    }

    if(n_ptr)
    {
        state.alloc_count.fetch_add(1, std::memory_order_relaxed);
        state.by_tag[(size_t)tag].fetch_add(new_size, std::memory_order_relaxed);
        uint64_t current = state.current_bytes.fetch_add(new_size, std::memory_order_relaxed) + new_size;

        uint64_t old_peak = state.peak_bytes.load();
        while (current > old_peak && !state.peak_bytes.compare_exchange_weak(old_peak, current))
        {
        }

        std::lock_guard<std::mutex> lock(state.live_mutex);
        state.live[n_ptr] = { new_size, tag, alignment };
    }

    return n_ptr;
}

WF_API bool WF_Free(WFMemory_state &state, void *ptr, size_t size)
{
    if (!ptr) return false;

    AllocationInfo info;
    {
        std::lock_guard<std::mutex> lock(state.live_mutex);
        auto it = state.live.find(ptr);
        if (it != state.live.end())
        {
            info = it->second;
            state.live.erase(it);
        }
        else
        {
            return false;
        }
    }

    state.current_bytes.fetch_sub(info.size, std::memory_order_relaxed);
    state.by_tag[(size_t)info.tag].fetch_sub(info.size, std::memory_order_relaxed);

    platform_free(ptr, info.alignment);

    return true;
}

WF_API void *WF_copy_memory(WFMemory_state &state, void *dst, const void *src, size_t size)
{
    return platform_copy_memory(dst, src, size);
}

WF_API void *WF_set_memory(WFMemory_state &state, void *dst, int value, size_t size)
{
    return platform_set_memory(dst, value, size);
}

std::string Get_Memory_state_string(WFMemory_state &state)
{
    std::string msg;
    msg += "Total allocations: " + std::to_string(state.alloc_count.load()) + "\n";
    msg += "Current bytes   : " + std::to_string(state.current_bytes.load()) + "\n";
    msg += "Peak bytes      : " + std::to_string(state.peak_bytes.load()) + "\n";

    for (size_t i = 0; i < (size_t)MemoryTag::MemoryTag_Count; ++i)
    {
        msg += "Tag[" + std::to_string(i) + "] = " + std::to_string(state.by_tag[i].load()) + "\n";
    }

    return msg;
}

std::string Get_Memory_string(WFMemory_state &state, void *ptr)
{
    return std::string();//todo
}