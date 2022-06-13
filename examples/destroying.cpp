#include "../src/NadWin.hpp"
#include <sstream>

#pragma comment(lib, "Comctl32.lib")

int main()
{
    NW::App app(L"Example app");
    NW::Window window(L"Example window", CW_USEDEFAULT, CW_USEDEFAULT, 800, 600);

    window.EventHandler = [&](NW::WindowEventTypes eventType, NW::WindowEventInfo* eventInfo) {
        switch (eventType)
        {
        case NW::WindowEventTypes::Destroy:
            PostQuitMessage(0);
            eventInfo->OverrideProcResult(0);
            break;
        }
    };

    NW::Static label1(&window, NW::Position(5, 5, 200, 25), L"This label will show");
    {
        NW::Static label2(&window, NW::Position(5, 35, 200, 25), L"This label won't show");
        // end of the scope, gets destroyed
    }
    NW::Static label3(&window, NW::Position(210, 35, 200, 25), L"<-- label2 should be here");

    window.Show();
    return app.MessageLoop();
}