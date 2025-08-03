#include "Application.h"
#include <iostream>

namespace weavefra
{
    void Application::Run()
    {
        while(!ShouldEnd)
        {
            if(std::cin.get())
            {
                ShouldEnd = true;
            }
        }
    }
}