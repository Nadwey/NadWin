#pragma once

#ifndef NADWIN_WIN_CPP
#define NADWIN_WIN_CPP

#ifdef _WIN32

#include <string>
#include <codecvt>
#include <locale>
#include <stdexcept>
#include <sstream>

#include "NadWin.hpp"

namespace NW {
	bool NApp::initialized = false;
	std::string NApp::appName = "";
	HINSTANCE NApp::hInstance = nullptr;

	NApp::NApp(std::string appName)
	{
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

		initialized = true;
	}

	NApp::~NApp()
	{
		if (!initialized) return;
		std::string windowClassName = getWindowClassName();
		UnregisterClassA(windowClassName.c_str(), hInstance);
		initialized = false;
	}

	int NApp::MessageLoop()
	{
		MSG msg;
		while (::GetMessageA(&msg, nullptr, 0, 0) != 0)
		{
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
		return msg.wParam;
	}

	std::string NApp::getWindowClassName()
	{
		std::stringstream ss;
		ss << appName << "-window";
		return ss.str();
	}

	LRESULT CALLBACK NApp::proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return DefWindowProcA(hwnd, msg, wParam, lParam);
	}

	NWindow::NWindow(std::string windowName, int x, int y, int width, int height)
	{
		std::string windowClassName = NApp::getWindowClassName();
		windowHandle = CreateWindowExA(0L, windowClassName.c_str(), windowName.c_str(), WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, x, y, width, height, nullptr, nullptr, NApp::hInstance, nullptr);
	}

	NWindow::~NWindow()
	{
		DestroyWindow(windowHandle);
		windowHandle = nullptr;
	}

	void NWindow::Show()
	{
		ShowWindow(windowHandle, SW_SHOW);
		UpdateWindow(windowHandle);
	}
}

#endif // _WIN32

#endif // !NADWIN_WIN_CPP
