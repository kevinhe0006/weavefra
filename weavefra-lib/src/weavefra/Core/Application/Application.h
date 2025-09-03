#pragma once

#include "WFpch.h"

#include "weavefra/Core/CoreDefined.h"
#include "weavefra/platform/platform.h"
#include "weavefra/Core/Asserts.h"
#include "weavefra/Core/EventBus/GlobalEventBus.h"

namespace weavefra
{
    enum class ApplicationSignal
    {
       OK = 0,

        APPLICATION_UNKNOWN         = -1,
        APPLICATION_NOT_FOUND       = -2,
        APPLICATION_DISMISS         = -3,

        APPLICATION_UNDEFINED       = -4,
        APPLICATION_ERR_FAIL        = -7,
        APPLICATION_ERR_CRUMBLE     = -8,
        APPLICATION_ERR_SUBSYSTEM   = -9
    };

    enum class ApplicationStatus
    {
        APPLICATION_OK        =  0,
        APPLICATION_DISMISS   = -1,
        APPLICATION_ERROR     = -2,
        APPLICATION_CRUMBLE   = -3,
        APPLICATION_DESTROY   = -4 
    };

    

    struct Application_Event : public Event
    {
        ApplicationStatus status;
        b8 Initialized;
    };

    class WF_API Application
    {
    private:
        platform_state PlatformState;
    public:
        b8 Initialized;

        Application();
        virtual void Run();
    };
}

struct WFApplicationPointer
{
    weavefra::Application applecation;
    weavefra::Application_Event Event;
    weavefra::ApplicationStatus status;
};