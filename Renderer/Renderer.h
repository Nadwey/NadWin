#pragma once

#ifndef NW_RENDERER
#define NW_RENDERER

#ifdef NW_RENDERER_USE_DIRECT2D
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#else
#include <Windows.h>
#include <wingdi.h>
#endif

#endif // !NW_RENDERER