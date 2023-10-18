#pragma once

#ifndef NADWIN_HPP
#define NADWIN_HPP 1

#include <string>
#include <stdexcept>
#include <vector>

#ifdef _WIN32
#include <Windows.h>
#endif

#if _DEBUG
#define NADWIN_LOG(message) printf("[NadWin] - [%s:%d]: %s\n", __FILE__, __LINE__, message);
#else
#define NADWIN_LOG(message)
#endif

namespace nadwin
{
	struct Vector2D {
		Vector2D() {};

		Vector2D(int x, int y)
		{
			this->x = x;
			this->y = y;
		}

		int x = 0;
		int y = 0;
	};

	//
	// App
	//

	/// <summary>
	/// Base application class, platform specific implementations should inherit from this class
	/// </summary>
	class BaseApp {
	public:
		/// <summary>
		/// Creates a message loop and runs the application, returns the exit code when the application exits
		/// </summary>
		/// <returns>Exit code</returns>
		virtual int Run() = 0;
	};

	/// <summary>
	/// Base window class, platform specific implementations should inherit from this class
	/// </summary>
	class BaseWindow {
	public:
		/// <summary>
		/// Sets the title of a window
		/// </summary>
		/// <param name="title">The title to set</param>
		virtual void SetTitle(std::string title) = 0;

		/// <summary>
		/// Sets the position of the window
		/// </summary>
		/// <param name="x">New x position of the window</param>
		/// <param name="y">New y position of the window</param>
		virtual void SetPosition(int x, int y) = 0;
		/// <summary>
		/// Sets the size of the window
		/// </summary>
		/// <param name="width">New width of the window</param>
		/// <param name="height">New height of the window</param>
		virtual void SetSize(int width, int height) = 0;

		/// <summary>
		/// Shows the window
		/// </summary>
		virtual void Show() = 0;
		/// <summary>
		/// Destroys the window
		/// </summary>
		virtual void Destroy() = 0;

		void AddControl(class BaseControl* control);

	protected:
		std::vector<class BaseControl*> m_controls;

		void PaintControl(class BaseControl* control);
		
	private:
		friend BaseControl;
	};

	class BaseControl {
	public:
		BaseControl() {};
		~BaseControl() {};

		virtual void SetPosition(int x, int y) = 0;
		virtual void SetSize(int width, int height) = 0;

		virtual void Show() = 0;
		virtual void Destroy() = 0;

	protected:
		Vector2D m_position;
		Vector2D m_size;

		virtual void Paint() = 0;

	private:
		friend class BaseWindow;
	};


	//
	// ???
	//

	void BaseWindow::AddControl(BaseControl* control)
	{
		m_controls.push_back(control);
	}

	void BaseWindow::PaintControl(BaseControl* control)
	{
		control->Paint();
	}

	//
	// Windows Definitions
	//

#if defined(_WIN32)
	class WINApp : public BaseApp {
	public:
		WINApp();
		~WINApp();

		int Run() override;

	private:
		static LRESULT CALLBACK appProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		HINSTANCE m_hinstance = nullptr;

		friend class WINWindow;
	};

	class WINWindow : public BaseWindow {
	public:
		WINWindow();
		~WINWindow();

		void SetTitle(std::string title) override;
		void SetPosition(int x, int y) override;
		void SetSize(int width, int height) override;

		void Show() override;
		void Destroy() override;

	private:
		LRESULT CALLBACK windowProc(UINT msg, WPARAM wparam, LPARAM lparam);

		HWND m_hwnd = nullptr;

		friend class WINApp;
	};
#endif

	//
	// Windows Implementations
	//

#if defined(_WIN32)
	//
	// App
	//

	WINApp::WINApp()
	{
		// Register window class
		WNDCLASSEXA wc = { 0 };
		wc.cbSize = sizeof(WNDCLASSEXA);
		wc.lpfnWndProc = appProc;
		wc.hInstance = m_hinstance;
		wc.lpszClassName = "NADWIN_WINDOW";
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
		RegisterClassExA(&wc);
	}

	WINApp::~WINApp()
	{
		// Unregister window class
		UnregisterClassA("NADWIN_WINDOW", m_hinstance);
	}

	int WINApp::Run()
	{
		MSG msg = { 0 };

		while (GetMessageA(&msg, nullptr, 0, 0) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}

		return static_cast<int>(msg.wParam);
	}

	LRESULT CALLBACK WINApp::appProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		WINWindow* window = reinterpret_cast<WINWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

		if (window == nullptr)
		{
			NADWIN_LOG("Window is null, calling DefWindowProc, this message is not an error, it's just for debugging purposes");
			return DefWindowProcA(hwnd, msg, wParam, lParam);
		}

		return window->windowProc(msg, wParam, lParam);
	}

	//
	// Window
	//

	WINWindow::WINWindow() {
		m_hwnd = CreateWindowExA(
			0,
			"NADWIN_WINDOW",
			nullptr,
			WS_OVERLAPPEDWINDOW,
			0, 0, CW_USEDEFAULT, CW_USEDEFAULT,
			nullptr,
			nullptr,
			nullptr,
			nullptr
		);

		if (m_hwnd == nullptr) {
			throw std::runtime_error("Failed to create window");
		}

		SetWindowLongPtrW(m_hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	}

	WINWindow::~WINWindow() {
		DestroyWindow(m_hwnd);
	}

	void WINWindow::SetTitle(std::string title) {
		SetWindowTextA(m_hwnd, title.c_str());
	}

	void WINWindow::SetPosition(int x, int y) {
		SetWindowPos(m_hwnd, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}

	void WINWindow::SetSize(int width, int height) {
		SetWindowPos(m_hwnd, nullptr, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
	}

	void WINWindow::Show() {
		ShowWindow(m_hwnd, SW_SHOW);
	}

	void WINWindow::Destroy() {
		DestroyWindow(m_hwnd);
	}

	LRESULT CALLBACK WINWindow::windowProc(UINT msg, WPARAM wparam, LPARAM lparam) {
		switch (msg) {
		case WM_PAINT:
		{
			for (const auto& control : m_controls)
			{
				PaintControl(control);
			}
			break;
		}
		}

		return DefWindowProcA(m_hwnd, msg, wparam, lparam);
	}
#endif

	//
	// Controls - Definitions
	//

	class Button : public BaseControl {
	public:
		Button();
		~Button();

		void SetPosition(int x, int y);
		void SetSize(int width, int height);

		void Show();
		void Destroy();

	protected:
		void Paint() override;
	};

	Button::Button()
	{

	}

	Button::~Button()
	{

	}

	void Button::SetPosition(int x, int y)
	{
		m_position.x = x;
		m_position.y = y;
	}

	void Button::SetSize(int width, int height)
	{
		m_size.x = width;
		m_size.y = height;
	}

	void Button::Show()
	{

	}

	void Button::Destroy()
	{

	}

	void Button::Paint()
	{
		NADWIN_LOG("Button::Paint()");
	}

#if defined(_WIN32)
	typedef WINWindow Window;
	typedef WINApp App;
#endif
}

#endif // !NADWIN_HPP
