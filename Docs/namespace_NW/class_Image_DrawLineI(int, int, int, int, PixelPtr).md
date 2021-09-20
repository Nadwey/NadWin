# Image::DrawLineI(int, int, int, int, Pixel*)

Sets selected pixel color.

## Syntax

```cpp
void DrawLineI(
    int x0,
    int y0,
    int x1,
    int y1,
    Pixel* pixel
);
```

## Parameters

`x0`

the x-coordinate of the line starting point.

`y0`

the y-coordinate of the line starting point.

`x1`

the x-coordinate of the line ending point.

`y1`

the y-coordinate of the line ending point.

`pixel`

Pointer to [Pixel](./struct_Pixel.md) structure with color values.

## Return value

None

## Remarks

Method doesn't check if the x or y coordinates are in the image, so it will probably throw an exception.

## See also

[struct Pixel](./struct_Pixel.md)
