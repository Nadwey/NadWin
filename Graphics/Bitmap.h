#pragma once

#ifndef NW_GRAPHICS_BITMAP
#define NW_GRAPHICS_BITMAP

#include <windows.h>
#include <string>

#undef LoadImage

namespace NW {
	struct Size
	{
		int width;
		int height;
	};

	struct Pixel
	{
		unsigned char r;
		unsigned char g;
		unsigned char b;
	};

	struct PixelBGR
	{
		unsigned char b;
		unsigned char g;
		unsigned char r;
	};

	struct Point
	{
		int x;
		int y;
	};

	// GDI Bitmap (only RGB format supported)
	class Bitmap
	{
	public:
		// Tworzy DC na kt�rym mo�esz malowa�
		Bitmap(int width, int height);

		// Tworzy DC + �aduje do niego bitmape
		Bitmap(std::string* FilePath);
		Bitmap(std::wstring* FilePath);

		~Bitmap();

		// Rozmiar
		int	GetWidth();
		int	GetHeight();
		Size GetSize();


		// Funkcje do pixeli
		Pixel GetPixel(int x, int y);
		PixelBGR& GetPixelRef(int x, int y);
		void SetPixel(int x, int y, Pixel* pixel);
		void SetPixel(int x, int y, Pixel* pixel, float opacity);
		void SwapPixel(int x0, int y0, int x1, int y1);
		Point ClipPixel(int x, int y);
		// Sprawdza czy pixel jest w obrazie
		bool IsValidPixel(int x, int y);


		// Rysowanie linii u�ywaj�c algorytmu Bresenhama
		void DrawLineI(int x0, int y0, int x1, int y1, Pixel* pixel);



		// Zmienia rozmiar bez zawarto�ci (resetuje)
		void Resize(int width, int height);
		// Zmienia rozmiar z zawarto�ci� (kopiuje zawarto��, resetuje, wkleja kopie)
		void ResizeWithContent(int width, int height);
		/// <summary>
		/// Je�eli parametr quality nie jest zerem StretchBlt mode jest ustawiane na HALFTONE (potem zmieniane na poprzednie)
		/// Je�eli argumenty SrcWidth lub SrcHeight b�d� r�wna� si� -1 to metoda zast�pi je wymiarami obrazu
		/// </summary>
		void Stretch(int xDest, int yDest, int DestWidth, int DestHeight, int xSrc, int ySrc, int SrcWidth = -1, int SrcHeight = -1, int quality = 0);
		// Je�eli parametr quality nie jest zerem StretchBlt mode jest ustawiane na HALFTONE (potem zmieniane na poprzednie)
		void Stretch(int DestWidth, int DestHeight, int quality = 0);


		const HDC GetHDC();
		const HBITMAP GetHBitmap();
	private:
		void Create(int width, int height);
		void LoadImage(std::string& FilePath);
		void LoadImage(std::wstring& FilePath);
		void Delete();
		BITMAPINFO GetBitmapInfo();
		void CalculatePadding();

		HDC hdc;
		HBITMAP hBitmap;
		BITMAP bitmap;
		int widthPadding = 0;
	};
}

#endif // !NW_GRAPHICS_BITMAP