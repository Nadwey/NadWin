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
#include <vector>
#include <utility>
#include <commctrl.h>
#include "Graphics/Brushes.h"

namespace NW
{
	namespace UI {
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
		};

		// W�a�ciwe UI

		class App {
		public:
			App(std::wstring AppName);
			App(std::string AppName);
			~App();

			int MessageLoop();
			std::wstring GetAppName();

			static void DoEvents();
		private:
			void registerClass(std::wstring&);
			static LRESULT CALLBACK proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

			static std::wstring AppName;
			static HINSTANCE hInstance;
			static unsigned long windowCount;
			static bool initialized;

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

			void Add(Control* control);

			void Repaint();
			void* customData;
		private:
			void createWindow(std::wstring& WindowName, int x, int y, int width, int height);
			LRESULT CALLBACK proc(UINT msg, WPARAM wParam, LPARAM lParam);

			HWND hwnd = nullptr;

			friend class App;
			friend class Control;
		};

		enum class EventTypes
		{
			Undefined = 0,
			MouseLeftDoubleClick,
			MouseLeftDown,
			MouseLeftUp,
			MouseMiddleDoubleClick,
			MouseMiddleDown,
			MouseMiddleUp,
			MouseRightDoubleClick,
			MouseRightDown,
			MouseRightUp,
			MouseOver,
			MouseLeave
		};

		// Controls

		class Control
		{
		public:
			std::function<void(EventTypes, Control*, void*)> EventHandler;

			void Repaint();

			Font font;
			void UpdateFont();

			Position GetPosition();
			void SetPosition(Position position);

			std::wstring GetText();
			void SetText(std::string text);
			void SetText(std::wstring text);

			void Focus();
			void RemoveFocus();
		protected:
			Control();

			// (parent)
			Window* window = nullptr;
			HWND hwnd = nullptr;
			bool isOver = false;
			std::wstring text;
			Position position;

			virtual void create();
			void updateText();

			static LRESULT CALLBACK ControlProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR);
		private:
			friend class Window;
		};

		class Button : public Control
		{
		public:
			Button(Window* window, Position position, std::string text);
			Button(Window* window, Position position, std::wstring text);
			~Button();

		private:
			void initialize(Position& position, std::wstring& text);
			void create() override;

			friend class Window;
		};

		class Static : public Control
		{
		public:
			Static(Window* window, Position position, std::string text);
			Static(Window* window, Position position, std::wstring text);
			~Static();

		private:
			void initialize(Position& position, std::wstring& text);
			void create() override;

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