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
            weavefra::Main_EventBus.subscribe<SandboxApplicationEvent>([](SandboxApplicationEvent e){
                if(e.isRun)
                {
                    std::cout<< "SandboxApplication is Run" << "\n";
                }
                else
                {
                    std::cout<< "SandboxApplication was not exist" << "\n";
                }
            });
            weavefra::Main_EventBus.emit(m_SandboxSandboxApplicationEvent);
        }
        ~sandboxApplication()
        {
            weavefra::Main_EventBus.emit(m_SandboxSandboxApplicationEvent);
        }
        void Run() override;
    };
}