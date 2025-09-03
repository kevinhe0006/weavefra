#pragma once

#include "WFpch.h"

#include "memory.h"
#include "weavefra/platform/platform.h"
#include "weavefra/Core/CoreDefined.h"

namespace weavefra
{
    struct Global_Memory_System
    {
        static WFMemory_state state;
    };

    b8 Memory_System_init()
    {
        WFMemory_init(Global_Memory_System::state);
    }
    b8 Memory_System_shutdown()
    {
        WFMemory_shutdown(Global_Memory_System::state);
    }

    WF_API void* WFS_Allocate(void* ptr,
    size_t new_size,
    size_t alignment = alignof(std::max_align_t),
    MemoryTag tag = MemoryTag::MemoryTag_Unknown)
    {
        return WF_Allocate(Global_Memory_System::state, ptr, new_size, alignment, tag, MemoryFlags::WF_MEM_Default); // MemoryFlags set
    }
    WF_API bool WFS_Free(void* ptr, size_t size)
    {
        return WF_Free(Global_Memory_System::state, ptr, size);
    }

    WF_API void* WFS_copy_memory(void* dst, const void* src, size_t size)
    {
        return WF_copy_memory(Global_Memory_System::state, dst, src, size);
    }
    WF_API void* WFS_set_memory(void* dst, int value, size_t size)
    {
        return WF_set_memory(Global_Memory_System::state, dst, value, size);
    }

    WF_API std::string Get_Memory_System_state()
    {
        return Get_Memory_state_string(Global_Memory_System::state);
    }
    WF_API std::string WFS_Get_Memory_string(void *ptr)
    {
        return Get_Memory_string(Global_Memory_System::state, ptr);
    }
}