---
slug: /
title: Intro
---

## Contents

- [Contents](#contents)
- [Installation](#installation)
- [Examples](#examples)
  - [Simple window](#simple-window)
  - [Simple window with close button handle](#simple-window-with-close-button-handle)
  - [Simple window with button](#simple-window-with-button)
  - [My app is very ugly and looks like it was made on Windows 95](#my-app-is-very-ugly-and-looks-like-it-was-made-on-windows-95)
    - [I don't use MSVC, help.](#i-dont-use-msvc-help)
  - [Destroying](#destroying)

## Installation

Git clone the repo or download `NadWin.hpp` and `NadWin.cpp` and place them next to each other.  
Then just include `NadWin.hpp` and it should work.

## Examples

### Simple window

Really simple window, without close button handle.

```cpp showLineNumbers
#include "NadWin/src/NadWin.hpp"

#pragma comment(lib, "Comctl32.lib")

int main()
{
    NW::App app(L"Really simple app"); // your app name
    NW::Window window(L"Really simple window", CW_USEDEFAULT, CW_USEDEFAULT, 800, 500);

    window.Show();
    return app.MessageLoop();
}
```

### Simple window with close button handle

```cpp showLineNumbers
#include "NadWin/src/NadWin.hpp"

#pragma comment(lib, "Comctl32.lib")

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
```

### Simple window with button

```cpp showLineNumbers
#include "NadWin/src/NadWin.hpp"

#pragma comment(lib, "Comctl32.lib")

int main()
{
    NW::App app(L"app");
    NW::Window window(L"window", CW_USEDEFAULT, CW_USEDEFAULT, 800, 500);

    window.EventHandler = [&](NW::WindowEventTypes eventType, NW::WindowEventInfo* eventInfo) {
        switch (eventType)
        {
        case NW::WindowEventTypes::Destroy:
            PostQuitMessage(0);
            eventInfo->OverrideProcResult(0);
            break;
        }
    };

    NW::Button button(&window, NW::Position(5, 5, 200, 25), L"Button");
    button.EventHandler = [&](NW::ControlEventTypes eventType, NW::ControlEventInfo* eventInfo) {
        switch (eventType)
        {
        case NW::ControlEventTypes::FromParent_Command:
            MessageBoxW(window.Hwnd(), L"You clicked the button!", L"NadWin demo", MB_OK | MB_ICONINFORMATION);
            break;
        }
    };

    window.Show();
    return app.MessageLoop();
}
```

### My app is very ugly and looks like it was made on Windows 95

Well, that is because you have probably some old version of Windows Common Controls®.  
You "update" them by adding this at the top of the file:

```cpp showLineNumbers
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
```

#### I don't use MSVC, help.

Idk, I didn't test it on other compilers but it probably doesn't work.  
Anyways good luck.

### Destroying

NadWin uses high tech C++ destructors to destroy your controls, so you don't(or do) have to worry about memory leaks.  
This is an example of how it's working:

```cpp showLineNumbers
#include "NadWin/src/NadWin.hpp"

#pragma comment(lib, "Comctl32.lib")

int main()
{
    NW::App app(L"destroying");
    NW::Window window(L"destroying demo", CW_USEDEFAULT, CW_USEDEFAULT, 800, 600);

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
```
