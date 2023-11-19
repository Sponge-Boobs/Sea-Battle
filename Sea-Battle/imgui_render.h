#ifndef IMGUI_RENDER_H
#define IMGUI_RENDER_H

//class Color;
class Vector;
struct ImRect;
typedef int ImTextFlag;
typedef int ImCustomFont;

enum ImTextFlag_
{
	ImTextFlag_None = 0,
	ImTextFlag_CenteredX = 1 << 0,
	ImTextFlag_CenteredY = 1 << 1,
	ImTextFlag_Outlined = 1 << 2,
	ImTextFlag_Dropshadow = 1 << 3,
	ImTextFlag_AutoY = 1 << 4,
	ImTextFlag_COUNT
};

enum ImCustomFont_
{
	ImCustomFont_default = 0,
	ImCustomFont_Tahoma9,
	ImCustomFont_Tahoma12,
	ImCustomFont_Tahoma13,
	ImCustomFont_Tahoma14,
	ImCustomFont_Tahoma15,
	ImCustomFont_Tahoma16,
	ImCustomFont_Tahoma25,
	ImCustomFont_Icons,
	ImCustomFont_Tabs,
	ImCustomFont_Arial,
	ImCustomFont_COUNT
};

#include "Includes.h"
#include "UI/UI.h"
#include "SDK/stb_image.h"

bool load_png_texture(const uint8_t* png_start, int png_size, int& image_width, int& image_height, ID3D11ShaderResourceView*& pShaderResource);
bool load_png_texture_from_memory_compressed(const void* compressed_ttf_data, int compressed_ttf_size, int& image_width, int& image_height, ID3D11ShaderResourceView*& pShaderResource);

namespace imgui_render
{
    void Initialize();

	inline ImFont* m_pFont[ImCustomFont_COUNT] = {};
    inline bool bInitialize = false;
}

#endif