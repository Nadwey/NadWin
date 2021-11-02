// https://github.com/Nadwey/NadWin

#pragma once
#ifndef _NADWIN_
#define _NADWIN_

#include <string>
#include <time.h>
#include <random>
#include <math.h>
#include <windows.h>
#include <locale>
#include <codecvt>

#undef LoadImage

namespace NW
{
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
		// Tworzy DC na którym mo¿esz malowaæ
		Bitmap(int width, int height);

		// Tworzy DC + ³aduje do niego bitmape
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
		

		// Rysowanie linii u¿ywaj¹c algorytmu Bresenhama
		void DrawLineI(int x0, int y0, int x1, int y1, Pixel* pixel);

		

		// Zmienia rozmiar bez zawartoœci (resetuje)
		void Resize(int width, int height);
		// Zmienia rozmiar z zawartoœci¹ (kopiuje zawartoœæ, resetuje, wkleja kopie)
		void ResizeWithContent(int width, int height);
		/// <summary>
		/// Je¿eli parametr quality nie jest zerem StretchBlt mode jest ustawiane na HALFTONE (potem zmieniane na poprzednie)
		/// Je¿eli argumenty SrcWidth lub SrcHeight bêd¹ równaæ siê -1 to metoda zast¹pi je wymiarami obrazu
		/// </summary>
		void Stretch(int xDest, int yDest, int DestWidth, int DestHeight, int xSrc, int ySrc, int SrcWidth = -1, int SrcHeight = -1, int quality = 0);
		// Je¿eli parametr quality nie jest zerem StretchBlt mode jest ustawiane na HALFTONE (potem zmieniane na poprzednie)
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

	class Image {
	public:
		Image(std::string path);
		Image(std::string* path);
	};

	namespace UI {
		enum WindowStyles {
			Border = WS_BORDER,
			Caption = WS_CAPTION,
			Child = WS_CHILD,
			ChildWindow = WS_CHILDWINDOW,
			ClipChildren = WS_CLIPCHILDREN,
			ClipSiblings = WS_CLIPSIBLINGS,
			Disabled = WS_DISABLED,
			DlgFrame = WS_DLGFRAME,
			Group = WS_GROUP,
			HScroll = WS_HSCROLL,
			Iconic = WS_ICONIC,
			Maximize = WS_MAXIMIZE,
			MaximizeBox = WS_MAXIMIZEBOX,
			Minimize = WS_MINIMIZE,
			MinimizeBox = WS_MINIMIZEBOX,
			Overlapped = WS_OVERLAPPED,
			OverlappedWindow = WS_OVERLAPPEDWINDOW,
			Popup = WS_POPUP,
			PopupWindow = WS_POPUPWINDOW,
			SizeBox = WS_SIZEBOX,
			SysMenu = WS_SYSMENU,
			TabStop = WS_TABSTOP,
			ThickFrame = WS_THICKFRAME,
			Tiled = WS_TILED,
			TiledWindow = WS_TILEDWINDOW,
			Visible = WS_VISIBLE,
			VScroll = WS_VSCROLL
		};

		class App {
		public:
			App(std::wstring AppName);
			App(std::string AppName);
			~App();
			
			int MessageLoop();

		private:
			void registerClass(std::wstring&);
			static LRESULT CALLBACK proc(HWND, UINT, LPARAM, WPARAM);

			static std::wstring AppName;
			static HINSTANCE hInstance;
			static unsigned long windowCount;

			friend class Window;
		};

		class Window {
		public:
			Window(std::wstring WindowName, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int width = CW_USEDEFAULT, int height = CW_USEDEFAULT);
			Window(std::string WindowName, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int width = CW_USEDEFAULT, int height = CW_USEDEFAULT);
			~Window();

			void Show();

		private:
			void createWindow(std::wstring& WindowName, int x, int y, int width, int height);

			HWND hwnd = nullptr;

			friend class App;
		};
	}

	// Klasa wywo³uje srand! (Tylko raz na wszystkie instancje)
	class Random
	{
	public:
		Random();

		// Dos³ownie rand()
		inline int Get();
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