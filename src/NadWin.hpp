// https://github.com/Nadwey/NadWin

#pragma once
#ifndef _NADWIN_
#define _NADWIN_

#ifndef UNICODE
#define UNICODE
#endif

#include <string>
#include <functional>
#include <locale>
#include <codecvt>
#include <stdexcept>
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>

#ifdef NW_ENABLE_DIRECTX_FEATURES
#define CANVAS2D_CLASS_NAME L"CANVAS2D"
#include <d2d1.h>
#include <dwrite.h>
#endif


// more information about _WIN32_WINNT -> https://docs.microsoft.com/en-us/cpp/porting/modifying-winver-and-win32-winnt?view=msvc-170
#ifndef _WIN32_WINNT
#error _WIN32_WINNT is not defined
#elif _WIN32_WINNT < 0x0501
#error Minimum _WIN32_WINNT version is 0x0501 (Windows XP)
// You can try to define your own version above #include <windows.h>
#else

namespace NW
{
    namespace UI {
        LONG_PTR GetStyle(HWND hwnd);
        void SetStyle(HWND hwnd, LONG_PTR style);
        bool HasStyle(HWND hwnd, LONG_PTR style);
        void AppendStyle(HWND hwnd, LONG_PTR style);
        void RemoveStyle(HWND hwnd, LONG_PTR style);
        LONG_PTR GetExStyle(HWND hwnd);
        void SetExStyle(HWND hwnd, LONG_PTR style);

        struct Range {
            int start;
            int end;
        };

        class Font
        {
        public:
            Font() = default;
            Font(int height, std::wstring faceName, int width = 0, bool italic = false, bool underline = false, bool strike = false);
            ~Font();

            void SetHeight(int height);
            void SetFaceName(std::wstring faceName);
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
            operator RECT() { return Rect(); }

            int x = 0;
            int y = 0;
            int width = 0;
            int height = 0;
        };

        // Właściwe UI

        class App {
        public:
            App(std::wstring AppName);
            ~App();

            static WPARAM MessageLoop();
            static std::wstring GetAppName();

            static void DoEvents();
        private:
            void registerClass(std::wstring&);
            static LRESULT CALLBACK proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

            static std::wstring AppName;
            static HINSTANCE hInstance;
            static bool initialized;
#ifdef NW_ENABLE_DIRECTX_FEATURES
            static ID2D1Factory* pD2DFactory;
#endif

            friend class Window;
#ifdef NW_ENABLE_DIRECTX_FEATURES
            friend class Canvas2D;
#endif
        };


        // Definicja definicji ;-;
        class Window;
        class Control;
        class Button;

        enum class WindowEventTypes
        {
            Undefined = 0,
            MouseMove,
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
            MouseLeave,
            KeyDown,
            KeyUp,
            KeyChar,
            Focus,
            RemoveFocus,
            Move,
            Moving,
            Size,
            Sizing,
            Paint,
            Destroy
        };

        struct WindowEventInfo {
        public:
            void OverrideProcResult(LRESULT result);

            UINT uMsg = 0;
            WPARAM wParam = 0;
            LPARAM lParam = 0;
            Window* control = nullptr;
            void* additionalData = nullptr;
        private:
            bool overrideProcResult = false;
            LRESULT result = 0;

            friend class Window;
        };

        class Window {
        public:
            Window(std::wstring WindowName, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int width = CW_USEDEFAULT, int height = CW_USEDEFAULT);
            ~Window();

            const HWND Hwnd();

            std::wstring GetText();
            LRESULT GetTextLength();
            void SetText(std::wstring text);

            void SetCanResize(bool resize);
            void SetCanMinimize(bool minimize);
            void SetCanMaximize(bool maximize);

            bool GetCanResize();
            bool GetCanMinimize();
            bool GetCanMaximize();

            Position GetPosition();
            void SetPosition(Position position, bool repaint = true);
            void Show();
            int GetWidth();
            int GetHeight();
            Position GetClientArea();

            void Repaint();

            void SetDefaultFont(Font* font);

            std::function<void(WindowEventTypes, WindowEventInfo*)> EventHandler;
            COLORREF backgroundColor = 0xffffff;
        private:
            void createWindow(std::wstring& WindowName, int x, int y, int width, int height);
            LRESULT CALLBACK proc(UINT msg, WPARAM wParam, LPARAM lParam);

            HWND hwnd = nullptr;
            bool isOver = false;
            Font defaultFont;

            friend class App;
            friend class Control;
            friend struct WindowEventInfo;
        };

        enum class ControlEventTypes
        {
            Undefined = 0,
            MouseMove,
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
            MouseLeave,
            KeyDown,
            KeyUp,
            KeyChar,
            Focus,
            RemoveFocus,
            Move,
            Moving,
            Size,
            Sizing,
            Destroy,
            Paint,
            FromParent_Command // Odbierane z wyższego okna
        };

        // Controls

        struct ControlEventInfo {
        public:
            void OverrideProcResult(LRESULT result);

            MSG* msg = nullptr;
            Control* control = nullptr;
            void* additionalData = nullptr;
        private:
            bool overrideProcResult = false;
            LRESULT result = 0;

            friend class Control;
            friend class Canvas2D;
        };

        class Control
        {
        public:
            virtual ~Control();

            std::function<void(ControlEventTypes, ControlEventInfo*)> EventHandler;

            void Repaint();

            void SetFont(Font* font);

            virtual std::wstring GetText();
            virtual LRESULT GetTextLength();
            virtual void SetText(std::wstring text);

            void Focus();
            void RemoveFocus();

            void SetEnabled(bool enabled);
            bool GetEnabled();

            HWND Hwnd();
            void Destroy();

            Position GetPosition();
            void SetPosition(Position position, bool repaint = true);
            int GetWidth();
            int GetHeight();
            Position GetClientArea();

            void* userData;
        protected:
            Control();

            // (parent)
            Window* window = nullptr;
            HWND hwnd = nullptr;
            bool isOver = false;

            virtual void create(std::wstring text, Position position, LONG_PTR customStyles);
            virtual void setWindowValues();
            virtual LRESULT processMessage(ControlEventInfo* eventInfo);
            static void handleEvents(ControlEventInfo* eventInfo);

            static LRESULT CALLBACK ControlProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR);
        private:
            friend class Window;
            friend class Canvas2D;
            friend struct ControlEventInfo;
        };

        class Button : public Control
        {
        public:
            Button(Window* window, Position position, std::wstring text, LONG_PTR customStyles = 0);

        private:
            void create(std::wstring text, Position position, LONG_PTR customStyles) override;
        };

#ifdef NW_ENABLE_DIRECTX_FEATURES
        class Canvas2D : public Control
        {
        public:
            Canvas2D(Window* window, Position position, std::wstring text, LONG_PTR customStyles = 0);
            ~Canvas2D();

            ID2D1HwndRenderTarget* GetHwndRenderTarget();

        private:
            void create(std::wstring text, Position position, LONG_PTR customStyles) override;
            void setWindowValues() override;
            LRESULT processMessage(ControlEventInfo* eventInfo) override;
            static LRESULT canvasProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

            ID2D1HwndRenderTarget* pRT = nullptr;

            friend class App;
            friend class Control;
            friend struct ControlEventInfo;
        };
#endif

        class CheckBox : public Control
        {
        public:
            CheckBox(Window* window, Position position, std::wstring text, LONG_PTR customStyles = 0);

            void SetChecked(bool checked);
            bool GetChecked();
            void ToggleChecked();
        private:
            void create(std::wstring text, Position position, LONG_PTR customStyles) override;
            LRESULT processMessage(ControlEventInfo* eventInfo) override;
        };

        class ComboBox : public Control
        {
        public:
            ComboBox(Window* window, Position position, std::wstring text, LONG_PTR customStyles = 0);

            void SetText(std::wstring text) override;

            LRESULT			AddString(std::wstring str);
            void			DeleteString(LRESULT index);
            void			DeleteString(std::wstring str);
            LRESULT			FindString(std::wstring str);
            LRESULT			GetCount();
            LRESULT			GetStringLength(LRESULT index);
            std::wstring	GetString(LRESULT index);
            LRESULT			GetSelected();
            std::wstring	GetSelectedString();
            void			SetSelected(LRESULT index);
            void			SetSelected(std::wstring str);
            void			ShowDropdown(bool show);

        private:
            void create(std::wstring text, Position position, LONG_PTR customStyles) override;
        };

        class DatePicker : public Control
        {
        public:
            DatePicker(Window* window, Position position, std::wstring text, LONG_PTR customStyles = 0);

            void SetTime(SYSTEMTIME time);
            SYSTEMTIME GetTime(bool* valid = nullptr);
            void SetFormat(std::wstring format);
            void SetMin(SYSTEMTIME time);
            void SetMax(SYSTEMTIME time);
            SYSTEMTIME GetMin();
            SYSTEMTIME GetMax();

        private:
            void create(std::wstring text, Position position, LONG_PTR customStyles) override;
        };

        class ListBox : public Control
        {
        public:
            ListBox(Window* window, Position position, std::wstring text, LONG_PTR customStyles = 0);

            void SetText(std::wstring text) override;

            LRESULT			AddString(std::wstring str);
            void			DeleteString(LRESULT index);
            void			DeleteString(std::wstring str);
            LRESULT			FindString(std::wstring str);
            LRESULT			GetCount();
            LRESULT			GetStringLength(LRESULT index);
            std::wstring	GetString(LRESULT index);
            LRESULT			GetSelected();
            bool            IsSelected();
            std::wstring	GetSelectedString();
            void			SetSelected(LRESULT index);
            void			SetSelected(std::wstring str);
            void            SetSort(bool sort);
            bool            GetSort();
            void            SetTopIndex(LRESULT index);
            LRESULT         GetTopIndex();

        private:
            void create(std::wstring text, Position position, LONG_PTR customStyles) override;
        };

        class ImageBox : public Control
        {
        public:
            ImageBox(Window* window, Position position, std::wstring text, LONG_PTR customStyles = 0);


        private:
            void create(std::wstring text, Position position, LONG_PTR customStyles) override;
        };

#if _WIN32_WINNT >= 0x0600

        enum class ProgressBarState {
            Normal = 0,
            Error,
            Paused
        };

        class ProgressBar : public Control
        {
        public:
            ProgressBar(Window* window, Position position, std::wstring text, LONG_PTR customStyles = 0);

            void SetRange(Range range);
            Range GetRange();
            void SetStep(int step);
            int GetStep();
            void Step();
            void SetPos(UINT pos);
            UINT GetPos();
            void SetState(ProgressBarState state);
            ProgressBarState GetState();
            void SetMarquee(bool enabled, int updateTime = 30);
        private:
            void create(std::wstring text, Position position, LONG_PTR customStyles) override;
        };

#endif

        class Static : public Control
        {
        public:
            Static(Window* window, Position position, std::wstring text, LONG_PTR customStyles = 0);

        private:
            void create(std::wstring text, Position position, LONG_PTR customStyles) override;
        };

        enum class TextBoxTextAlign {
            Left = 0,
            Center,
            Right
        };

        class TextBoxBase : public Control {
        public:
            void SetTextAlign(TextBoxTextAlign align);
            TextBoxTextAlign GetTextAlign();

            void SetReadOnly(bool readOnly);
            void SetSelection(Range selection);
            Range GetSelection();
#if _WIN32_WINNT >= 0x0600
            // Common Controls >= 6.0
            void SetPlaceholder(std::wstring text);
#endif
        };

        class TextBoxMultiline : public TextBoxBase
        {
        public:
            TextBoxMultiline(Window* window, Position position, std::wstring text, LONG_PTR customStyles = 0);

            LRESULT GetLineIndex(LRESULT line);
            LRESULT GetLineLength(LRESULT line);
            LRESULT GetLineCount();

        private:
            void create(std::wstring text, Position position, LONG_PTR customStyles) override;
        };

        class TextBoxSingleline : public TextBoxBase
        {
        public:
            TextBoxSingleline(Window* window, Position position, std::wstring text, LONG_PTR customStyles = 0);

            void SetPasswordMode(bool passwordMode);
            bool GetPasswordMode();

        private:
            void create(std::wstring text, Position position, LONG_PTR customStyles) override;
        };

#if _WIN32_WINNT >= 0x0600
        class TrackBar : public Control
        {
        public:
            TrackBar(Window* window, Position position, std::wstring text, LONG_PTR customStyles = 0);

            void SetMin(int min, bool redraw = true);
            void SetMax(int max, bool redraw = true);
            int GetMin();
            int GetMax();
            void SetPos(int pos, bool redraw = true);
            int GetPos();
            void SetTickFrequency(int frequency);
            void SetTicksVisibility(bool visible);
            bool GetTicksVisibility();

        private:
            void create(std::wstring text, Position position, LONG_PTR customStyles) override;
        };
#endif
    }
}
#endif

#endif // !_NADWIN_