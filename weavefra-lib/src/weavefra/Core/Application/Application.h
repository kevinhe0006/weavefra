#pragma once

#include "WFpch.h"

#include "weavefra/Core/CoreDefined.h"
#include "weavefra/platform/platform.h"
#include "weavefra/Core/Asserts.h"
#include "weavefra/platform/platform.h"

namespace weavefra
{
    class WF_API Application
    {
    private:
        platform_state state;
    public:
        Application();
        virtual void Run();
    };
}

struct WFApplicationPoint
{
    weavefra::Application* state;
};