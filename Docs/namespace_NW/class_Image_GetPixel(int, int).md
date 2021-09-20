# Image::GetPixel(int, int)

Gets pixel selected color.

## Syntax

```cpp
Pixel GetPixel(
    int x,
    int y
);
```

## Parameters

`x`

The x-coordinate of the pixel to retrieve.

`y`

The y-coordinate of the pixel to retrieve.

## Return value

Color of the pixel

## Remarks

Method doesn't check if the x or y coordinates are in the image, so it will probably throw an exception or return some garbage data.

## See also

[struct Pixel](./struct_Pixel.md)
