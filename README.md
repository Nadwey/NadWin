# NadWin

A Blazingly fast🚀, small⚡, cross-platform✝️, header-only📦, easy-to-use🤝, simple👌, modern🔥, beautiful🌈, elegant👑, powerful💪, intuitive🧠, and flexible🧘‍♂️ GUI library for C++. 

## Hello World in NadWin

```cpp
#include "nadwin/nadwin.hpp"

int main()
{
	nadwin::App app;

	nadwin::Window window;
	window.SetTitle("Hello World");
	window.Show();

	return app.Run();
}
```

## TODO

- [ ] Windows implementation
	- [x] Window creation
	- [ ] WinGDI rendering
	- [ ] Direct2D or whatever rendering
- [ ] Linux (Xorg, Wayland) implementation
	- [ ] Window creation
	- [ ] ...?
	- [ ] Profit
- [ ] macOS implementation
	- [ ] Figure out how to get a Mac
- [ ] Make controls
- [ ] Make a layout system (probably will be a separate library) or steal one from somewhere