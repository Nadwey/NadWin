#include "NadWin.h"

namespace NW
{
	//
	// 
	// class Image
	// 
	//

	//
	// public:
	//

	Bitmap::Bitmap(int width, int height) { Create(width, height); }
	Bitmap::Bitmap(std::string* FilePath) { LoadImage(*FilePath); }
	Bitmap::Bitmap(std::wstring* FilePath) { LoadImage(*FilePath); }

	Bitmap::~Bitmap()
	{
		if (hBitmap) DeleteObject(hBitmap);
		if (hdc) DeleteDC(hdc);
	}
	
	int Bitmap::GetWidth()
	{
		return bitmap.bmWidth;
	}

	int Bitmap::GetHeight()
	{
		return bitmap.bmHeight;
	}

	Size Bitmap::GetSize()
	{
		return Size{ bitmap.bmWidth, bitmap.bmHeight };
	}

	Pixel Bitmap::GetPixel(int x, int y)
	{
		PixelBGR& pixelref = GetPixelRef(x, y);
		return Pixel{ pixelref.r, pixelref.g, pixelref.b };
	}

	PixelBGR& Bitmap::GetPixelRef(int x, int y)
	{
		unsigned char* data = reinterpret_cast<unsigned char*>(bitmap.bmBits);
		int i = (3 * x) + (y * (bitmap.bmWidthBytes + widthPadding));
		return *reinterpret_cast<PixelBGR*>(data + i);
	}

	void Bitmap::SetPixel(int x, int y, Pixel* pixel)
	{
		PixelBGR& pixelref = GetPixelRef(x, y);
		pixelref.r = pixel->r;
		pixelref.g = pixel->g;
		pixelref.b = pixel->b;
	}

	void Bitmap::SetPixel(int x, int y, Pixel* pixel, float opacity)
	{
		PixelBGR& pixelRef = GetPixelRef(x, y);
		float rOpacity = 1.0f - opacity;
		pixelRef.r = (pixel->r * opacity) + (pixelRef.r * rOpacity);
		pixelRef.g = (pixel->g * opacity) + (pixelRef.g * rOpacity);
		pixelRef.b = (pixel->b * opacity) + (pixelRef.b * rOpacity);
	}

	void Bitmap::SwapPixel(int x0, int y0, int x1, int y1)
	{
		Pixel pixel1 = GetPixel(x0, y0);
		Pixel pixel2 = GetPixel(x1, y1);

		SetPixel(x1, y1, &pixel1);
		SetPixel(x0, y0, &pixel2);
	}

	Point Bitmap::ClipPixel(int x, int y)
	{
		if (x > bitmap.bmWidth - 1) x = bitmap.bmWidth - 1;
		else if (x < 0) x = 0;

		if (y > bitmap.bmHeight - 1) y = bitmap.bmHeight - 1;
		else if (y < 0) y = 0;
		
		return Point{ x, y };
	}

	bool Bitmap::IsValidPixel(int x, int y)
	{
		return (x < 0 || x > bitmap.bmWidth - 1 || y < 0 || y > bitmap.bmHeight - 1);
	}

	void Bitmap::DrawLineI(int x0, int y0, int x1, int y1, Pixel* pixel)
	{
		int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
		int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
		int err = dx + dy, e2;

		for (;;) {
			SetPixel(x0, y0, pixel);
			if (x0 == x1 && y0 == y1) break;
			e2 = 2 * err;
			if (e2 >= dy) { err += dy; x0 += sx; }
			if (e2 <= dx) { err += dx; y0 += sy; }
		}
	}

	void Bitmap::Resize(int width, int height)
	{
		Delete();
		Create(width, height);
	}

	void Bitmap::ResizeWithContent(int width, int height)
	{
		// Stwórz memDC i wklej tam zawartość
		HDC memDC = CreateCompatibleDC(hdc);
		HBITMAP memBitmap = CreateCompatibleBitmap(hdc, width, height);
		SelectObject(memDC, memBitmap);
		BitBlt(memDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdc, 0, 0, SRCCOPY);

		// Zmień rozmiar
		Resize(width, height);

		// Wklej zawartość z memDC i usuń je
		BitBlt(hdc, 0, 0, width, height, memDC, 0, 0, SRCCOPY);
		DeleteObject(memBitmap);
		DeleteDC(memDC);
	}
	
	
	void Bitmap::Stretch(int xDest, int yDest, int DestWidth, int DestHeight, int xSrc, int ySrc, int SrcWidth, int SrcHeight, int quality)
	{
		int StretchBltPreviousMode = GetStretchBltMode(hdc);
		if (quality)
		{
			SetBrushOrgEx(hdc, quality, quality, 0);
			SetStretchBltMode(hdc, HALFTONE);
		}

		BITMAPINFO bmi = GetBitmapInfo();
		bmi.bmiHeader.biHeight = -bmi.bmiHeader.biHeight;
		StretchDIBits(hdc, xDest, yDest, DestWidth, DestHeight, xSrc, ySrc, (SrcWidth == -1 ? bitmap.bmWidth : SrcWidth), (SrcHeight == -1 ? bitmap.bmHeight : SrcHeight), bitmap.bmBits, &bmi, DIB_RGB_COLORS, SRCCOPY);
	
		SetStretchBltMode(hdc, StretchBltPreviousMode);
	}

	void Bitmap::Stretch(int DestWidth, int DestHeight, int quality)
	{
		Stretch(0, 0, DestWidth, DestHeight, 0, 0, -1, -1, quality);
	}

	const HDC Bitmap::GetHDC()
	{
		return hdc;
	}

	const HBITMAP Bitmap::GetHBitmap()
	{
		return hBitmap;
	}

	//
	// private:
	//

	void Bitmap::Create(int width, int height)
	{
		// Memory DC
		HDC desktopDC = GetDC(0);
		hdc = CreateCompatibleDC(desktopDC);

		BITMAPINFO bmi = GetBitmapInfo();
		bmi.bmiHeader.biWidth = width;
		bmi.bmiHeader.biHeight = -height;
		bmi.bmiHeader.biBitCount = 24;
		bmi.bmiHeader.biPlanes = 1;

		// Cokolwiek to robi
		hBitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, reinterpret_cast<void**>(&bitmap.bmBits), 0, 0);
		if (!hBitmap || !bitmap.bmBits) throw std::exception("Nie udało się stworzyć sekcji DIB");

		bitmap.bmBitsPixel = 24;
		bitmap.bmWidth = bmi.bmiHeader.biWidth;
		bitmap.bmHeight = -bmi.bmiHeader.biHeight;
		bitmap.bmWidthBytes = bmi.bmiHeader.biWidth * 3;
		bitmap.bmPlanes = 1;
		CalculatePadding();

		ReleaseDC(0, desktopDC);
		SelectObject(hdc, hBitmap);
	}

	void Bitmap::LoadImage(std::string& FilePath)
	{
		throw std::exception("Not implemented");
	}

	void Bitmap::LoadImage(std::wstring& FilePath)
	{
		HDC desktopHdc = GetDC(GetDesktopWindow());
		HDC imageDC = CreateCompatibleDC(desktopHdc);
		ReleaseDC(GetDesktopWindow(), desktopHdc);

		HBITMAP imageHBitmap = reinterpret_cast<HBITMAP>(LoadImageW(0, FilePath.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION));
		SelectObject(imageDC, imageHBitmap);

		BITMAP bm;
		GetObject(imageHBitmap, sizeof(BITMAP), &bm);

		Create(bm.bmWidth, bm.bmHeight);
		BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, imageDC, 0, 0, SRCCOPY);

		DeleteObject(imageHBitmap);
		DeleteDC(imageDC);
	}

	void Bitmap::Delete()
	{
		if (hBitmap) DeleteObject(hBitmap);
		if (hdc) DeleteDC(hdc);
	}

	BITMAPINFO Bitmap::GetBitmapInfo()
	{
		BITMAPINFO bmi;
		memset(&bmi, 0, sizeof(BITMAPINFO));
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = bitmap.bmWidth;
		bmi.bmiHeader.biHeight = bitmap.bmHeight;
		bmi.bmiHeader.biPlanes = bitmap.bmPlanes;
		bmi.bmiHeader.biBitCount = bitmap.bmBitsPixel;
		bmi.bmiHeader.biCompression = BI_RGB;
		bmi.bmiHeader.biSizeImage = 0;
		return bmi;
	}

	void Bitmap::CalculatePadding()
	{
		if (bitmap.bmWidthBytes % 4 != 0) widthPadding = 4 - (bitmap.bmWidthBytes % 4);
	}

	namespace UI {
		void DoEvents()
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
			font = CreateFontW(height, width, 0, 0, 0, italic, underline, strike, 0, 0, 0, CLEARTYPE_QUALITY, 0, faceName.c_str());
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
		// class Padding
		//
		//

		//
		// public:
		//

		Padding::Padding(int left, int top, int right, int bottom)
			: left(left)
			, top(top)
			, right(right)
			, bottom(bottom)
		{

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

		//
		// private:
		//

		void App::registerClass(std::wstring& AppName)
		{
			static bool initialized = false;
			if (initialized) throw std::exception("Application class is arleady registered");
			hInstance = reinterpret_cast<HINSTANCE>(GetModuleHandle(nullptr));

			WNDCLASSEXW wcx{};
			wcx.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
			wcx.cbSize = sizeof(WNDCLASSEXW);
			wcx.lpfnWndProc = reinterpret_cast<WNDPROC>(proc);
			wcx.hInstance = hInstance;
			wcx.hCursor = LoadCursorW(nullptr, IDC_ARROW);
			wcx.hIcon = LoadIconW(nullptr, IDI_APPLICATION);
			wcx.lpszClassName = AppName.c_str();
			if (!RegisterClassExW(&wcx)) throw std::exception("Failed to register application class");
			this->AppName = AppName;
			Render::Renderer::Initialize();
			initialized = true;
		}

		LRESULT CALLBACK App::proc(HWND hwnd, UINT msg, LPARAM lParam, WPARAM wParam) 
		{
			Window* windowHandle = reinterpret_cast<Window*>(GetPropA(hwnd, "wClass"));
			LRESULT wProcRet = 0;
			if (windowHandle) wProcRet = windowHandle->proc(msg, lParam, wParam);
			switch (msg)
			{
			case WM_DESTROY:
			{
				windowCount--;
				if (windowCount <= 0)
				{
					Render::Renderer::Release();
					PostQuitMessage(0);
					return 0;
				}
				break;
			}
			default:
				break;
			}

			if (windowHandle) return wProcRet;
			return DefWindowProc(hwnd, msg, lParam, wParam);
		}

		std::wstring App::AppName = L"";
		HINSTANCE App::hInstance = nullptr;
		unsigned long App::windowCount = 0;

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
			if (renderer) delete renderer;
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
			controls.push_back(button);
		}

		void Window::Remove(Control* button)
		{
			controls.erase(std::remove(controls.begin(), controls.end(), button), controls.end());
		}

		void Window::ReRender()
		{
			InvalidateRect(hwnd, nullptr, false);
		}

		//
		// private:
		//

		void Window::createWindow(std::wstring& WindowName, int x, int y, int width, int height) 
		{
			hwnd = CreateWindowExW(0L, App::AppName.c_str(), WindowName.c_str(), static_cast<DWORD>(WindowStyles::OverlappedWindow), CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr, App::hInstance, nullptr);
			if (!hwnd) throw std::exception("Failed to create window");
			SetPropA(hwnd, "wClass", reinterpret_cast<HANDLE>(this));
			renderer = new Render::Renderer(hwnd);
			App::windowCount++;
		}

		LRESULT CALLBACK Window::proc(UINT msg, LPARAM lParam, WPARAM wParam)
		{
			switch (msg)
			{
			case WM_ERASEBKGND:
			{
				return 1;
			}
			case WM_SIZE:
			{
				RECT rc;
				GetClientRect(hwnd, &rc);
				renderer->Resize(&rc);
				break;
			}
			case WM_PAINT:
			{
				ControlRenderInfo controlRenderInfo;
				controlRenderInfo.hwnd = hwnd;
				RECT clientRect;
				GetClientRect(hwnd, &clientRect);
				controlRenderInfo.clientRect = &clientRect;
				controlRenderInfo.renderer = renderer;

				// tło
				renderer->BeginDraw();
				renderer->Clear(background);

				for (auto const& control : controls)
				{
					control->render(controlRenderInfo);
				}

				renderer->EndDraw();

				break;
			}
			default:
				break;
			}

			return DefWindowProc(hwnd, msg, lParam, wParam);
		}

		//
		//
		// class Control
		//
		//

		//
		// public:
		//

		Control::Control() : font(20, L"Segoe UI"), padding(0, 0, 0, 0), border(1, 1, 1, 1, 0x333333)
		{
			
		}

		//
		// protected:
		//

		void Control::render(ControlRenderInfo&)
		{
			throw std::exception("Can't render pure control class");
		}

		Render::Renderer* Control::GetWindowRenderer()
		{
			return window->renderer;
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
			if (background)
			{
				delete background;
				background = nullptr;
			}
		}

		void Button::initialize(Position& position, std::wstring& text)
		{
			background = reinterpret_cast<Render::Brush*>(new Render::SolidBrush(GetWindowRenderer(), Render::Rgb{ 0xea, 0xea, 0xea }));
			this->position = position;
			this->text = text;
		}

		void Button::render(ControlRenderInfo& CTI)
		{
			RECT rc = position.Rect();
			CTI.renderer->FillRect(&rc, background);
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