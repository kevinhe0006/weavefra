#pragma once
#include "weavefra.h"

namespace Sandbox
{
    struct SandboxApplicationEvent : public weavefra::Event
    {
        bool isRun;
        SandboxApplicationEvent(bool isrun) : isRun(isrun){
        }
    };
    class sandboxApplication : public weavefra::Application
    {
    private:
        Sandbox::SandboxApplicationEvent m_SandboxSandboxApplicationEvent;
    public:
        sandboxApplication() : m_SandboxSandboxApplicationEvent(false)
        {
            
        }
        ~sandboxApplication()
        {

        }
        void Run() override;
    };
}