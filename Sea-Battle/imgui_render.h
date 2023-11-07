#ifndef IMGUI_RENDER_H
#define IMGUI_RENDER_H

#include "Includes.h"
#include "UI/UI.h"
#include "SDK/stb_image.h"

bool load_png_texture(const uint8_t* png_start, int png_size, int& image_width, int& image_height, ID3D11ShaderResourceView*& pShaderResource);
bool load_png_texture_from_memory_compressed(const void* compressed_ttf_data, int compressed_ttf_size, int& image_width, int& image_height, ID3D11ShaderResourceView*& pShaderResource);

namespace imgui_render
{
    void Initialize();

    inline bool bInitialize = false;
}

#endif