#include <weavefra.h>
#include "ApplicationSandbox.h"


int main()
{
    Sandbox::sandboxApplication* m_sandbox = new Sandbox::sandboxApplication;
    m_sandbox->Run();
    delete m_sandbox;
    std::cin.get();
    return 0;
}