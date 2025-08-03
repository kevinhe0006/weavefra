#include <iostream>
#include "weavefra.h"

class sandbox : public weavefra::Application
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