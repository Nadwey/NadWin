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
#include <exception>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commctrl.h>

#if _WIN32_WINNT < 0x0501
#error Minimum _WIN32_WINNT version is 0x0501 (Windows XP)
// You can try to define your own version above #include <windows.h>
#else

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

        // Właściwe UI

        class App {
        public:
            App(std::wstring AppName);
            App(std::string AppName);
            ~App();

            WPARAM MessageLoop();
            std::wstring GetAppName();

            static void DoEvents();
        private:
            void registerClass(std::wstring&);
            static LRESULT CALLBACK proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

            static std::wstring AppName;
            static HINSTANCE hInstance;
            static bool initialized;

            friend class Window;
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
            Size,
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
            Window(std::string WindowName, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int width = CW_USEDEFAULT, int height = CW_USEDEFAULT);
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

            void Move(int x, int y, int width, int height, bool repaint = true);
            void Show();

            void Add(Control* control);

            void Repaint();

            std::function<void(WindowEventTypes, WindowEventInfo*)> EventHandler;
        private:
            void createWindow(std::wstring& WindowName, int x, int y, int width, int height);
            LRESULT CALLBACK proc(UINT msg, WPARAM wParam, LPARAM lParam);

            HWND hwnd = nullptr;
            bool isOver = false;

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
            Destroy,
            FromParent_Command // Odbierane z wyższego okna
        };

        // Controls

        struct ControlEventInfo {
        public:
            void OverrideProcResult(LRESULT result);

            UINT uMsg = 0;
            WPARAM wParam = 0;
            LPARAM lParam = 0;
            Control* control = nullptr;
            void* additionalData = nullptr;
        private:
            bool overrideProcResult = false;
            LRESULT result = 0;

            friend class Control;
        };

        class Control
        {
        public:
            ~Control();

            std::function<void(ControlEventTypes, ControlEventInfo*)> EventHandler;

            void Repaint();

            void SetFont(Font* font);
            void SetFont(Font font);

            virtual std::wstring GetText();
            virtual LRESULT GetTextLength();
            virtual void SetText(std::wstring text);

            void Focus();
            void RemoveFocus();

            HWND Hwnd();
            void Destroy();

            Position GetPosition();
            void SetPosition(Position position, bool repaint = true);
        protected:
            Control();

            // (parent)
            Window* window = nullptr;
            HWND hwnd = nullptr;
            bool isOver = false;
            std::wstring initText;
            Position position;

            virtual void initialize(Position& position, std::wstring& text);
            virtual void create();
            virtual void setWindowValues();

            static LRESULT CALLBACK ControlProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR);
        private:
            friend class Window;
            friend struct ControlEventInfo;
        };

        class Button : public Control
        {
        public:
            Button(Window* window, Position position, std::string text);
            Button(Window* window, Position position, std::wstring text);

        private:
            void create() override;
        };

        class CheckBox : public Control
        {
        public:
            CheckBox(Window* window, Position position, std::string text);
            CheckBox(Window* window, Position position, std::wstring text);

            void SetChecked(bool checked);
            bool GetChecked();
            void ToggleChecked();
        private:
            void create() override;
        };

        class ComboBox : public Control
        {
        public:
            ComboBox(Window* window, Position position, std::string text);
            ComboBox(Window* window, Position position, std::wstring text);

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
            void initialize(Position& position, std::wstring& text);
            void create() override;
        };

        class DatePicker : public Control
        {
        public:
            DatePicker(Window* window, Position position, std::string text);
            DatePicker(Window* window, Position position, std::wstring text);

            void SetTime(SYSTEMTIME time);
            SYSTEMTIME GetTime(bool* valid = nullptr);
            void SetFormat(std::wstring format);
            void SetMin(SYSTEMTIME time);
            void SetMax(SYSTEMTIME time);
            SYSTEMTIME GetMin();
            SYSTEMTIME GetMax();

        private:
            void create() override;
        };

        enum class TextBoxTextAlign {
            Left = 0,
            Center,
            Right
        };

        struct Range {
            int start;
            int end;
        };

        class TextBoxBase : public Control {
        public:
            void SetTextAlign(TextBoxTextAlign align);
            TextBoxTextAlign GetTextAlign();

            void SetReadOnly(bool readOnly);
            void SetSelection(Range selection);
            Range GetSelection();
        };

        class TextBoxMultiline : public TextBoxBase
        {
        public:
            TextBoxMultiline(Window* window, Position position, std::string text);
            TextBoxMultiline(Window* window, Position position, std::wstring text);

            LRESULT GetLineIndex(LRESULT line);
            LRESULT GetLineLength(LRESULT line);
            LRESULT GetLineCount();

        private:
            void initialize(Position& position, std::wstring& text);
            void create() override;
        };

        class TextBoxSingleline : public TextBoxBase
        {
        public:
            TextBoxSingleline(Window* window, Position position, std::string text);
            TextBoxSingleline(Window* window, Position position, std::wstring text);

            void SetPasswordMode(bool passwordMode);
            bool GetPasswordMode();

        private:
            void create() override;
        };

        class ListBox : public Control
        {
        public:
            ListBox(Window* window, Position position, std::string text);
            ListBox(Window* window, Position position, std::wstring text);

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
            void create() override;
        };

        enum class ProgressBarState {
            Normal = 0,
            Error,
            Paused
        };

#if _WIN32_WINNT >= 0x0600

        class ProgressBar : public Control
        {
        public:
            ProgressBar(Window* window, Position position, std::string text);
            ProgressBar(Window* window, Position position, std::wstring text);

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
            void create() override;
        };

#endif

        class Static : public Control
        {
        public:
            Static(Window* window, Position position, std::string text);
            Static(Window* window, Position position, std::wstring text);

        private:
            void create() override;
        };
    }
}
#endif

#endif // !_NADWIN_
