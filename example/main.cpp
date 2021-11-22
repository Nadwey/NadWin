#include "NadWin/NadWin.h"

#pragma comment(lib, "Comctl32.lib")

int main()
{
	namespace UI = NW::UI;

	UI::App app("Example app");
	UI::Window window(L"Example window", CW_USEDEFAULT, CW_USEDEFAULT, 800, 600);
	
	UI::Button button(&window, UI::Position(5, 5, 300, 50), L"Hello, World!");
	button.font.SetFaceName(L"Segoe UI Light");
	button.font.SetHeight(35);
	button.UpdateFont();
	button.EventHandler = [&window](UI::EventTypes eventType, UI::Control* control, void*) {
		switch (eventType)
		{
		case NW::UI::EventTypes::MouseLeftDown:
			MessageBoxW(window.Hwnd(), L"Clicked the button.", L"Info", MB_OK | MB_ICONINFORMATION);
			break;
		default:
			break;
		}
	};

	window.Add(&button);
	window.Show();

	app.MessageLoop();
}