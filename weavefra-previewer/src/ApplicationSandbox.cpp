#include "ApplicationSandbox.h"

namespace Sandbox
{
    void sandboxApplication::Run()
    {
        while(m_state->window.WindowExist)
        {
            m_state->window.ProcessMessages();
        }
        std::cin.get();
    }
}