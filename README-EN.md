# NadWin

WinAPI GUI Library (kind of a wrapper).

![Example](./screen.png)

## Usage

Example (MSVC):

```cpp
#include "NadWin/src/NadWin.hpp"

#pragma comment(lib, "Comctl32.lib")

#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

int main()
{
    NW::App app(L"Example app");
    NW::Window window(L"Example window", CW_USEDEFAULT, CW_USEDEFAULT, 800, 600);
    
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
```
