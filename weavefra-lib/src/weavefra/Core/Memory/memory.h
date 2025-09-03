#pragma once

#include "WFpch.h"

#include "weavefra/Core/CoreDefined.h"

enum struct MemoryTag : uint8_t
{
    MemoryTag_Unknown = 0, MemoryTag_General, MemoryTag_Renderer, MemoryTag_Resource, MemoryTag_Temp
    , MemoryTag_String, MemoryTag_Job, MemoryTag_Count
};

enum struct MemoryGroup : uint8_t
{
    MemoryGroup_None = 0, MemoryGroup_Frame, MemoryGroup_Persistent, MemoryGroup_Tooling, MemoryGroup_Count
};

enum struct MemoryFlags : uint32_t
{
    WF_MEM_Default  = 0,
    WF_MEM_ZeroInit = 1 << 0
};

struct AllocationInfo
{
    uint64_t size = 0;
    MemoryTag tag = MemoryTag::MemoryTag_Unknown;
    size_t alignment = alignof(std::max_align_t);
};

struct WFMemory_state
{
    std::atomic<uint64_t> current_bytes{0};
    std::atomic<uint64_t> peak_bytes{0};
    std::atomic<uint64_t> alloc_count{0};
    std::array<std::atomic<uint64_t>, (size_t)MemoryTag::MemoryTag_Count> by_tag{};

    std::mutex live_mutex;
    std::unordered_map<void*, AllocationInfo> live;
};

bool WFMemory_init(WFMemory_state& state);
//todo : WFMemory_state check
bool WFMemory_shutdown(WFMemory_state& state);


// Conventions:
// - ptr == nullptr && new_size > 0 => malloc
// - ptr != nullptr && new_size > 0 => realloc
// - ptr != nullptr && new_size == 0 => free (and returns nullptr)
inline void* WF_Allocate(WFMemory_state& state,
    void* ptr,
    size_t new_size,
    size_t alignment = alignof(std::max_align_t),
    MemoryTag tag = MemoryTag::MemoryTag_Unknown,
    MemoryFlags flags = MemoryFlags::WF_MEM_Default);

inline bool  WF_Free(WFMemory_state& state, void* ptr, size_t size);

inline void* WF_copy_memory(WFMemory_state& state, void* dst, const void* src, size_t size);
inline void* WF_set_memory(WFMemory_state& state, void* dst, int value, size_t size);

inline std::string Get_Memory_state_string(WFMemory_state& state);

inline std::string Get_Memory_string(WFMemory_state& state, void* ptr);