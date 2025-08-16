#pragma once

#include "WFpch.h"

#include "weavefra/Core/CoreDefined.h"
#include "weavefra/platform/platform.h"

namespace weavefra
{
    class WF_API Application
    {
    public:
        Application();
        virtual void Run();
    };
}