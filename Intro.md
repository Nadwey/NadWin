## Creating a window

```cpp
#include "NadWin/src/NadWin.hpp"

int main()
{
	NW::NApp app("Example app");

	NW::NWindow window(
        "Example window",
        10, // x position
        10, // y position
        800, // width
        600 // height
    );
	window.Show();

	return app.MessageLoop();
}
```