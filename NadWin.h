// https://github.com/Nadwey/NadWin

#pragma once
#ifndef _NADWIN_
#define _NADWIN_

#include <string>
#include <time.h>
#include <random>
#include <windows.h>

#undef LoadImage

namespace NW
{
	struct Size
	{
		int width;
		int height;
	};

	struct Pixel {
		unsigned char r;
		unsigned char g;
		unsigned char b;
	};

	class Image
	{
	public:
		// Tworzy DC na którym mo¿esz malowaæ
		Image(int width, int height);

		// Tworzy DC + ³aduje do niego bitmape
		Image(std::string* FilePath);
		Image(std::wstring* FilePath);

		~Image();

		// Rozmiar
		int	GetWidth();
		int	GetHeight();
		Size GetSize();
		

		// Funkcje do pixeli
		Pixel GetPixel(int x, int y);
		void SetPixel(int x, int y, Pixel* pixel);
		void SetPixel(int x, int y, Pixel pixel);
		void SetPixel(int x, int y, Pixel* pixel, float opacity);
		void SwapPixel(int x0, int y0, int x1, int y1);

		// Rysowanie linii u¿ywaj¹c algorytmu Bresenhama
		void DrawLineI(int x0, int y0, int x1, int y1, Pixel* pixel);

		

		// Zmienia rozmiar bez zawartoœci (mo¿na u¿yæ do hard resetu)
		void Resize(int width, int height);
		// Zmienia rozmiar z zawartoœci¹
		void ResizeWithContent(int width, int height);
		/// <summary>
		/// Zmienia rozmiar z zawartoœci¹ + rozci¹ga
		/// Je¿eli parametr quality nie jest zerem StretchBlt mode jest ustawiane na HALFTONE (potem zmieniane na poprzednie)
		/// </summary>
		void Stretch(int width, int height, int quality = 0);


		const HDC GetHDC();
		const HBITMAP GetHBitmap();
	private:
		void Create(int width, int height);
		void LoadImage(std::string& FilePath);
		void LoadImage(std::wstring& FilePath);
		void Delete();
		
		HDC hdc;
		HBITMAP hBitmap;
		BITMAP bitmap;
	};

	class Position
	{
	public:
		Position() = default;
		Position(int x, int y, int width, int height);

		void operator=(RECT rect);
		operator RECT();

		int x = 0;
		int y = 0;
		int width = 0;
		int height = 0;
	};

	class Padding
	{
	public:
		Padding() = default;
		Padding(int left, int top, int right, int bottom);

		int left = 0;
		int top = 0;
		int right = 0;
		int bottom = 0;
	};

	class Border : public Padding
	{
	public:
		Border() = default;
		Border(COLORREF color, int left, int top, int right, int bottom);

		COLORREF color = 0;
	private:
		bool NeedsDrawing();
	};

	// Klasa wywo³uje srand! (Tylko raz na wszystkie instancje)
	class Random
	{
	public:
		Random();

		// Dos³ownie rand()
		int Get();
		int Get(int min, int max);

		// Zwraca losowy float w zakresie 0 - 1
		float GetF();
		float GetF(float min, float max);

		// Zwraca losowe double w zakresie 0 - 1
		double GetD();
		double GetD(double min, double max);
	};
}


#endif // !_NADWIN_