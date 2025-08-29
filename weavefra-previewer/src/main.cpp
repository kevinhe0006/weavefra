#include "ApplicationSandbox.h"
#include "weavefra/Core/EventBus/GlobalEventBus.h"


int main()
{
    Sandbox::sandboxApplication* m_sandbox = new Sandbox::sandboxApplication;
    WF_FATAL_LOG("test log");
    WF_ERROR_LOG("test log");
    WF_WORNING_LOG("test log");
    WF_INFO_LOG("test log");
    WF_DEBUG_LOG("test log");
    WF_TRACE_LOG("text log");
    m_sandbox->Run();
    delete m_sandbox;
    exit(EXIT_SUCCESS);
    std::cin.get();
    return 0;
}