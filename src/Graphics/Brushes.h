#pragma once

#ifndef NW_GRAPHICS_BRUSHES
#define NW_GRAPHICS_BRUSHES

#include <Windows.h>

namespace NW {
	struct Rgb {
		unsigned char r;
		unsigned char g;
		unsigned char b;

		COLORREF ToColorRef();
		void FromColorRef(COLORREF colorRef);
	};

	namespace Brushes {
		class SolidBrush {
		public:
			SolidBrush(Rgb color);
			~SolidBrush();

			void SetColor(Rgb color);
			Rgb GetColor();

			const HBRUSH const GetBrush();

		private:
			void update();

			HBRUSH brush = nullptr;
			Rgb color = { 0 };
		};
	}
}

#endif // !NW_GRAPHICS_BRUSHES