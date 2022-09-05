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
#else
#error NadWin Error: Unsupported OS
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

	bool NApp::initialized = false;
	std::string NApp::appName = "";
#ifdef _WIN32
	HINSTANCE NApp::hInstance = nullptr;
#elif defined(__linux__)
	Display* NApp::display = nullptr;
	int NApp::screen = 0;
#endif

	NApp::NApp(std::string appName)
	{
#ifdef _WIN32
		if (initialized) throw std::runtime_error("Application class is arleady registered");
		hInstance = static_cast<HINSTANCE>(GetModuleHandleA(nullptr));

		this->appName = appName;
		std::string windowClassName = getWindowClassName();

		WNDCLASSEXA wcx = { 0 };

		wcx.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wcx.cbSize = sizeof(WNDCLASSEXA);
		wcx.lpfnWndProc = static_cast<WNDPROC>(proc);
		wcx.hInstance = hInstance;
		wcx.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcx.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
		wcx.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));

		wcx.lpszClassName = windowClassName.c_str();

		if (!RegisterClassExA(&wcx)) throw std::runtime_error("Failed to register application class");
#elif defined(__linux__)
		display = XOpenDisplay(nullptr);
		if (!display) throw std::runtime_error("Failed to open display");

		screen = DefaultScreen(display);
#endif

		initialized = true;
	}

	NApp::~NApp()
	{
		if (!initialized) return;
#ifdef _WIN32
		std::string windowClassName = getWindowClassName();
		UnregisterClassA(windowClassName.c_str(), hInstance);
#elif defined(__linux__)
		XCloseDisplay(display);
#endif
		initialized = false;
	}

	int NApp::MessageLoop()
	{
#ifdef _WIN32
		MSG msg;
		while (::GetMessageA(&msg, nullptr, 0, 0) != 0)
		{
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
		return msg.wParam;
#elif defined(__linux__)
		while (true) {
			XEvent e;
			XNextEvent(display, &e);
		}

		return 0;
#endif
	}

#ifdef _WIN32
	std::string NApp::getWindowClassName()
	{
		std::stringstream ss;
		ss << appName << "-window";
		return ss.str();
	}
#endif


#ifdef _WIN32
	LRESULT CALLBACK NApp::proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return DefWindowProcA(hwnd, msg, wParam, lParam);
	}
#elif defined(__linux__)
	void NApp::proc()
	{

	}
#endif
	

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

		friend class NWindow;
	};

	NWindow::NWindow(std::string windowName, int x, int y, int width, int height)
	{
#ifdef _WIN32
		std::string windowClassName = NApp::getWindowClassName();
		windowHandle = CreateWindowExA(0L, windowClassName.c_str(), windowName.c_str(), WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, x, y, width, height, nullptr, nullptr, NApp::hInstance, nullptr);
#elif defined(__linux__) 
		windowHandle = XCreateSimpleWindow(NApp::display, RootWindow(NApp::display, NApp::screen), x, y, width, height, 1, BlackPixel(NApp::display, NApp::screen), WhitePixel(NApp::display, NApp::screen));
		XStoreName(NApp::display, windowHandle, windowName.c_str());
#endif
	}

	NWindow::~NWindow()
	{
#ifdef _WIN32
		DestroyWindow(windowHandle);
		windowHandle = nullptr;
#elif defined(__linux__) 
		XDestroyWindow(NApp::display, windowHandle);
		windowHandle = 0;
#endif
	}

	void NWindow::Show()
	{
#ifdef _WIN32
		ShowWindow(windowHandle, SW_SHOW);
		UpdateWindow(windowHandle);
#elif defined(__linux__) 
		XSelectInput(NApp::display, windowHandle, ExposureMask | KeyPressMask);
		XMapWindow(NApp::display, windowHandle);
#endif
	}
}

#endif // !NADWIN_HPP
