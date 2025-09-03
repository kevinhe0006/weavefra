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
        platform_state* m_state;
    public:
        sandboxApplication() : m_SandboxSandboxApplicationEvent(false), m_state(Platform_Init())
        {
            weavefra::GlobalEventBus::subscribe<SandboxApplicationEvent>([](SandboxApplicationEvent e){
                if(e.isRun)
                {
                    std::cout<< "SandboxApplication is Run" << "\n";
                }
                else
                {
                    std::cout<< "this SandboxApplication was not exist" << "\n";
                }
            });
            weavefra::GlobalEventBus::emit(m_SandboxSandboxApplicationEvent);
        }
        ~sandboxApplication()
        {
            weavefra::GlobalEventBus::emit(m_SandboxSandboxApplicationEvent);
            Platform_Shutdown(m_state);
        }
        void Run() override;
    };
}