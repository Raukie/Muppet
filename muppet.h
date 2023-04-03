#pragma once

#ifdef _WIN32
#include <windows.h>
extern "C" _declspec(dllexport) DWORD NvOptimusEnablement = true;
#endif // __WIN32

#include "window.h"
#include "graphics.h"
