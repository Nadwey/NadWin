#pragma once

#ifndef NADWIN_LINUX_HPP
#define NADWIN_LINUX_HPP

#ifdef __linux__

#include <string>
#include <codecvt>
#include <locale>
#include <stdexcept>
#include <sstream>

#include "NadWin.hpp"

namespace NW {
	bool NApp::initialized = false;
	std::string NApp::appName = "";
	Display* NApp::display = nullptr;
	int NApp::screen = 0;

	NApp::NApp(std::string appName)
	{
		display = XOpenDisplay(nullptr);
		if (!display) throw std::runtime_error("Failed to open display");

		screen = DefaultScreen(display);

		initialized = true;
	}

	NApp::~NApp()
	{
		if (!initialized) return;
		XCloseDisplay(display);
		initialized = false;
	}

	int NApp::MessageLoop()
	{
		while (true) {
			XEvent e;
			XNextEvent(display, &e);
		}

		return 0;
	}

	void NApp::proc()
	{

	}

	NWindow::NWindow(std::string windowName, int x, int y, int width, int height)
	{
		windowHandle = XCreateSimpleWindow(NApp::display, RootWindow(NApp::display, NApp::screen), x, y, width, height, 1, BlackPixel(NApp::display, NApp::screen), WhitePixel(NApp::display, NApp::screen));
		XStoreName(NApp::display, windowHandle, windowName.c_str());
	}

	NWindow::~NWindow()
	{
		XDestroyWindow(NApp::display, windowHandle);
		windowHandle = 0;
	}

	void NWindow::Show()
	{
		XSelectInput(NApp::display, windowHandle, ExposureMask | KeyPressMask);
		XMapWindow(NApp::display, windowHandle);
	}
}

#endif // __linux__

#endif // !NADWIN_LINUX_HPP
