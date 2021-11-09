#include "Renderer.h"

namespace NW
{
	namespace Render {
#ifdef NW_RENDERER_USE_DIRECT2D
		D2D1_COLOR_F RgbToColorF(Rgb color)
		{
			return D2D1::ColorF::ColorF(static_cast<float>(color.r) / 255.0f, static_cast<float>(color.g) / 255.0f, static_cast<float>(color.b) / 255.0f);
		}

		D2D1_RECT_F RectToRectF(RECT* rect)
		{
			return D2D1::RectF(rect->left, rect->top, rect->right, rect->bottom);
		}
#endif

		template<class Interface>
		inline void SafeRelease(Interface** ppInterfaceToRelease)
		{
			if (*ppInterfaceToRelease != nullptr)
			{
				(*ppInterfaceToRelease)->Release();
				(*ppInterfaceToRelease) = nullptr;
			}
		}

		//
		//
		// class Brush
		//
		//

		//
		// protected:
		//

		void Brush::update()
		{
			throw std::exception("Can't update raw Brush class");
		}


		void Brush::clear() 
		{
			if (brush)
			{
#ifdef NW_RENDERER_USE_DIRECT2D
				brush->Release();
#else
				DeleteObject(brush);
#endif
				brush = nullptr;
			}
		}

		//
		//
		// class SolidBrush
		//
		//

		//
		// public:
		//

		void SolidBrush::SetColor(Rgb color)
		{
			this->color = color;
			update();
		}

		Rgb SolidBrush::GetColor()
		{
			return color;
		}

		SolidBrush::SolidBrush(Renderer* renderer, Rgb color)
		{
			this->renderer = renderer;
			this->color = color;
			update();
		}

		SolidBrush::~SolidBrush()
		{
			clear();
		}

		

#ifdef NW_RENDERER_USE_DIRECT2D
		const ID2D1SolidColorBrush* SolidBrush::GetBrush()
		{
			return reinterpret_cast<ID2D1SolidColorBrush*>(brush);
		}
#else
		const HBRUSH SolidBrush::GetBrush()
		{
			return brush;
		}
#endif

		//
		// private:
		//

		void SolidBrush::update()
		{
			clear();
#ifdef NW_RENDERER_USE_DIRECT2D
			renderer->pRT->CreateSolidColorBrush(RgbToColorF(color), reinterpret_cast<ID2D1SolidColorBrush**>(&brush));
#else
			brush = CreateSolidBrush(color.ColorRef());
#endif
		}

		//
		//
		// class Renderer
		//
		//

		//
		// public:
		//
		
		Renderer::Renderer(HWND hwnd) : hwnd(hwnd)
		{
#ifdef NW_RENDERER_USE_DIRECT2D
			RECT rc;
			GetClientRect(hwnd, &rc);

			pD2DFactory->CreateHwndRenderTarget(
				D2D1::RenderTargetProperties(),
				D2D1::HwndRenderTargetProperties(
					hwnd,
					D2D1::SizeU(rc.right - rc.left,	rc.bottom - rc.top)
				),
				&pRT
			);
#endif
		}

		Renderer::~Renderer()
		{
#ifdef NW_RENDERER_USE_DIRECT2D
			SafeRelease(&pRT);
#endif
		}

		void Renderer::BeginDraw()
		{
#ifdef NW_RENDERER_USE_DIRECT2D
			pRT->BeginDraw();
#else
			hdc = BeginPaint(hwnd, &ps);
			dcMem = CreateCompatibleDC(hdc);
			bitmapMem = CreateCompatibleBitmap(hdc, size.right - size.left, size.bottom - size.top);
			SelectObject(dcMem, bitmapMem);
#endif
		}

		void Renderer::EndDraw()
		{
#ifdef NW_RENDERER_USE_DIRECT2D
			pRT->EndDraw();
#else
			BitBlt(hdc, 0, 0, size.right - size.left, size.bottom - size.top, dcMem, 0, 0, SRCCOPY);

			DeleteObject(bitmapMem);
			DeleteDC(dcMem);
			EndPaint(hwnd, &ps);
			hdc = nullptr;
#endif
		}

		void Renderer::Resize(RECT* rect)
		{
			size = *rect;
#ifdef NW_RENDERER_USE_DIRECT2D
			pRT->Resize(D2D1::SizeU(size.right - size.left, size.bottom - size.top));
#endif
		}

		void Renderer::Initialize()
		{
#ifdef NW_RENDERER_USE_DIRECT2D
			std::cout << "Using Direct2D\n";
			HRESULT hr;
			if (!pD2DFactory)
				hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
#else
			std::cout << "Using WinGDI\n";
#endif
		}

		void Renderer::Release()
		{
#ifdef NW_RENDERER_USE_DIRECT2D
			SafeRelease(&pD2DFactory);
#endif
		}

		void Renderer::Clear(Rgb color)
		{
#ifdef NW_RENDERER_USE_DIRECT2D
			pRT->Clear(RgbToColorF(color));
#else
			HBRUSH brush = CreateSolidBrush(color.ColorRef());
			::FillRect(dcMem, &size, brush);
			DeleteObject(brush);
#endif
		}

		void Renderer::DrawRect(RECT* rect, SolidBrush* border,  int strokeWidth)
		{
#ifdef NW_RENDERER_USE_DIRECT2D
			D2D1_RECT_F rectF = RectToRectF(rect);
			pRT->DrawRectangle(rectF, border->brush, strokeWidth);
#else
			HPEN pen = CreatePen(PS_SOLID, strokeWidth, border->color.ColorRef());
			SelectObject(dcMem, pen);

			SelectObject(dcMem, GetStockObject(NULL_BRUSH));
			SetBkMode(dcMem, TRANSPARENT);

			

			Rectangle(dcMem, rect->left, rect->top, rect->right, rect->bottom);
			DeleteObject(pen);
#endif
		}

		void Renderer::FillRect(RECT* rect, Brush* brush)
		{
#ifdef NW_RENDERER_USE_DIRECT2D
			D2D1_RECT_F rectF = RectToRectF(rect);
			pRT->FillRectangle(rectF, brush->brush);
			
#else
			::FillRect(dcMem, rect, brush->brush);
#endif
		}

		//
		// private:
		//

#ifdef NW_RENDERER_USE_DIRECT2D
		ID2D1Factory* Renderer::pD2DFactory = nullptr;
#endif
	}
}