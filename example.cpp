#include "src/NadWin.cpp"

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "gdi32.lib")

int main()
{
    NW::App app(L"app name");
    NW::Window window(L"your beautiful window", CW_USEDEFAULT, CW_USEDEFAULT, 800, 500);

    window.EventHandler = [&](NW::WindowEventTypes eventType, NW::WindowEventInfo* eventInfo) {
        switch (eventType)
        {
        case NW::WindowEventTypes::Destroy:
            PostQuitMessage(0);
            eventInfo->OverrideProcResult(0);
            break;
        }
    };

    window.Show();
    return app.MessageLoop();
}