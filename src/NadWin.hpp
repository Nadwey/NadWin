#pragma once

#ifndef NADWIN_HPP
#define NADWIN_HPP

#include <string>
#include <codecvt>
#include <locale>
#include <stdexcept>
#include <sstream>

#ifdef _WIN32
#include <Windows.h>
#elif defined(__linux__)
#include <X11/Xlib.h>
#endif

namespace NW {
	/// @brief Application class.
	/// There should be only one instance for the whole application.
	class NApp {
	public:
		/// @brief Initializes application
		/// @param appName Application name.
		NApp(std::string appName);

		~NApp();

		/// @brief Enters application's message loop and exits when exit was requested
		int MessageLoop();
	private:
		static bool initialized;
		static std::string appName;

#ifdef _WIN32
		static HINSTANCE hInstance;
		static LRESULT CALLBACK proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		/// @brief Returns application name and "-window" appended
		/// used mostly for Windows
		static std::string getWindowClassName();
#elif defined(__linux__)
		static Display* display;
		static int screen;
		static void proc();
#endif

		friend class NWindow;
	};
	

	/// @brief Window class
	class NWindow {
	public:
		/// @brief Creates window but doesn't show it yet.
		/// to show the window use NWindow::Show()
		/// @param windowName Name of the window
		/// @param x x position of the window
		/// @param y y position of the window
		/// @param width width of the window
		/// @param height height of the window
		NWindow(std::string windowName, int x, int y, int width, int height);
		~NWindow();

		/// @brief Shows the window
		void Show();
	private:
#ifdef _WIN32
		HWND windowHandle = nullptr;
#elif defined(__linux__) 
		Window windowHandle = 0;
#endif

		friend class NApp;
	};
}

#endif // !NADWIN_HPP
