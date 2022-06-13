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

    std::vector<std::unique_ptr<NW::Button>> buttons;

    NW::Button addButton(&window, NW::Position(5, 5, 300, 50), L"Add button");
    addButton.EventHandler = [&](NW::ControlEventTypes eventType, NW::ControlEventInfo* eventInfo) {
        switch (eventType)
        {
        case NW::ControlEventTypes::FromParent_Command:
        {
            std::wstringstream wss;
            wss << "Button " << buttons.size() + 1;

            std::unique_ptr<NW::Button> btn(new NW::Button(&window, NW::Position(5, buttons.size() * 35 + 55, 200, 30), wss.str()));

            btn->EventHandler = [i = buttons.size(), &window](NW::ControlEventTypes eventType, NW::ControlEventInfo* eventInfo) {
                switch (eventType)
                {
                case NW::ControlEventTypes::FromParent_Command:
                    std::wstringstream wss;
                    wss << "Clicked button No. " << i + 1;

                    MessageBoxW(window.Hwnd(), wss.str().c_str(), L"Info", MB_OK | MB_ICONINFORMATION);
                    break;
                }
            };

            buttons.push_back(std::move(btn));
            break;
        }
        }
    };

    window.Show();
    return app.MessageLoop();
}