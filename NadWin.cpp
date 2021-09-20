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

	Image::Image(int width, int height) { Create(width, height); }
	Image::Image(std::string* FilePath) { LoadImage(*FilePath); }
	Image::Image(std::wstring* FilePath) { LoadImage(*FilePath); }

	Image::~Image()
	{
		if (hBitmap) DeleteObject(hBitmap);
		if (hdc) DeleteDC(hdc);
	}
	
	int Image::GetWidth()
	{
		return bitmap.bmWidth;
	}

	int Image::GetHeight()
	{
		return bitmap.bmHeight;
	}

	Size Image::GetSize()
	{
		return Size{ bitmap.bmWidth, bitmap.bmHeight };
	}

	Pixel Image::GetPixel(int x, int y)
	{
		unsigned char* data = reinterpret_cast<unsigned char*>(bitmap.bmBits);
		int i = 3 * x + (y * bitmap.bmWidthBytes);
		return Pixel{ data[i + 2], data[i + 1], data[i] };
	}

	void Image::SetPixel(int x, int y, Pixel* pixel)
	{
		unsigned char* data = reinterpret_cast<unsigned char*>(bitmap.bmBits);
		int i = 3 * x + (y * bitmap.bmWidthBytes);

		data[i] = pixel->b;
		data[i + 1] = pixel->g;
		data[i + 2] = pixel->r;
	}

	void Image::SetPixel(int x, int y, Pixel pixel)
	{
		SetPixel(x, y, &pixel);
	}

	void Image::SetPixel(int x, int y, NW::Pixel* pixel, float opacity)
	{
		NW::Pixel newPixel;
		NW::Pixel oldPixel = GetPixel(x, y);
		float rOpacity = 1.0f - opacity;
		newPixel.r = (pixel->r * opacity) + (oldPixel.r * rOpacity);
		newPixel.g = (pixel->g * opacity) + (oldPixel.g * rOpacity);
		newPixel.b = (pixel->b * opacity) + (oldPixel.b * rOpacity);

		SetPixel(x, y, &newPixel);
	}

	void Image::SwapPixel(int x1, int y1, int x2, int y2)
	{
		Pixel pixel1 = GetPixel(x1, y1);
		Pixel pixel2 = GetPixel(x2, y2);

		SetPixel(x2, y2, &pixel1);
		SetPixel(x1, y1, &pixel2);
	}

	void Image::DrawLineI(int x0, int y0, int x1, int y1, NW::Pixel* pixel)
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

	void Image::Resize(int width, int height)
	{
		Delete();
		Create(width, height);
	}

	void Image::ResizeWithContent(int width, int height)
	{
		// Stwórz memDC i wklej tam cokolwiek teraz jest
		HDC memDC = CreateCompatibleDC(hdc);
		HBITMAP memBitmap = CreateCompatibleBitmap(hdc, width, height);
		SelectObject(memDC, memBitmap);
		BitBlt(memDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdc, 0, 0, SRCCOPY);

		// Zmieñ rozmiar
		Resize(width, height);

		// Wklej z memDC i usuñ je
		BitBlt(hdc, 0, 0, width, height, memDC, 0, 0, SRCCOPY);
		DeleteObject(memBitmap);
		DeleteDC(memDC);
	}
	
	void Image::Stretch(int width, int height, int quality)
	{
		// Stwórz memDC i wklej tam cokolwiek teraz jest
		HDC memDC = CreateCompatibleDC(hdc);
		HBITMAP memBitmap = CreateCompatibleBitmap(hdc, bitmap.bmWidth, bitmap.bmHeight);
		SelectObject(memDC, memBitmap);
		BitBlt(memDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdc, 0, 0, SRCCOPY);

		// Pobieranie starego rozmiaru przed zmian¹ rozmiaru
		int oldWidth = bitmap.bmWidth;
		int oldHeight = bitmap.bmHeight;

		// Zmieñ rozmiar
		Resize(width, height);

		// Wklej z memDC i usuñ je
		int StretchBltPreviousMode = GetStretchBltMode(hdc);
		if (quality)
		{
			SetBrushOrgEx(hdc, quality, quality, 0);
			SetStretchBltMode(hdc, HALFTONE);
		}
		StretchBlt(hdc, 0, 0, width, height, memDC, 0, 0, oldWidth, oldHeight, SRCCOPY);
		SetStretchBltMode(hdc, StretchBltPreviousMode);
		DeleteObject(memBitmap);
		DeleteDC(memDC);
	}

	const HDC Image::GetHDC()
	{
		return hdc;
	}

	const HBITMAP Image::GetHBitmap()
	{
		return hBitmap;
	}

	//
	// private:
	//

	void Image::Create(int width, int height)
	{
		// Memory DC
		HDC desktopDC = GetDC(NULL);
		hdc = CreateCompatibleDC(desktopDC);

		BITMAPINFO bmi;
		memset(&bmi, 0, sizeof(BITMAPINFO));
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = width;
		bmi.bmiHeader.biHeight = -height;
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 24;
		bmi.bmiHeader.biCompression = BI_RGB;

		// Cokolwiek to robi
		hBitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&bitmap.bmBits, NULL, NULL);
		if (!hBitmap || !bitmap.bmBits) throw std::exception("Nie uda³o siê stworzyæ sekcji DIB");

		bitmap.bmBitsPixel = 24;
		bitmap.bmWidth = bmi.bmiHeader.biWidth;
		bitmap.bmHeight = -bmi.bmiHeader.biHeight;
		bitmap.bmWidthBytes = bmi.bmiHeader.biWidth * 3;
		bitmap.bmPlanes = 1;

		ReleaseDC(NULL, desktopDC);
		SelectObject(hdc, hBitmap);
	}

	void Image::LoadImage(std::string& FilePath)
	{
		HDC desktopHdc = GetDC(GetDesktopWindow());
		HDC imageDC = CreateCompatibleDC(desktopHdc);
		ReleaseDC(GetDesktopWindow(), desktopHdc);

		HBITMAP imageHBitmap = (HBITMAP)LoadImageA(0, FilePath.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		SelectObject(imageDC, imageHBitmap);

		BITMAP bm;
		GetObject(imageHBitmap, sizeof(BITMAP), &bm);
		
		Create(bm.bmWidth, bm.bmHeight);
		BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, imageDC, 0, 0, SRCCOPY);

		DeleteObject(imageHBitmap);
		DeleteDC(imageDC);
	}

	void Image::LoadImage(std::wstring& FilePath)
	{
		HDC desktopHdc = GetDC(GetDesktopWindow());
		HDC imageDC = CreateCompatibleDC(desktopHdc);
		ReleaseDC(GetDesktopWindow(), desktopHdc);

		HBITMAP imageHBitmap = (HBITMAP)LoadImageW(0, FilePath.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		SelectObject(imageDC, imageHBitmap);

		BITMAP bm;
		GetObject(imageHBitmap, sizeof(BITMAP), &bm);

		Create(bm.bmWidth, bm.bmHeight);
		BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, imageDC, 0, 0, SRCCOPY);

		DeleteObject(imageHBitmap);
		DeleteDC(imageDC);
	}

	void Image::Delete()
	{
		if (hBitmap) DeleteObject(hBitmap);
		if (hdc) DeleteDC(hdc);
	}
	
	//
	//
	// class Position
	//
	//

	//
	// public:
	//

	Position::Position(int x, int y, int width, int height)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}

	void Position::operator=(RECT rect)
	{
		x = rect.left;
		y = rect.top;
		width = rect.right - rect.left;
		height = rect.bottom - rect.top;
	}

	Position::operator RECT()
	{
		RECT ret;
		ret.left = x;
		ret.top = y;
		ret.right = x + width;
		ret.bottom = y + height;

		return ret;
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
	{
		this->left = left;
		this->top = top;
		this->right = right;
		this->bottom = bottom;
	}

	//
	//
	// class Border
	//
	//

	//
	// public:
	//

	Border::Border(COLORREF color, int left, int top, int right, int bottom)
	{
		this->color = color;
		this->left = left;
		this->top = top;
		this->right = right;
		this->bottom = bottom;
	}

	//
	// private:
	//

	bool Border::NeedsDrawing()
	{
		return (left || top || right || bottom);
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
			srand(time(0));
			first = false;
		}
	}

	int Random::Get()
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