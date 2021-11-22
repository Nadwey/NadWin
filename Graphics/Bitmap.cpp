#include "Bitmap.h"

namespace NW {
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
		// Stw�rz memDC i wklej tam zawarto��
		HDC memDC = CreateCompatibleDC(hdc);
		HBITMAP memBitmap = CreateCompatibleBitmap(hdc, width, height);
		SelectObject(memDC, memBitmap);
		BitBlt(memDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdc, 0, 0, SRCCOPY);

		// Zmie� rozmiar
		Resize(width, height);

		// Wklej zawarto�� z memDC i usu� je
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
		if (!hBitmap || !bitmap.bmBits) throw std::exception("Nie uda�o si� stworzy� sekcji DIB");

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
}