#include "../src/NadWin.hpp"

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
        default:
            break;
        }
    };

    NW::Button button(&window, NW::Position(5, 5, 300, 50), L"Hello, World!");
    button.EventHandler = [&window](NW::ControlEventTypes eventType, NW::ControlEventInfo* eventInfo) {
        switch (eventType)
        {
        case NW::ControlEventTypes::FromParent_Command:
            MessageBoxW(window.Hwnd(), L"You clicked the button!", L"Info", MB_OK | MB_ICONINFORMATION);
            break;
        default:
            break;
        }
    };

    window.Show();
    return app.MessageLoop();
}