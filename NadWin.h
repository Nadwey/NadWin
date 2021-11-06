// https://github.com/Nadwey/NadWin

#pragma once
#ifndef _NADWIN_
#define _NADWIN_

#include <string>
#include <time.h>
#include <random>
#include <math.h>
#include <windows.h>
#include <functional>
#include <locale>
#include <codecvt>
#include <unordered_map>
#include <memory>
#include <vector>
#include "Renderer/Renderer.h"

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

	namespace UI {
		void DoEvents();

		enum class WindowStyles : DWORD {
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

		struct ControlRenderInfo {
			RECT* clientRect;
			HWND hwnd;
			HDC hdc;
		};
		
		class Brush 
		{
		public:
			Brush() = default;
			Brush(COLORREF color);
			~Brush();

			void SetColor(COLORREF color);
			COLORREF GetColor();

			const HBRUSH GetBrush();
		private:
			void update();

			HBRUSH brush = nullptr;
			COLORREF color = 0x000000;

		};

		class Font 
		{
		public:
			Font() = default;
			Font(int height, std::wstring faceName, int width = 0, bool italic = false, bool underline = false, bool strike = false);
			Font(int height, std::string faceName, int width = 0, bool italic = false, bool underline = false, bool strike = false);
			~Font();

			void SetHeight(int height);
			void SetFaceName(std::wstring faceName);
			void SetFaceName(std::string faceName);
			void SetWidth(int width);
			void SetItalic(bool italic);
			void SetUnderline(bool underline);
			void SetStrike(bool strike);

			int GetHeight();
			std::wstring GetFaceName();
			int GetWidth();
			bool GetItalic();
			bool GetUnderline();
			bool GetStrike();

			const HFONT GetFont();
		private:
			void update();

			HFONT font;
			int height;
			std::wstring faceName;
			int width;
			bool italic;
			bool underline;
			bool strike;

			friend class Control;
		};

		class Position {
		public:
			Position() = default;
			Position(int x, int y, int width, int height);
			Position(RECT rect);
			Position(RECT* rect);

			RECT Rect();
			void FromRect(RECT rect);
			void FromRect(RECT* rect);

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

		class Border
		{
		public:
			Border() = default;
			Border(int left, int top, int right, int bottom, COLORREF color);

			bool NeedsRender();

			int left = 0;
			int top = 0;
			int right = 0;
			int bottom = 0;
			Brush color;
		};

		// W�a�ciwe UI

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

		
		// Definicja definicji ;-;
		class Control;
		class Button;

		class Window {
		public:
			Window(std::wstring WindowName, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int width = CW_USEDEFAULT, int height = CW_USEDEFAULT);
			Window(std::string WindowName, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int width = CW_USEDEFAULT, int height = CW_USEDEFAULT);
			~Window();

			const HWND Hwnd();
			void Move(int x, int y, int width, int height, bool repaint = true);
			void Show();

			// Control gets copied by reference!
			void Add(Control* control);
			void Remove(Control* button);

			void ReRender();

			Brush background;
		private:
			void createWindow(std::wstring& WindowName, int x, int y, int width, int height);
			LRESULT CALLBACK proc(UINT msg, LPARAM lParam, WPARAM wParam);

			std::vector<Control*> controls;
			HWND hwnd = nullptr;

			friend class App;
			friend class Control;
		};

		// Controls

		class Control
		{
		public:
			Control();
			struct
			{
				std::function<void()> OnClick;
			} Events;
			
			Brush background;
			Border border;
			Font font;
			COLORREF foregroundColor = 0x000000;
			Padding padding;
			Position position;
		protected:
			virtual void render(ControlRenderInfo& controlRenderInfo);

		private:
			friend class Window;
		};

		class Button : public Control
		{
		public:
			Button(Position position, std::string text);
			Button(Position position, std::wstring text);
			~Button();

		private:
			void initialize(Position& position, std::wstring& text);
			void render(ControlRenderInfo& controlRenderInfo) override;

			std::wstring text;

			friend class Window;
		};
	}

	// Klasa wywo�uje srand! (Tylko raz na wszystkie instancje)
	class Random
	{
	public:
		Random();

		// Dos�ownie rand()
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