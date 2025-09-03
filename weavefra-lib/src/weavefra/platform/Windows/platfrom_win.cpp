#include "weavefra/platform/platform.h"
#include "weavefra/Core/logger.h"

#if _OS_WINDOWS

    
    platform_state* Platform_Init(const Platform_InitOptions &Opt)
    {
		platform_state* State = (platform_state *)new platform_state{};
		if (!State) { State->Result = Platform_Error::PLATFORM_ERR_DISCONNECTED; return State; };
		State->window = Platform_Window(Opt.Window_Desc);
		State->window.Show_Platform_Window(1);
		State->Result = Platform_Error::PLATFORM_OK;
        return State;
    }
	Platform_Error Platform_Shutdown(platform_state *plat_state)
	{
		if(!plat_state) { WF_ERROR_LOG("platform state not exist, at : %s : %s", __FILE__, __LINE__);
			return Platform_Error::PLATFORM_ERR_NOMEM; }
		delete plat_state;
		return Platform_Error::PLATFORM_OK;
	}


	
	Platform_Clock::Platform_Clock()
		: Timestamp_(steady_clock::now()), system_time_point(system_clock::now()) {}


	//todo: alligned
	void* platform_allocate(u64 size, size_t aligned, b8 zero_init)
	{
		if(size == 0) return nullptr;
		if(aligned <= alignof(std::max_align_t))
		{
			if(zero_init)
			{
				void* ptr = std::malloc(size);
				std::memset(ptr, 0, size);
				return ptr;
			}
			return malloc(size);
		}
		else
		{
			if(zero_init)
			{
				void* ptr = _aligned_malloc(size, aligned);
				std::memset(ptr, 0, size);
				return ptr;
			}
			return _aligned_malloc(size, aligned);
		}
		return malloc(size);
	}
	void* platform_reallocate(void* ptr, u64 new_size, size_t alignment, b8 zero_init)
	{
		void* new_ptr = nullptr;
		if(alignment <= alignof(std::max_align_t))
		{
			new_ptr = realloc(ptr, new_size);
		}
		else
		{
			new_ptr = _aligned_realloc(ptr, new_size, alignment);
		}

		if(new_ptr && zero_init)
    	{
        	std::memset(new_ptr, 0, new_size);
    	}

		return new_ptr;
	}
	void platform_free(void* block, size_t aligned)
	{
		if(aligned > alignof(std::max_align_t))
		{
			return _aligned_free(block);
		}
		else
		{
			return free(block);
		}
	}
	void* platform_copy_memory(void *dest, const void* source, u64 size)
	{
		return memcpy(dest, source, size);
	}
	void* platform_set_memory(void *dest, i32 value, u64 size)
	{
		return memset(dest, value, size);
	}

#endif
