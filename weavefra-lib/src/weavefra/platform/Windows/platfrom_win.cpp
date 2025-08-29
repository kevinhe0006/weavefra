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



	void* platform_allocate(u64 size, bool aligned)
	{
		return malloc(size);
	}
	void platform_free(void* block, bool aligned)
	{
		free(block);
	}
	void* platform_copy_memory(void *dest, const void* source, u64 size)
	{
		return memcpy(dest, source, size);
	}
	void* platform_set_memory(void *dest, s32 value, u64 size)
	{
		return memset(dest, value, size);
	}

#endif
