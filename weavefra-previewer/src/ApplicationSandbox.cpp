#include "ApplicationSandbox.h"

namespace Sandbox
{
    void sandboxApplication::Run()
    {
        while (!ShouldEnd)
        {
            m_SandboxSandboxApplicationEvent.isRun = true;
            weavefra::Main_EventBus.emit(m_SandboxSandboxApplicationEvent);

            if(auto input = std::cin.get())
            {
                char key = input;
                if(key == 'E')
                {
                    ShouldEnd = true;
                    std::cin.get();
                }
            }
        }
        m_SandboxSandboxApplicationEvent.isRun = false;
    }
}