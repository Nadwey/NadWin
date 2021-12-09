#include "Brushes.h"

namespace NW {
	//
	//
	// struct Rgb
	//
	//

	COLORREF Rgb::ToColorRef()
	{
		return RGB(r, g, b);
	}

	void Rgb::FromColorRef(COLORREF colorRef)
	{
		r = GetRValue(colorRef);
		g = GetGValue(colorRef);
		b = GetBValue(colorRef);
	}

	namespace Brushes {
		//
		//
		// class SolidBrush
		//
		//

		//
		// public:
		//

		SolidBrush::SolidBrush(Rgb color)
		{
			SetColor(color);
		}

		SolidBrush::~SolidBrush()
		{
			if (brush)
			{
				DeleteObject(brush);
				brush = nullptr;
			}
		}

		void SolidBrush::SetColor(Rgb color)
		{
			this->color = color;
			update();
		}

		Rgb SolidBrush::GetColor()
		{
			return color;
		}

		const HBRUSH SolidBrush::GetBrush()
		{
			return brush;
		}

		//
		// private:
		//

		void SolidBrush::update()
		{
			if (brush)
			{
				DeleteObject(brush);
				brush = nullptr;
			}
			brush = CreateSolidBrush(color.ToColorRef());
		}
	}
}