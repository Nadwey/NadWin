#pragma once

#ifndef NW_RENDERER
#define NW_RENDERER

#define NW_RENDERER_USE_DIRECT2D
#ifdef NW_RENDERER_USE_DIRECT2D
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#endif
#include <Windows.h>
#include <wingdi.h>
#include <iostream>
namespace NW
{
	typedef unsigned char byte;

	namespace Render {
		class Renderer;

		template<class Interface>
		inline void SafeRelease(Interface** ppInterfaceToRelease);

		struct Rgb {
			byte r = 0;
			byte g = 0;
			byte b = 0;

			COLORREF ColorRef()	{ return RGB(r, g, b); }
		};

		class Brush
		{
		public:
			

		protected:
			virtual void update();
			void clear();

			Renderer* renderer;
#ifdef NW_RENDERER_USE_DIRECT2D
			ID2D1Brush* brush;
#else
			HBRUSH brush;
#endif

			friend class Renderer;
		};

		class SolidBrush : Brush
		{
		public:
			SolidBrush(Renderer* renderer, Rgb color);
			~SolidBrush();

			void SetColor(Rgb color);
			Rgb GetColor();
#ifdef NW_RENDERER_USE_DIRECT2D
			const ID2D1SolidColorBrush* GetBrush();
#else
			const HBRUSH GetBrush();
#endif
		private:
			Rgb color;

			void update() override;

			friend class Renderer;
		};

		class Renderer
		{
		public:
			Renderer(HWND hwnd);
			~Renderer();

			void BeginDraw();
			void EndDraw();

			void Resize(RECT* rect);

			// Wywo³aj przy starcie programu
			static void Initialize();
			// Wywo³aj kiedy nie bêdziesz ju¿ u¿ywa³ tej klasy
			static void Release();

			void Clear(Rgb color);
			void DrawRect(RECT* rect, SolidBrush* brush, int strokeWidth);
			void DrawText();
			void FillRect(RECT* rect, Brush* brush);
		
		private:
#ifdef NW_RENDERER_USE_DIRECT2D
			static ID2D1Factory* pD2DFactory;
			ID2D1HwndRenderTarget* pRT = nullptr;
#else
			HDC hdc = nullptr;
			HDC dcMem = nullptr;
			HBITMAP bitmapMem = nullptr;
			PAINTSTRUCT ps = { 0 };
#endif
			RECT size;
			HWND hwnd;
			
			friend class Brush;
			friend class SolidBrush;
		};
	}
	
}



#endif // !NW_RENDERER