# Image::SwapPixel(int, int, int, int)

Swaps pixels colors

## Syntax

```cpp
void SwapPixel(
    int x0,
    int y0,
    int x1,
    int y1
);
```

## Parameters

`x0`

The x-coordinate of the first pixel to swap.

`y0`

The y-coordinate of the first pixel to swap.

`x1`

The x-coordinate of the second pixel to swap.

`y1`

The y-coordinate of the second pixel to swap.

## Return value

None

## Remarks

Method doesn't check if the coordinates are in the image, so it will probably throw an exception.
