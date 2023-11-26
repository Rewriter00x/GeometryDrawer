#include <iostream>
#include <thread>

#include "OpenGL/RenderThread.h"

int main()
{
    RenderThread* rt = nullptr;
    std::jthread renderThread(
        [&rt] {
            rt = new RenderThread;
            rt->AddDebugData();
            rt->Run();
            delete rt;
        });

    return 0;
}