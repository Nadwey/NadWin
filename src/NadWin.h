// https://github.com/Nadwey/NadWin

#pragma once
#ifndef _NADWIN_
#define _NADWIN_

#include <string>
#include <time.h>
#include <random>
#include <math.h>
#include <functional>
#include <locale>
#include <codecvt>
#include <vector>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commctrl.h>
#include <iostream>


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
            Focus,
            RemoveFocus,
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
            std::function<void(ControlEventTypes, ControlEventInfo*)> EventHandler;

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

            HWND Hwnd();
            void Destroy();
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
            friend struct ControlEventInfo;
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
        };

        class CheckBox : public Control
        {
        public:
            CheckBox(Window* window, Position position, std::string text);
            CheckBox(Window* window, Position position, std::wstring text);
            ~CheckBox();

            void SetChecked(bool checked);
            bool GetChecked();
            void ToggleChecked();
        private:
            void initialize(Position& position, std::wstring& text);
            void create() override;
        };

        class ComboBox : public Control
        {
        public:
            ComboBox(Window* window, Position position, std::string text);
            ComboBox(Window* window, Position position, std::wstring text);
            ~ComboBox();

            LRESULT			AddString(std::wstring str);
            bool			DeleteString(LRESULT index);
            bool			DeleteString(std::wstring str);
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
            ~DatePicker();

            void SetTime(SYSTEMTIME time);
            SYSTEMTIME GetTime(bool* valid = nullptr);
            void SetFormat(std::wstring format);
            void SetMin(SYSTEMTIME time);
            void SetMax(SYSTEMTIME time);
            SYSTEMTIME GetMin();
            SYSTEMTIME GetMax();

        private:
            void initialize(Position& position, std::wstring& text);
            void create() override;
        };

        enum class TextBoxTextAlign {
            Left = 0,
            Center,
            Right
        };

        struct Selection {
            DWORD start;
            DWORD end;
        };

        class TextBoxBase : public Control {
        public:
            void SetTextAlign(TextBoxTextAlign align);
            TextBoxTextAlign GetTextAlign();

            void SetReadOnly(bool readOnly);
            void SetSelection(Selection selection);
            Selection GetSelection();
        };

        class TextBoxMultiline : public TextBoxBase
        {
        public:
            TextBoxMultiline(Window* window, Position position, std::string text);
            TextBoxMultiline(Window* window, Position position, std::wstring text);
            ~TextBoxMultiline();

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
            ~TextBoxSingleline();

            void SetPasswordMode(bool passwordMode);
            bool GetPasswordMode();

        private:
            void initialize(Position& position, std::wstring& text);
            void create() override;
        };

        class ListBox : public Control
        {
        public:
            ListBox(Window* window, Position position, std::string text);
            ListBox(Window* window, Position position, std::wstring text);
            ~ListBox();

            LRESULT			AddString(std::wstring str);
            bool			DeleteString(LRESULT index);
            bool			DeleteString(std::wstring str);
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

        private:
            void initialize(Position& position, std::wstring& text);
            void create() override;
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
        };
    }
}


#endif // !_NADWIN_