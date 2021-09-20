# Image::SetPixel(int, int, Pixel)

Sets selected pixel color.

## Syntax

```cpp
void SetPixel(
    int x,
    int y,
    Pixel pixel
);
```

## Parameters

`x`

The x-coordinate of the pixel to set.

`y`

The y-coordinate of the pixel to set.

`pixel`

[Pixel](./struct_Pixel.md) structure with color values

## Return value

None

## Remarks

Method doesn't check if the x or y coordinates are in the image, so it will probably throw an exception.

This function calls [Image::SetPixel(int x, int y, Pixel* pixel)](./class_Image_SetPixel(int,%20int,%20PixelPtr).md) with reference to `pixel` paramter

## See also

[struct Pixel](./struct_Pixel.md)
