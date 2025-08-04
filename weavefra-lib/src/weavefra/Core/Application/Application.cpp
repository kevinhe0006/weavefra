#include "Application.h"

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