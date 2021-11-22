#include "NadWin.h"

namespace NW
{
	namespace UI {
		std::wstring s2ws(std::string s)
		{
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			std::wstring ws = converter.from_bytes(s);
			return ws;
		}

		//
		//
		// class Font
		//
		//

		//
		// public:
		//

		Font::Font(int height, std::wstring faceName, int width, bool italic, bool underline, bool strike)
			: height(height)
			, faceName(faceName)
			, width(width)
			, italic(italic)
			, underline(underline)
			, strike(strike)
		{
			update();
		}

		Font::Font(int height, std::string faceName, int width, bool italic, bool underline, bool strike) : height(height)
			, faceName(s2ws(faceName))
			, width(width)
			, italic(italic)
			, underline(underline)
			, strike(strike)
		{
			update();
		}

		Font::~Font()
		{
			if (font) DeleteObject(font);
		}

		void Font::SetHeight(int height)
		{
			this->height = height;
			update();
		}
		void Font::SetFaceName(std::wstring faceName)
		{
			this->faceName = faceName;
			update();
		}
		void Font::SetFaceName(std::string faceName)
		{
			this->faceName = s2ws(faceName);
			update();
		}
		void Font::SetWidth(int width)
		{
			this->width = width;
			update();
		}
		void Font::SetItalic(bool italic)
		{
			this->italic = italic;
			update();
		}
		void Font::SetUnderline(bool underline)
		{
			this->underline = underline;
			update();
		}
		void Font::SetStrike(bool strike)
		{
			this->strike = strike;
			update();
		}

		int Font::GetHeight()
		{
			return height;
		}
		std::wstring Font::GetFaceName()
		{
			return faceName;
		}
		int Font::GetWidth()
		{
			return width;
		}
		bool Font::GetItalic()
		{
			return italic;
		}
		bool Font::GetUnderline()
		{
			return underline;
		}
		bool Font::GetStrike()
		{
			return strike;
		}

		const HFONT Font::GetFont()
		{
			return font;
		}

		//
		// private:
		//

		void Font::update()
		{
			if (font) DeleteObject(font);
			font = CreateFontW(height, width, 0, 0, 0, italic, underline, strike, 0, 0, 0, ANTIALIASED_QUALITY, 0, faceName.c_str());
		}



		//
		//
		// class Position
		//
		//

		//
		// public:
		//

		Position::Position(int x, int y, int width, int height) : x(x), y(y), width(width), height(height)
		{

		}

		Position::Position(RECT rect) : Position(&rect)
		{

		}

		Position::Position(RECT* rect)
		{
			FromRect(rect);
		}

		RECT Position::Rect()
		{
			RECT rc;
			rc.left = x;
			rc.top = y;
			rc.right = x + width;
			rc.bottom = y + height;
			return rc;
		}

		void Position::FromRect(RECT rect)
		{
			FromRect(&rect);
		}

		void Position::FromRect(RECT* rect)
		{
			x = rect->left;
			y = rect->top;
			width = rect->right - rect->left;
			height = rect->bottom - rect->top;
		}

		//
		//
		// class Border
		//
		//

		//
		// public:
		//

		Border::Border(int left, int top, int right, int bottom, COLORREF color)
			: left(left)
			, top(top)
			, right(right)
			, bottom(bottom)
		{

		}

		bool Border::NeedsRender()
		{
			return left || top || right || bottom;
		}

		//
		//
		// class App
		//
		//

		//
		// public:
		//

		App::App(std::wstring AppName)
		{
			registerClass(AppName);
		}

		App::App(std::string AppName)
		{
			std::wstring AppNameW = s2ws(AppName);
			registerClass(AppNameW);
		}

		App::~App()
		{
			UnregisterClassW(AppName.c_str(), hInstance);
			initialized = false;
		}

		int App::MessageLoop()
		{
			MSG msg;
			while (GetMessage(&msg, NULL, 0, 0) != 0)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			return msg.wParam;
		}

		void App::DoEvents()
		{
			MSG msg;
			BOOL result;

			while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
			{
				result = ::GetMessage(&msg, NULL, 0, 0);
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}

		//
		// private:
		//

		void App::registerClass(std::wstring& AppName)
		{
			if (initialized) throw std::exception("Application class is arleady registered");
			hInstance = reinterpret_cast<HINSTANCE>(GetModuleHandle(nullptr));

			WNDCLASSEXW wcx = { 0 };

			wcx.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
			wcx.cbSize = sizeof(WNDCLASSEXW);
			wcx.lpfnWndProc = reinterpret_cast<WNDPROC>(proc);
			wcx.hInstance = hInstance;
			wcx.hCursor = LoadCursorW(nullptr, IDC_ARROW);
			wcx.hIcon = LoadIconW(nullptr, IDI_APPLICATION);
			wcx.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW+1);
			wcx.lpszClassName = AppName.c_str();

			if (!RegisterClassExW(&wcx)) throw std::exception("Failed to register application class");
			this->AppName = AppName;
			initialized = true;
		}

		LRESULT CALLBACK App::proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			Window* windowHandle = reinterpret_cast<Window*>(GetWindowLongPtrA(hwnd, GWLP_USERDATA));
			if (windowHandle) return windowHandle->proc(msg, wParam, lParam);
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}

		std::wstring App::AppName = L"";
		HINSTANCE App::hInstance = nullptr;
		unsigned long App::windowCount = 0;
		bool App::initialized = false;

		//
		//
		// class Window
		//
		//

		//
		// public:
		//

		Window::Window(std::wstring WindowName, int x, int y, int width, int height)
		{
			createWindow(WindowName, x, y, width, height);
		}

		Window::Window(std::string WindowName, int x, int y, int width, int height)
		{
			std::wstring WindowNameW = s2ws(WindowName);
			createWindow(WindowNameW, x, y, width, height);
		}

		Window::~Window()
		{
			DestroyWindow(hwnd);
		}

		const HWND Window::Hwnd()
		{
			return hwnd;
		}

		void Window::Move(int x, int y, int width, int height, bool repaint)
		{
			MoveWindow(hwnd, x, y, width, height, repaint);
		}

		void Window::Show()
		{
			ShowWindow(hwnd, SW_SHOW);
			UpdateWindow(hwnd);
		}

		void Window::Add(Control* button)
		{
			button->create();
		}

		void Window::Repaint()
		{
			InvalidateRect(hwnd, nullptr, true);
		}

		//
		// private:
		//

		void Window::createWindow(std::wstring& WindowName, int x, int y, int width, int height)
		{
			hwnd = CreateWindowExW(0L, App::AppName.c_str(), WindowName.c_str(), WS_OVERLAPPEDWINDOW, x, y, width, height, nullptr, nullptr, App::hInstance, nullptr);
			if (!hwnd) throw std::exception("Failed to create window");
			SetWindowLongPtrA(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
		}

		LRESULT CALLBACK Window::proc(UINT msg, WPARAM wParam, LPARAM lParam)
		{
			switch (msg)
			{
			default:
				break;
			}

			return DefWindowProc(hwnd, msg, wParam, lParam);
		}

		//
		//
		// class Control
		//
		//

		//
		// public:
		//

		void Control::Repaint()
		{
			InvalidateRect(hwnd, nullptr, false);
		}

		void Control::UpdateFont()
		{
			SendMessage(hwnd, WM_SETFONT, reinterpret_cast<WPARAM>(font.GetFont()), false);
		}

		Position Control::GetPosition()
		{
			return position;
		}

		void Control::SetPosition(Position position)
		{
			this->position = position;
			MoveWindow(hwnd, position.x, position.y, position.width, position.height, false);
		}

		std::wstring Control::GetText()
		{
			return text;
		}

		void Control::SetText(std::string text)
		{
			std::wstring ws = s2ws(text);
			this->text = ws;
			updateText();
		}

		void Control::SetText(std::wstring text)
		{
			this->text = text;
			updateText();
		}

		void Control::Focus()
		{
			SetFocus(hwnd);
		}

		void Control::RemoveFocus()
		{
			SetFocus(nullptr);
		}

		//
		// protected:
		//

		Control::Control() : font(20, L"Segoe UI")
		{

		}

		void Control::create()
		{
			throw std::exception("Can't create pure control class");
		}

		LRESULT CALLBACK Control::ControlProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
		{
			Control* control = reinterpret_cast<Control*>(dwRefData);
			switch (msg)
			{
			case WM_MOUSEMOVE:
			{
				TRACKMOUSEEVENT tme = { 0 };
				tme.cbSize = sizeof(tme);
				tme.hwndTrack = control->hwnd;
				tme.dwFlags = TME_LEAVE;
				TrackMouseEvent(&tme);

				if (control->isOver) break;
				control->isOver = true;

				if (control->EventHandler) control->EventHandler(EventTypes::MouseOver, control, nullptr);
				break;
			}
			case WM_LBUTTONDBLCLK:
				if (control->EventHandler) control->EventHandler(EventTypes::MouseLeftDoubleClick, control, nullptr);
				return 0;
			case WM_LBUTTONDOWN:
				if (control->EventHandler) control->EventHandler(EventTypes::MouseLeftDown, control, nullptr);
				return 0;
			case WM_LBUTTONUP:
				if (control->EventHandler) control->EventHandler(EventTypes::MouseLeftUp, control, nullptr);
				return 0;
			case WM_RBUTTONDBLCLK:
				if (control->EventHandler) control->EventHandler(EventTypes::MouseRightDoubleClick, control, nullptr);
				return 0;
			case WM_RBUTTONDOWN:
				if (control->EventHandler) control->EventHandler(EventTypes::MouseRightDown, control, nullptr);
				return 0;
			case WM_RBUTTONUP:
				if (control->EventHandler) control->EventHandler(EventTypes::MouseRightUp, control, nullptr);
				return 0;
			case WM_MBUTTONDBLCLK:
				if (control->EventHandler) control->EventHandler(EventTypes::MouseMiddleDoubleClick, control, nullptr);
				return 0;
			case WM_MBUTTONDOWN:
				if (control->EventHandler) control->EventHandler(EventTypes::MouseMiddleDown, control, nullptr);
				return 0;
			case WM_MBUTTONUP:
				if (control->EventHandler) control->EventHandler(EventTypes::MouseMiddleUp, control, nullptr);
				return 0;
			case WM_MOUSELEAVE:
			{
				if (!control->isOver) break;
				control->isOver = false;

				if (control->EventHandler) control->EventHandler(EventTypes::MouseLeave, control, nullptr);

				break;
			}
			default:
				std::tuple<UINT, WPARAM, LPARAM> data = { msg, wParam, lParam };
				if (control->EventHandler) control->EventHandler(EventTypes::Undefined, control, reinterpret_cast<void*>(&data));
				break;
			}

			return DefSubclassProc(hwnd, msg, wParam, lParam);
		}

		void Control::updateText()
		{
			SendMessageW(hwnd, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(text.c_str()));
		}

		//
		//
		// class Button
		//
		//

		//
		// public:
		//

		Button::Button(Window* window, Position position, std::string text)
		{
			this->window = window;
			std::wstring ws = s2ws(text);
			initialize(position, ws);
		}

		Button::Button(Window* window, Position position, std::wstring text)
		{
			this->window = window;
			initialize(position, text);
		}

		Button::~Button()
		{
			if (hwnd)
			{
				DestroyWindow(hwnd);
				hwnd = nullptr;
			}
		}

		//
		// private:
		//

		void Button::initialize(Position& position, std::wstring& text)
		{
			this->position = position;
			SetPosition(position);
			SetText(text);
		}

		void Button::create()
		{
			hwnd = CreateWindowExW(0, L"BUTTON", text.c_str(), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, position.x, position.y, position.width, position.height, window->Hwnd(), nullptr, nullptr, nullptr);
			SetWindowLongPtrA(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
			SetWindowSubclass(hwnd, Button::ControlProc, 0, reinterpret_cast<DWORD_PTR>(this));
			UpdateFont();
		}


		//
		//
		// class Static
		//
		//

		//
		// public:
		//

		Static::Static(Window* window, Position position, std::string text)
		{
			this->window = window;
			std::wstring ws = s2ws(text);
			initialize(position, ws);
		}

		Static::Static(Window* window, Position position, std::wstring text)
		{
			this->window = window;
			initialize(position, text);
		}
		Static::~Static()
		{
			if (hwnd)
			{
				DestroyWindow(hwnd);
				hwnd = nullptr;
			}
		}

		//
		// private:
		//

		void Static::initialize(Position& position, std::wstring& text)
		{
			this->position = position;
			SetPosition(position);
			SetText(text);
		}

		void Static::create()
		{
			hwnd = CreateWindowExW(0, L"STATIC", text.c_str(), WS_VISIBLE | WS_CHILD, position.x, position.y, position.width, position.height, window->Hwnd(), nullptr, nullptr, nullptr);
			SetWindowLongPtrA(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
			SetWindowSubclass(hwnd, Button::ControlProc, 0, reinterpret_cast<DWORD_PTR>(this));
			UpdateFont();
		}
	}
	
	
	

	//
	//
	// class Random
	//
	//

	//
	// public:
	//

	Random::Random()
	{
		static bool first = true;
		if (first)
		{
			srand(time(nullptr));
			first = false;
		}
	}

	inline int Random::Get()
	{
		return rand();
	}

	int Random::Get(int min, int max)
	{
		return min + rand() % ((max + 1) - min);
	}

	float Random::GetF()
	{
		return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	}

	float Random::GetF(float min, float max)
	{
		return GetF() * (max - min) + min;
	}

	double Random::GetD()
	{
		return static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
	}
	double Random::GetD(double min, double max)
	{
		return GetD() * (max - min) + min;
	}
}