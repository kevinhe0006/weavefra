#include <iostream>
#include "weavefra.h"

class sandbox : public weavefra::windowslib
{
public:
    sandbox()
    {

    }
    ~sandbox()
    {

    }
};

int main()
{
    sandbox m_sandbox;
    m_sandbox.Run();
    return 0;
}