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
		std::wstring s2ws(std::string s)
		{
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			std::wstring ws = converter.from_bytes(s);
			return ws;
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
			wcx.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW);
			wcx.hCursor = LoadCursorW(nullptr, IDC_ARROW);
			wcx.hIcon = LoadIconW(nullptr, IDI_APPLICATION);
			wcx.lpszClassName = AppName.c_str();
			if (!RegisterClassExW(&wcx)) throw std::exception("Failed to register application class");
			this->AppName = AppName;
			initialized = true;
		}

		LRESULT CALLBACK App::proc(HWND hwnd, UINT msg, LPARAM lParam, WPARAM wParam) 
		{
			switch (msg)
			{
			case WM_DESTROY:
			{
				windowCount--;
				if (windowCount <= 0)
				{
					PostQuitMessage(0);
					return 0;
				}
				break;
			}
			default:
				break;
			}

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
			DestroyWindow(hwnd);
		}

		void Window::Show()
		{
			ShowWindow(hwnd, SW_SHOW);
			UpdateWindow(hwnd);
		}

		//
		// private:
		//

		void Window::createWindow(std::wstring& WindowName, int x, int y, int width, int height) {
			hwnd = CreateWindowExW(0L, App::AppName.c_str(), WindowName.c_str(), WindowStyles::OverlappedWindow, CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr, App::hInstance, nullptr);
			if (!hwnd) throw std::exception("Failed to create window");
			App::windowCount++;
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