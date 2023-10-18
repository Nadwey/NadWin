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
#define NADWIN_LOG(message) printf("\033[36m[NadWin : LOG] - [%s:%d]: %s\033[0m\n", __FILE__, __LINE__, message);
#define NADWIN_WARN(message) printf("\033[33m[NadWin : WARN] - [%s:%d]: %s\033[0m\n", __FILE__, __LINE__, message);
#define NADWIN_ERROR(message) printf("\033[31m[NadWin : ERROR] - [%s:%d]: %s\033[0m\n", __FILE__, __LINE__, message);
#else
#define NADWIN_LOG(message)
#define NADWIN_WARN(message)
#define NADWIN_ERROR(message)
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

	struct ColorRGB {
		unsigned char r = 0;
		unsigned char g = 0;
		unsigned char b = 0;

		COLORREF ToCOLORREF()
		{
			return RGB(r, g, b);
		}
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

	class BaseRenderer {
	public:
		virtual void DrawRect(Vector2D position, Vector2D size, ColorRGB color) = 0;
		virtual void PaintText(Vector2D position, Vector2D size, std::string text, ColorRGB color) = 0; // stupid winapi macros
	};

	class BaseRenderingContext {
	public:
		virtual void Init(class BaseWindow* window) = 0;
		virtual void Shutdown() = 0;

		BaseRenderer* GetRenderer();

	protected:
		BaseRenderer* m_renderer = nullptr;
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

		BaseRenderingContext* GetRenderingContext();

	protected:
		std::vector<class BaseControl*> m_controls;

		void PaintControl(class BaseControl* control);

		BaseRenderingContext* m_renderingContext = nullptr;
	private:
		friend class BaseControl;
	};

	class BaseControl {
	public:
		virtual void SetPosition(int x, int y) = 0;
		virtual void SetSize(int width, int height) = 0;

		virtual void Destroy() = 0;

	protected:
		Vector2D m_position;
		Vector2D m_size;

		BaseWindow* m_window = nullptr;

		void RemoveFromWindow();

		virtual void Paint(BaseWindow* window) = 0;

	private:
		friend class BaseWindow;
	};

	//
	// ???
	//

	BaseRenderer* BaseRenderingContext::GetRenderer()
	{
		return m_renderer;
	}

	void BaseWindow::AddControl(BaseControl* control)
	{
		m_controls.push_back(control);
		control->m_window = this;
	}

	BaseRenderingContext* BaseWindow::GetRenderingContext()
	{
		return m_renderingContext;
	}

	void BaseWindow::PaintControl(BaseControl* control)
	{
		control->Paint(this);
	}

	void BaseControl::RemoveFromWindow()
	{
		if (!m_window)
		{
			NADWIN_WARN("Called Destroy() on a control that is not attached to a window, control will not be destroyed!");
			return;
		}
		m_window->m_controls.erase(std::remove(m_window->m_controls.begin(), m_window->m_controls.end(), this), m_window->m_controls.end());
	};

#if defined(_WIN32)
	//
	// Windows Definitions
	//

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

	class WinGDIRenderer : public BaseRenderer {
	public:
		WinGDIRenderer(HWND hwnd);
		~WinGDIRenderer();

		void DrawRect(Vector2D position, Vector2D size, ColorRGB color) override;
		void PaintText(Vector2D position, Vector2D size, std::string text, ColorRGB color) override;

	private:
		HWND m_hwnd = nullptr;
		HDC m_hdc = nullptr;
	};

	class WINRenderingContext : public BaseRenderingContext {
	public:
		void Init(BaseWindow* window) override;
		void Shutdown() override;
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
		friend class WINRenderingContext;
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

		if (!RegisterClassExA(&wc))
		{
			throw std::runtime_error("Failed to register window class");
		}
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
			NADWIN_WARN("Failed to get window reference in appProc");
			return DefWindowProcA(hwnd, msg, wParam, lParam);
		}

		return window->windowProc(msg, wParam, lParam);
	}

	//
	// WinGDIRenderer
	//

	WinGDIRenderer::WinGDIRenderer(HWND hwnd)
	{
		m_hwnd = hwnd;
		m_hdc = GetDC(m_hwnd);
	}

	WinGDIRenderer::~WinGDIRenderer()
	{
		ReleaseDC(m_hwnd, m_hdc);
	}

	void WinGDIRenderer::DrawRect(Vector2D position, Vector2D size, ColorRGB color)
	{
		RECT rect = { 0 };
		rect.left = position.x;
		rect.top = position.y;
		rect.right = position.x + size.x;
		rect.bottom = position.y + size.y;

		HBRUSH brush = (HBRUSH)::GetStockObject(DC_BRUSH);
		SetDCBrushColor(m_hdc, color.ToCOLORREF());

		FillRect(m_hdc, &rect, brush);
	}

	void WinGDIRenderer::PaintText(Vector2D position, Vector2D size, std::string text, ColorRGB color)
	{
		SetTextColor(m_hdc, color.ToCOLORREF());
		SetBkMode(m_hdc, TRANSPARENT);

		RECT rect;
		rect.left = position.x;
		rect.top = position.y;
		rect.right = position.x + size.x;
		rect.bottom = position.y + size.y;

		DrawTextA(m_hdc, text.c_str(), text.length(), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}

	//
	// WINRenderingContext
	//

	void WINRenderingContext::Init(BaseWindow* window)
	{
		WINWindow* winWindow = reinterpret_cast<WINWindow*>(window);

		// TODO: Check which renderer to use
		m_renderer = new WinGDIRenderer(winWindow->m_hwnd);
	}

	void WINRenderingContext::Shutdown()
	{
		delete m_renderer;
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

		m_renderingContext = new WINRenderingContext();
		m_renderingContext->Init(this);
	}

	WINWindow::~WINWindow() {
		m_renderingContext->Shutdown();
		delete m_renderingContext;
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

		void SetPosition(int x, int y) override;
		void SetSize(int width, int height) override;

		void Destroy() override;

		ColorRGB background_color = { 32, 128, 255 };
		std::string text = "";
	protected:
		void Paint(BaseWindow* window) override;
	};

	Button::Button()
	{

	}

	Button::~Button()
	{
		Destroy();
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

	void Button::Destroy()
	{
		RemoveFromWindow();
	}

	void Button::Paint(BaseWindow* window)
	{
		// TODO: Check if control is visible

		BaseRenderer* renderer = window->GetRenderingContext()->GetRenderer();
		renderer->DrawRect(m_position, m_size, background_color);
		renderer->PaintText(m_position, m_size, text, { 255, 255, 255 });
	}

#if defined(_WIN32)
	typedef WINWindow Window;
	typedef WINApp App;
#endif
}

#endif // !NADWIN_HPP
