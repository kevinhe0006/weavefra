#include "ApplicationSandbox.h"

namespace Sandbox
{
    void sandboxApplication::Run()
    {
        while (!ShouldEnd)
        {
            m_SandboxSandboxApplicationEvent.isRun = true;

            if(std::cin.get())
            {
                ShouldEnd = true;
            }

            weavefra::Main_EventBus.emit(m_SandboxSandboxApplicationEvent);
            
            weavefra::Main_EventBus.subscribe<SandboxApplicationEvent>([](SandboxApplicationEvent e){
                if(e.isRun)
                {
                    std::cout<< "SandboxApplication is Run" << "\n";
                }
                else
                {
                    std::cout<< "SandboxApplication is Run" << "\n";
                }
            });
        }
        m_SandboxSandboxApplicationEvent.isRun = false;
        weavefra::Main_EventBus.emit(m_SandboxSandboxApplicationEvent);
    }
}