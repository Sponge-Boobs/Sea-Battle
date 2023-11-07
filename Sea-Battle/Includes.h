#pragma once

// global includes (WinApi)
#include <iostream>
#include <Windows.h>

// other (name later for includes...)
#include <tchar.h>

// ImGui includes
#include "SDK/ImGui/imgui.h"
#include "SDK/ImGui/imgui_impl_dx11.h"
#include "SDK/ImGui/imgui_impl_win32.h"

#include "SDK/color.h"

// DirectX includes
#include <d3d11.h>
#include <d3dx11tex.h>
#include <DirectXMath.h>


// Libs
#pragma comment(lib, "netapi32.lib")
#pragma comment(lib, "advapi32.lib")
// ---
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "dwmapi.lib")

// Delete std::
using namespace std;