﻿#include "NadWin.h"

namespace NW
{
    namespace UI {
        bool HasStyle(HWND hwnd, LONG style)
        {
            return GetWindowLongPtr(hwnd, GWL_STYLE) & style;
        }

        void AppendStyle(HWND hwnd, LONG style) 
        {
            LONG_PTR currentStyle = GetWindowLongPtr(hwnd, GWL_STYLE);
            SetWindowLongPtr(hwnd, GWL_STYLE, currentStyle | style);
        }

        void RemoveStyle(HWND hwnd, LONG style)
        {
            LONG_PTR currentStyle = GetWindowLongPtr(hwnd, GWL_STYLE);
            SetWindowLongPtr(hwnd, GWL_STYLE, currentStyle & ~style);
        }

        std::wstring s2ws(std::string s)
        {
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            std::wstring ws = converter.from_bytes(s);
            return ws;
        }

        //
        //
        // class Font
        //
        //

        //
        // public:
        //

        Font::Font(int height, std::wstring faceName, int width, bool italic, bool underline, bool strike)
            : height(height)
            , faceName(faceName)
            , width(width)
            , italic(italic)
            , underline(underline)
            , strike(strike)
        {
            update();
        }

        Font::Font(int height, std::string faceName, int width, bool italic, bool underline, bool strike) : height(height)
            , faceName(s2ws(faceName))
            , width(width)
            , italic(italic)
            , underline(underline)
            , strike(strike)
        {
            update();
        }

        Font::~Font()
        {
            if (font) DeleteObject(font);
        }

        void Font::SetHeight(int height)
        {
            this->height = height;
            update();
        }
        void Font::SetFaceName(std::wstring faceName)
        {
            this->faceName = faceName;
            update();
        }
        void Font::SetFaceName(std::string faceName)
        {
            this->faceName = s2ws(faceName);
            update();
        }
        void Font::SetWidth(int width)
        {
            this->width = width;
            update();
        }
        void Font::SetItalic(bool italic)
        {
            this->italic = italic;
            update();
        }
        void Font::SetUnderline(bool underline)
        {
            this->underline = underline;
            update();
        }
        void Font::SetStrike(bool strike)
        {
            this->strike = strike;
            update();
        }

        int Font::GetHeight()
        {
            return height;
        }
        std::wstring Font::GetFaceName()
        {
            return faceName;
        }
        int Font::GetWidth()
        {
            return width;
        }
        bool Font::GetItalic()
        {
            return italic;
        }
        bool Font::GetUnderline()
        {
            return underline;
        }
        bool Font::GetStrike()
        {
            return strike;
        }

        const HFONT Font::GetFont()
        {
            return font;
        }

        //
        // private:
        //

        void Font::update()
        {
            if (font) DeleteObject(font);
            font = CreateFontW(height, width, 0, 0, 0, italic, underline, strike, 0, 0, 0, ANTIALIASED_QUALITY, 0, faceName.c_str());
        }



        //
        //
        // class Position
        //
        //

        //
        // public:
        //

        Position::Position(int x, int y, int width, int height) : x(x), y(y), width(width), height(height)
        {

        }

        Position::Position(RECT rect) : Position(&rect)
        {

        }

        Position::Position(RECT* rect)
        {
            FromRect(rect);
        }

        RECT Position::Rect()
        {
            RECT rc;
            rc.left = x;
            rc.top = y;
            rc.right = x + width;
            rc.bottom = y + height;
            return rc;
        }

        void Position::FromRect(RECT rect)
        {
            FromRect(&rect);
        }

        void Position::FromRect(RECT* rect)
        {
            x = rect->left;
            y = rect->top;
            width = rect->right - rect->left;
            height = rect->bottom - rect->top;
        }

        //
        //
        // class Border
        //
        //

        //
        // public:
        //

        Border::Border(int left, int top, int right, int bottom, COLORREF color)
            : left(left)
            , top(top)
            , right(right)
            , bottom(bottom)
        {

        }

        bool Border::NeedsRender()
        {
            return left || top || right || bottom;
        }

        //
        //
        // class App
        //
        //

        //
        // public:
        //

        App::App(std::wstring AppName)
        {
            registerClass(AppName);
        }

        App::App(std::string AppName)
        {
            std::wstring AppNameW = s2ws(AppName);
            registerClass(AppNameW);
        }

        App::~App()
        {
            UnregisterClassW(AppName.c_str(), hInstance);
            initialized = false;
        }

        WPARAM App::MessageLoop()
        {
            MSG msg;
            while (GetMessage(&msg, NULL, 0, 0) != 0)
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            return msg.wParam;
        }

        std::wstring App::GetAppName()
        {
            return AppName;
        }

        void App::DoEvents()
        {
            MSG msg;
            BOOL result;

            while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
            {
                result = ::GetMessage(&msg, NULL, 0, 0);
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
            }
        }

        //
        // private:
        //

        void App::registerClass(std::wstring& AppName)
        {
            if (initialized) throw std::exception("Application class is arleady registered");
            hInstance = reinterpret_cast<HINSTANCE>(GetModuleHandle(nullptr));

            WNDCLASSEXW wcx = { 0 };

            wcx.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
            wcx.cbSize = sizeof(WNDCLASSEXW);
            wcx.lpfnWndProc = reinterpret_cast<WNDPROC>(proc);
            wcx.hInstance = hInstance;
            wcx.hCursor = LoadCursorW(nullptr, IDC_ARROW);
            wcx.hIcon = LoadIconW(nullptr, IDI_APPLICATION);
            wcx.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW+1);
            wcx.lpszClassName = AppName.c_str();

            if (!RegisterClassExW(&wcx)) throw std::exception("Failed to register application class");
            this->AppName = AppName;
            initialized = true;
        }

        LRESULT CALLBACK App::proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            Window* windowHandle = reinterpret_cast<Window*>(GetWindowLongPtrA(hwnd, GWLP_USERDATA));
            if (windowHandle) return windowHandle->proc(msg, wParam, lParam);
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }

        std::wstring App::AppName = L"";
        HINSTANCE App::hInstance = nullptr;
        bool App::initialized = false;

        //
        //
        // struct WindowEventInfo
        //
        //

        //
        // public:
        //

        void WindowEventInfo::OverrideProcResult(LRESULT result)
        {
            overrideProcResult = true;
            this->result = result;
        }

        //
        //
        // class Window
        //
        //

        //
        // public:
        //

        Window::Window(std::wstring WindowName, int x, int y, int width, int height)
        {
            createWindow(WindowName, x, y, width, height);
        }

        Window::Window(std::string WindowName, int x, int y, int width, int height)
        {
            std::wstring WindowNameW = s2ws(WindowName);
            createWindow(WindowNameW, x, y, width, height);
        }

        Window::~Window()
        {
            if (!hwnd) return;
            DestroyWindow(hwnd);
            hwnd = nullptr;
        }

        const HWND Window::Hwnd()
        {
            return hwnd;
        }

        void Window::Move(int x, int y, int width, int height, bool repaint)
        {
            MoveWindow(hwnd, x, y, width, height, repaint);
        }

        void Window::Show()
        {
            ShowWindow(hwnd, SW_SHOW);
            UpdateWindow(hwnd);
        }

        void Window::Add(Control* button)
        {
            button->create();
        }

        void Window::Repaint()
        {
            InvalidateRect(hwnd, nullptr, true);
        }

        //
        // private:
        //

        void Window::createWindow(std::wstring& WindowName, int x, int y, int width, int height)
        {
            hwnd = CreateWindowExW(0L, App::AppName.c_str(), WindowName.c_str(), WS_OVERLAPPEDWINDOW, x, y, width, height, nullptr, nullptr, App::hInstance, nullptr);
            if (!hwnd) throw std::exception("Failed to create window");
            SetWindowLongPtrA(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
        }

        LRESULT CALLBACK Window::proc(UINT msg, WPARAM wParam, LPARAM lParam)
        {
            WindowEventInfo windowEventInfo;
            windowEventInfo.uMsg = msg;
            windowEventInfo.wParam = wParam;
            windowEventInfo.lParam = lParam;
            windowEventInfo.control = this;
            switch (msg)
            {
            case WM_DESTROY:
                if (this->EventHandler) this->EventHandler(WindowEventTypes::Destroy, &windowEventInfo);
                break;
            case WM_MOUSEMOVE:
            {
                if (this->EventHandler) this->EventHandler(WindowEventTypes::MouseMove, &windowEventInfo);

                TRACKMOUSEEVENT tme = { 0 };
                tme.cbSize = sizeof(tme);
                tme.hwndTrack = this->hwnd;
                tme.dwFlags = TME_LEAVE;
                TrackMouseEvent(&tme);

                if (this->isOver) break;
                this->isOver = true;

                if (this->EventHandler) this->EventHandler(WindowEventTypes::MouseOver, &windowEventInfo);
                break;
            }
            case WM_LBUTTONDBLCLK:
                if (this->EventHandler) this->EventHandler(WindowEventTypes::MouseLeftDoubleClick, &windowEventInfo);
                break;
            case WM_LBUTTONDOWN:
                if (this->EventHandler) this->EventHandler(WindowEventTypes::MouseLeftDown, &windowEventInfo);
                break;
            case WM_LBUTTONUP:
                if (this->EventHandler) this->EventHandler(WindowEventTypes::MouseLeftUp, &windowEventInfo);
                break;
            case WM_RBUTTONDBLCLK:
                if (this->EventHandler) this->EventHandler(WindowEventTypes::MouseRightDoubleClick, &windowEventInfo);
                break;
            case WM_RBUTTONDOWN:
                if (this->EventHandler) this->EventHandler(WindowEventTypes::MouseRightDown, &windowEventInfo);
                break;
            case WM_RBUTTONUP:
                if (this->EventHandler) this->EventHandler(WindowEventTypes::MouseRightUp, &windowEventInfo);
                break;
            case WM_MBUTTONDBLCLK:
                if (this->EventHandler) this->EventHandler(WindowEventTypes::MouseMiddleDoubleClick, &windowEventInfo);
                break;
            case WM_MBUTTONDOWN:
                if (this->EventHandler) this->EventHandler(WindowEventTypes::MouseMiddleDown, &windowEventInfo);
                break;
            case WM_MBUTTONUP:
                if (this->EventHandler) this->EventHandler(WindowEventTypes::MouseMiddleUp, &windowEventInfo);
                break;
            case WM_MOUSELEAVE:
                if (!this->isOver) break;
                this->isOver = false;
                if (this->EventHandler) this->EventHandler(WindowEventTypes::MouseLeave, &windowEventInfo);
                break;
            case WM_SETFOCUS:
                if (this->EventHandler) this->EventHandler(WindowEventTypes::Focus, &windowEventInfo);
                break;
            case WM_KILLFOCUS:
                if (this->EventHandler) this->EventHandler(WindowEventTypes::RemoveFocus, &windowEventInfo);
                break;
            case WM_COMMAND:
                PostMessage(reinterpret_cast<HWND>(lParam), WM_COMMAND, wParam, lParam);
            default:
                if (this->EventHandler) this->EventHandler(WindowEventTypes::Undefined, &windowEventInfo);
                break;
            }

            if (windowEventInfo.overrideProcResult) return windowEventInfo.result;
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }

        //
        //
        // struct ControlEventInfo
        //
        //

        //
        // public:
        //

        void ControlEventInfo::OverrideProcResult(LRESULT result)
        {
            overrideProcResult = true;
            this->result = result;
        }

        //
        //
        // class Control
        //
        //

        //
        // public:
        //

        void Control::Repaint()
        {
            InvalidateRect(hwnd, nullptr, false);
        }

        void Control::UpdateFont()
        {
            SendMessage(hwnd, WM_SETFONT, reinterpret_cast<WPARAM>(font.GetFont()), false);
        }

        Position Control::GetPosition()
        {
            return position;
        }

        void Control::SetPosition(Position position)
        {
            this->position = position;
            MoveWindow(hwnd, position.x, position.y, position.width, position.height, false);
        }

        std::wstring Control::GetText()
        {
            return text;
        }

        void Control::SetText(std::string text)
        {
            std::wstring ws = s2ws(text);
            this->text = ws;
            updateText();
        }

        void Control::SetText(std::wstring text)
        {
            this->text = text;
            updateText();
        }

        void Control::Focus()
        {
            SetFocus(hwnd);
        }

        void Control::RemoveFocus()
        {
            SetFocus(nullptr);
        }

        HWND Control::Hwnd()
        {
            return hwnd;
        }

        void Control::Destroy()
        {
            DestroyWindow(hwnd);
        }

        //
        // protected:
        //

        Control::Control() : font(20, L"Segoe UI")
        {

        }

        void Control::create()
        {
            throw std::exception("Can't create pure control class");
        }

        LRESULT CALLBACK Control::ControlProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
        {
            Control* control = reinterpret_cast<Control*>(dwRefData);
            ControlEventInfo controlEventInfo;
            controlEventInfo.uMsg = msg;
            controlEventInfo.wParam = wParam;
            controlEventInfo.lParam = lParam;
            controlEventInfo.control = control;
            switch (msg)
            {
            case WM_DESTROY:
                if (control->EventHandler) control->EventHandler(ControlEventTypes::Destroy, &controlEventInfo);
                break;
            case WM_MOUSEMOVE:
            {
                if (control->EventHandler) control->EventHandler(ControlEventTypes::MouseMove, &controlEventInfo);

                TRACKMOUSEEVENT tme = { 0 };
                tme.cbSize = sizeof(tme);
                tme.hwndTrack = control->hwnd;
                tme.dwFlags = TME_LEAVE;
                TrackMouseEvent(&tme);

                if (control->isOver) break;
                control->isOver = true;

                if (control->EventHandler) control->EventHandler(ControlEventTypes::MouseOver, &controlEventInfo);
                break;
            }
            case WM_LBUTTONDBLCLK:
                if (control->EventHandler) control->EventHandler(ControlEventTypes::MouseLeftDoubleClick, &controlEventInfo);
                break;
            case WM_LBUTTONDOWN:
                if (control->EventHandler) control->EventHandler(ControlEventTypes::MouseLeftDown, &controlEventInfo);
                break;
            case WM_LBUTTONUP:
                if (control->EventHandler) control->EventHandler(ControlEventTypes::MouseLeftUp, &controlEventInfo);
                break;
            case WM_RBUTTONDBLCLK:
                if (control->EventHandler) control->EventHandler(ControlEventTypes::MouseRightDoubleClick, &controlEventInfo);
                break;
            case WM_RBUTTONDOWN:
                if (control->EventHandler) control->EventHandler(ControlEventTypes::MouseRightDown, &controlEventInfo);
                break;
            case WM_RBUTTONUP:
                if (control->EventHandler) control->EventHandler(ControlEventTypes::MouseRightUp, &controlEventInfo);
                break;
            case WM_MBUTTONDBLCLK:
                if (control->EventHandler) control->EventHandler(ControlEventTypes::MouseMiddleDoubleClick, &controlEventInfo);
                break;
            case WM_MBUTTONDOWN:
                if (control->EventHandler) control->EventHandler(ControlEventTypes::MouseMiddleDown, &controlEventInfo);
                break;
            case WM_MBUTTONUP:
                if (control->EventHandler) control->EventHandler(ControlEventTypes::MouseMiddleUp, &controlEventInfo);
                break;
            case WM_MOUSELEAVE:
                if (!control->isOver) break;
                control->isOver = false;
                if (control->EventHandler) control->EventHandler(ControlEventTypes::MouseLeave, &controlEventInfo);
                break;
            case WM_SETFOCUS:
                if (control->EventHandler) control->EventHandler(ControlEventTypes::Focus, &controlEventInfo);
                break;
            case WM_KILLFOCUS:
                if (control->EventHandler) control->EventHandler(ControlEventTypes::RemoveFocus, &controlEventInfo);
                break;
            case WM_COMMAND:
                if (control->EventHandler) control->EventHandler(ControlEventTypes::FromParent_Command, &controlEventInfo);
                break;
            default:
                if (control->EventHandler) control->EventHandler(ControlEventTypes::Undefined, &controlEventInfo);
                break;
            }

            if (controlEventInfo.overrideProcResult) return controlEventInfo.result;
            return DefSubclassProc(hwnd, msg, wParam, lParam);
        }

        void Control::updateText()
        {
            SendMessageW(hwnd, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(text.c_str()));
        }

        //
        //
        // class Button
        //
        //

        //
        // public:
        //

        Button::Button(Window* window, Position position, std::string text)
        {
            this->window = window;
            std::wstring ws = s2ws(text);
            initialize(position, ws);
        }

        Button::Button(Window* window, Position position, std::wstring text)
        {
            this->window = window;
            initialize(position, text);
        }

        Button::~Button()
        {
            if (hwnd)
            {
                DestroyWindow(hwnd);
                hwnd = nullptr;
            }
        }

        //
        // private:
        //

        void Button::initialize(Position& position, std::wstring& text)
        {
            this->position = position;
            SetPosition(position);
            SetText(text);
        }

        void Button::create()
        {
            hwnd = CreateWindowExW(0, L"BUTTON", text.c_str(), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, position.x, position.y, position.width, position.height, window->Hwnd(), nullptr, nullptr, nullptr);
            SetWindowLongPtrA(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
            SetWindowSubclass(hwnd, Button::ControlProc, 0, reinterpret_cast<DWORD_PTR>(this));
            UpdateFont();
        }

        //
        //
        // class CheckBox
        //
        //

        //
        // public:
        //

        CheckBox::CheckBox(Window* window, Position position, std::string text)
        {
            this->window = window;
            std::wstring ws = s2ws(text);
            initialize(position, ws);
        }

        CheckBox::CheckBox(Window* window, Position position, std::wstring text)
        {
            this->window = window;
            initialize(position, text);
        }

        CheckBox::~CheckBox()
        {
            if (hwnd)
            {
                DestroyWindow(hwnd);
                hwnd = nullptr;
            }
        }

        void CheckBox::SetChecked(bool checked)
        {
            PostMessageW(hwnd, BM_SETCHECK, checked ? BST_CHECKED : BST_UNCHECKED, 0);
        }

        bool CheckBox::GetChecked()
        {
            return SendMessage(hwnd, BM_GETCHECK, 0, 0) == BST_CHECKED;
        }

        void CheckBox::ToggleChecked()
        {
            SetChecked(!GetChecked());
        }

        //
        // private:
        //

        void CheckBox::initialize(Position& position, std::wstring& text)
        {
            this->position = position;
            SetPosition(position);
            SetText(text);
        }

        void CheckBox::create()
        {
            hwnd = CreateWindowExW(0, L"BUTTON", text.c_str(), WS_VISIBLE | WS_CHILD | BS_CHECKBOX, position.x, position.y, position.width, position.height, window->Hwnd(), nullptr, nullptr, nullptr);
            SetWindowLongPtrA(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
            SetWindowSubclass(hwnd, CheckBox::ControlProc, 0, reinterpret_cast<DWORD_PTR>(this));
            UpdateFont();
        }

        //
        //
        // class ComboBox
        //
        //

        //
        // public:
        //

        ComboBox::ComboBox(Window* window, Position position, std::string text)
        {
            this->window = window;
            std::wstring ws = s2ws(text);
            initialize(position, ws);
        }

        ComboBox::ComboBox(Window* window, Position position, std::wstring text)
        {
            this->window = window;
            initialize(position, text);
        }

        ComboBox::~ComboBox()
        {
            if (hwnd)
            {
                DestroyWindow(hwnd);
                hwnd = nullptr;
            }
        }

        LRESULT ComboBox::AddString(std::wstring str)
        {
            return SendMessageW(hwnd, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(str.c_str()));
        }

        // Zwraca true jeśli operacja powiodła się
        bool ComboBox::DeleteString(LRESULT index)
        {
            return SendMessageW(hwnd, CB_DELETESTRING, index, 0) != CB_ERR;
        }

        bool ComboBox::DeleteString(std::wstring str)
        {
            return DeleteString(FindString(str));
        }

        LRESULT ComboBox::FindString(std::wstring str)
        {
            return SendMessageW(hwnd, CB_FINDSTRINGEXACT, -1, reinterpret_cast<LPARAM>(str.c_str()));
        }

        LRESULT ComboBox::GetCount()
        {
            return SendMessageW(hwnd, CB_GETCOUNT, 0, 0);
        }

        LRESULT ComboBox::GetStringLength(LRESULT index)
        {
            return SendMessageW(hwnd, CB_GETLBTEXTLEN, index, 0);
        }

        std::wstring ComboBox::GetString(LRESULT index)
        {
            std::wstring out;
            LRESULT len = GetStringLength(index);
            WCHAR* buf = new WCHAR[len + 1];
            SendMessageW(hwnd, CB_GETLBTEXT, index, reinterpret_cast<LPARAM>(buf));
            out = buf;
            delete[] buf;
            return out;
        }

        LRESULT ComboBox::GetSelected()
        {
            return SendMessageW(hwnd, CB_GETCURSEL, 0, 0);
        }

        std::wstring ComboBox::GetSelectedString()
        {
            return GetString(GetSelected());
        }

        void ComboBox::SetSelected(LRESULT index)
        {
            SendMessage(hwnd, CB_SETCURSEL, index, 0);
        }

        void ComboBox::SetSelected(std::wstring str)
        {
            SendMessage(hwnd, CB_SETCURSEL, FindString(str), 0);
        }

        void ComboBox::ShowDropdown(bool show)
        {
            SendMessageW(hwnd, CB_SHOWDROPDOWN, static_cast<WPARAM>(show), 0);
        }

        //
        // private:
        //

        void ComboBox::initialize(Position& position, std::wstring& text)
        {
            this->position = position;
            SetPosition(position);
            SetText(text);
        }

        void ComboBox::create()
        {
            hwnd = CreateWindowExW(0, WC_COMBOBOXW, text.c_str(), CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_VISIBLE | WS_VSCROLL, position.x, position.y, position.width, position.height, window->Hwnd(), nullptr, nullptr, nullptr);
            SetWindowLongPtrA(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
            SetWindowSubclass(hwnd, ComboBox::ControlProc, 0, reinterpret_cast<DWORD_PTR>(this));
            UpdateFont();
        }

        //
        //
        // class DatePicker
        //
        //

        //
        // public:
        //

        DatePicker::DatePicker(Window* window, Position position, std::string text)
        {
            this->window = window;
            std::wstring ws = s2ws(text);
            initialize(position, ws);
        }

        DatePicker::DatePicker(Window* window, Position position, std::wstring text)
        {
            this->window = window;
            initialize(position, text);
        }

        DatePicker::~DatePicker()
        {
            if (hwnd)
            {
                DestroyWindow(hwnd);
                hwnd = nullptr;
            }
        }

        void DatePicker::SetTime(SYSTEMTIME time)
        {
            SendMessageW(hwnd, DTM_SETSYSTEMTIME, GDT_VALID, reinterpret_cast<LPARAM>(&time));
        }

        SYSTEMTIME DatePicker::GetTime(bool* valid)
        {
            SYSTEMTIME time;
            LRESULT result = SendMessageW(hwnd, DTM_GETSYSTEMTIME, 0, reinterpret_cast<LPARAM>(&time));
            if (valid) *valid = result == GDT_VALID;
            return time;
        }

        void DatePicker::SetFormat(std::wstring format)
        {
            SendMessageW(hwnd, DTM_SETFORMAT, 0, reinterpret_cast<LPARAM>(format.c_str()));
        }

        void DatePicker::SetMin(SYSTEMTIME time)
        {
            SYSTEMTIME times[2];
            times[0] = time;
            SendMessage(hwnd, DTM_SETRANGE, GDTR_MIN, reinterpret_cast<LPARAM>(&times));
        }

        void DatePicker::SetMax(SYSTEMTIME time)
        {
            SYSTEMTIME times[2];
            times[1] = time;
            SendMessage(hwnd, DTM_SETRANGE, GDTR_MAX, reinterpret_cast<LPARAM>(&times));
        }

        SYSTEMTIME DatePicker::GetMin()
        {
            SYSTEMTIME times[2];
            SendMessage(hwnd, DTM_GETRANGE, 0, reinterpret_cast<LPARAM>(&times));
            return times[0];
        }

        SYSTEMTIME DatePicker::GetMax()
        {
            SYSTEMTIME times[2];
            SendMessage(hwnd, DTM_GETRANGE, 0, reinterpret_cast<LPARAM>(&times));
            return times[1];
        }

        //
        // private:
        //

        void DatePicker::initialize(Position& position, std::wstring& text)
        {
            this->position = position;
            SetPosition(position);
            SetText(text);
        }

        void DatePicker::create()
        {
            hwnd = CreateWindowExW(0, DATETIMEPICK_CLASSW, text.c_str(), WS_BORDER | WS_CHILD | WS_VISIBLE | DTS_SHOWNONE, position.x, position.y, position.width, position.height, window->Hwnd(), nullptr, nullptr, nullptr);
            SetWindowLongPtrA(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
            SetWindowSubclass(hwnd, DatePicker::ControlProc, 0, reinterpret_cast<DWORD_PTR>(this));
            UpdateFont();
        }

        //
        //
        // class TextBoxBase
        //
        //

        //
        // public:
        //

        void TextBoxBase::SetTextAlign(TextBoxTextAlign align) {
            switch (align)
            {
            case NW::UI::TextBoxTextAlign::Left:
                AppendStyle(hwnd, ES_LEFT);
                RemoveStyle(hwnd, ES_CENTER);
                RemoveStyle(hwnd, ES_RIGHT);
                break;
            case NW::UI::TextBoxTextAlign::Center:
                RemoveStyle(hwnd, ES_LEFT);
                AppendStyle(hwnd, ES_CENTER);
                RemoveStyle(hwnd, ES_RIGHT);
                break;
            case NW::UI::TextBoxTextAlign::Right:
                RemoveStyle(hwnd, ES_LEFT);
                RemoveStyle(hwnd, ES_CENTER);
                AppendStyle(hwnd, ES_RIGHT);
                break;
            }
        }

        TextBoxTextAlign TextBoxBase::GetTextAlign()
        {
            if (HasStyle(hwnd, ES_LEFT)) return TextBoxTextAlign::Left;
            if (HasStyle(hwnd, ES_CENTER)) return TextBoxTextAlign::Center;
            if (HasStyle(hwnd, ES_RIGHT)) return TextBoxTextAlign::Right;
            return TextBoxTextAlign::Left;
        }

        void TextBoxBase::SetReadOnly(bool readOnly)
        {
            SendMessage(hwnd, EM_SETREADONLY, static_cast<WPARAM>(readOnly), 0);
        }

        void TextBoxBase::SetSelection(Selection selection)
        {
            SendMessage(hwnd, EM_SETSEL, selection.start, selection.end);
        }

        Selection TextBoxBase::GetSelection()
        {
            Selection selection;
            SendMessage(hwnd, EM_GETSEL, reinterpret_cast<WPARAM>(&selection.start), reinterpret_cast<WPARAM>(&selection.end));
            return selection;
        }

        //
        //
        // class TextBoxMultiline
        //
        //

        //
        // public:
        //

        TextBoxMultiline::TextBoxMultiline(Window* window, Position position, std::string text)
        {
            this->window = window;
            std::wstring ws = s2ws(text);
            initialize(position, ws);
        }

        TextBoxMultiline::TextBoxMultiline(Window* window, Position position, std::wstring text)
        {
            this->window = window;
            initialize(position, text);
        }
        TextBoxMultiline::~TextBoxMultiline()
        {
            if (hwnd)
            {
                DestroyWindow(hwnd);
                hwnd = nullptr;
            }
        }

        LRESULT TextBoxMultiline::GetLineIndex(LRESULT line)
        {
            return SendMessage(hwnd, EM_LINEINDEX, line, 0);
        }

        LRESULT TextBoxMultiline::GetLineLength(LRESULT line)
        {
            return SendMessage(hwnd, EM_LINELENGTH, GetLineIndex(line), 0);
        }

        LRESULT TextBoxMultiline::GetLineCount()
        {
            return SendMessage(hwnd, EM_GETLINECOUNT, 0, 0);
        }

        //
        // private:
        //

        void TextBoxMultiline::initialize(Position& position, std::wstring& text)
        {
            this->position = position;
            SetPosition(position);
            SetText(text);
        }

        void TextBoxMultiline::create()
        {
            hwnd = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", text.c_str(), WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL, position.x, position.y, position.width, position.height, window->Hwnd(), nullptr, nullptr, nullptr);
            SetWindowLongPtrA(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
            SetWindowSubclass(hwnd, TextBoxMultiline::ControlProc, 0, reinterpret_cast<DWORD_PTR>(this));
            UpdateFont();
        }

        //
        //
        // class TextBoxSingleline
        //
        //

        //
        // public:
        //

        TextBoxSingleline::TextBoxSingleline(Window* window, Position position, std::string text)
        {
            this->window = window;
            std::wstring ws = s2ws(text);
            initialize(position, ws);
        }

        TextBoxSingleline::TextBoxSingleline(Window* window, Position position, std::wstring text)
        {
            this->window = window;
            initialize(position, text);
        }
        TextBoxSingleline::~TextBoxSingleline()
        {
            if (hwnd)
            {
                DestroyWindow(hwnd);
                hwnd = nullptr;
            }
        }

        void TextBoxSingleline::SetPasswordMode(bool passwordMode)
        {
            if (passwordMode) SendMessage(hwnd, EM_SETPASSWORDCHAR, 0x2022, 0);
            else SendMessage(hwnd, EM_SETPASSWORDCHAR, 0, 0);
        }

        bool TextBoxSingleline::GetPasswordMode()
        {
            return SendMessage(hwnd, EM_GETPASSWORDCHAR, 0, 0);
        }

        //
        // private:
        //

        void TextBoxSingleline::initialize(Position& position, std::wstring& text)
        {
            this->position = position;
            SetPosition(position);
            SetText(text);
        }

        void TextBoxSingleline::create()
        {
            hwnd = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", text.c_str(), WS_CHILD | WS_VISIBLE | ES_LEFT | ES_AUTOHSCROLL | ES_PASSWORD, position.x, position.y, position.width, position.height, window->Hwnd(), nullptr, nullptr, nullptr);
            SetWindowLongPtrA(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
            SetWindowSubclass(hwnd, TextBoxSingleline::ControlProc, 0, reinterpret_cast<DWORD_PTR>(this));
            UpdateFont();
            SetPasswordMode(false);
        }

        //
        //
        // class Static
        //
        //

        //
        // public:
        //

        ListBox::ListBox(Window* window, Position position, std::string text)
        {
            this->window = window;
            std::wstring ws = s2ws(text);
            initialize(position, ws);
        }

        ListBox::ListBox(Window* window, Position position, std::wstring text)
        {
            this->window = window;
            initialize(position, text);
        }
        ListBox::~ListBox()
        {
            if (hwnd)
            {
                DestroyWindow(hwnd);
                hwnd = nullptr;
            }
        }

        LRESULT ListBox::AddString(std::wstring str)
        {
            return SendMessageW(hwnd, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(str.c_str()));
        }

        bool ListBox::DeleteString(LRESULT index)
        {
            return SendMessageW(hwnd, LB_ADDSTRING, index, 0) == LB_ERR;
        }

        bool ListBox::DeleteString(std::wstring str)
        {
            return DeleteString(FindString(str));
        }

        LRESULT ListBox::FindString(std::wstring str)
        {
            return SendMessageW(hwnd, LB_FINDSTRINGEXACT, -1, reinterpret_cast<LPARAM>(str.c_str()));
        }
        
        LRESULT ListBox::GetCount()
        {
            return SendMessageW(hwnd, LB_GETCOUNT, 0, 0);
        }

        LRESULT ListBox::GetStringLength(LRESULT index)
        {
            LRESULT len = SendMessageW(hwnd, LB_GETTEXTLEN, index, 0);
            if (len == LB_ERR) throw std::exception("Opcja nie istnieje");
            return len;
        }

        std::wstring ListBox::GetString(LRESULT index)
        {
            std::wstring out;
            LRESULT len = GetStringLength(index);
            WCHAR* buf = new WCHAR[len + 1];
            SendMessageW(hwnd, LB_GETTEXT, index, reinterpret_cast<LPARAM>(buf));
            out = buf;
            delete[] buf;
            return out;
        }

        LRESULT ListBox::GetSelected()
        {
            LRESULT selected = SendMessageW(hwnd, LB_GETCURSEL, 0, 0);
            if (selected == LB_ERR) throw std::exception("Zadna opcja nie jest wybrana, uzyj IsSelected()");
            return selected;
        }

        std::wstring ListBox::GetSelectedString()
        {
            std::wstring str = GetString(GetSelected());
            return str;
        }

        bool ListBox::IsSelected()
        {
            return SendMessageW(hwnd, LB_GETCURSEL, 0, 0) != LB_ERR;
        }

        void ListBox::SetSelected(LRESULT index)
        {
            SendMessageW(hwnd, LB_SETCURSEL, index, 0);
        }

        void ListBox::SetSelected(std::wstring str)
        {
            SetSelected(FindString(str));
        }

        void ListBox::SetSort(bool sort)
        {
            if (sort) AppendStyle(hwnd, LBS_SORT);
            else RemoveStyle(hwnd, LBS_SORT);
        }

        bool ListBox::GetSort()
        {
            return HasStyle(hwnd, LBS_SORT);
        }

        //
        // private:
        //

        void ListBox::initialize(Position& position, std::wstring& text)
        {
            this->position = position;
            SetPosition(position);
            SetText(text);
        }

        void ListBox::create()
        {
            hwnd = CreateWindowExW(WS_EX_CLIENTEDGE, WC_LISTBOXW, text.c_str(), LBS_HASSTRINGS | WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL, position.x, position.y, position.width, position.height, window->Hwnd(), nullptr, nullptr, nullptr);
            SetWindowLongPtrA(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
            SetWindowSubclass(hwnd, ListBox::ControlProc, 0, reinterpret_cast<DWORD_PTR>(this));
            UpdateFont();
        }

        //
        //
        // class Static
        //
        //

        //
        // public:
        //

        Static::Static(Window* window, Position position, std::string text)
        {
            this->window = window;
            std::wstring ws = s2ws(text);
            initialize(position, ws);
        }

        Static::Static(Window* window, Position position, std::wstring text)
        {
            this->window = window;
            initialize(position, text);
        }
        Static::~Static()
        {
            if (hwnd)
            {
                DestroyWindow(hwnd);
                hwnd = nullptr;
            }
        }

        //
        // private:
        //

        void Static::initialize(Position& position, std::wstring& text)
        {
            this->position = position;
            SetPosition(position);
            SetText(text);
        }

        void Static::create()
        {
            hwnd = CreateWindowExW(0, L"STATIC", text.c_str(), WS_VISIBLE | WS_CHILD, position.x, position.y, position.width, position.height, window->Hwnd(), nullptr, nullptr, nullptr);
            SetWindowLongPtrA(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
            SetWindowSubclass(hwnd, Static::ControlProc, 0, reinterpret_cast<DWORD_PTR>(this));
            UpdateFont();
        }
    }
}