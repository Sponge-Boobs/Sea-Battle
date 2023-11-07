#define STB_IMAGE_IMPLEMENTATION
#include "imgui_render.h"

static unsigned int stb_decompress_length(const unsigned char* input)
{
    return (input[8] << 24) + (input[9] << 16) + (input[10] << 8) + input[11];
}

static unsigned char* stb__barrier_out_e, * stb__barrier_out_b;
static const unsigned char* stb__barrier_in_b;
static unsigned char* stb__dout;
static void stb__match(const unsigned char* data, unsigned int length)
{
    // INVERSE of memmove... write each byte before copying the next...
    IM_ASSERT(stb__dout + length <= stb__barrier_out_e);
    if (stb__dout + length > stb__barrier_out_e) { stb__dout += length; return; }
    if (data < stb__barrier_out_b) { stb__dout = stb__barrier_out_e + 1; return; }
    while (length--) *stb__dout++ = *data++;
}

static void stb__lit(const unsigned char* data, unsigned int length)
{
    IM_ASSERT(stb__dout + length <= stb__barrier_out_e);
    if (stb__dout + length > stb__barrier_out_e) { stb__dout += length; return; }
    if (data < stb__barrier_in_b) { stb__dout = stb__barrier_out_e + 1; return; }
    memcpy(stb__dout, data, length);
    stb__dout += length;
}

#define stb__in2(x)   ((i[x] << 8) + i[(x)+1])
#define stb__in3(x)   ((i[x] << 16) + stb__in2((x)+1))
#define stb__in4(x)   ((i[x] << 24) + stb__in3((x)+1))

static const unsigned char* stb_decompress_token(const unsigned char* i)
{
    if (*i >= 0x20) { // use fewer if's for cases that expand small
        if (*i >= 0x80)       stb__match(stb__dout - i[1] - 1, i[0] - 0x80 + 1), i += 2;
        else if (*i >= 0x40)  stb__match(stb__dout - (stb__in2(0) - 0x4000 + 1), i[2] + 1), i += 3;
        else /* *i >= 0x20 */ stb__lit(i + 1, i[0] - 0x20 + 1), i += 1 + (i[0] - 0x20 + 1);
    }
    else { // more ifs for cases that expand large, since overhead is amortized
        if (*i >= 0x18)       stb__match(stb__dout - (stb__in3(0) - 0x180000 + 1), i[3] + 1), i += 4;
        else if (*i >= 0x10)  stb__match(stb__dout - (stb__in3(0) - 0x100000 + 1), stb__in2(3) + 1), i += 5;
        else if (*i >= 0x08)  stb__lit(i + 2, stb__in2(0) - 0x0800 + 1), i += 2 + (stb__in2(0) - 0x0800 + 1);
        else if (*i == 0x07)  stb__lit(i + 3, stb__in2(1) + 1), i += 3 + (stb__in2(1) + 1);
        else if (*i == 0x06)  stb__match(stb__dout - (stb__in3(1) + 1), i[4] + 1), i += 5;
        else if (*i == 0x04)  stb__match(stb__dout - (stb__in3(1) + 1), stb__in2(4) + 1), i += 6;
    }
    return i;
}

static unsigned int stb_adler32(unsigned int adler32, unsigned char* buffer, unsigned int buflen)
{
    const unsigned long ADLER_MOD = 65521;
    unsigned long s1 = adler32 & 0xffff, s2 = adler32 >> 16;
    unsigned long blocklen = buflen % 5552;

    unsigned long i;
    while (buflen) {
        for (i = 0; i + 7 < blocklen; i += 8) {
            s1 += buffer[0], s2 += s1;
            s1 += buffer[1], s2 += s1;
            s1 += buffer[2], s2 += s1;
            s1 += buffer[3], s2 += s1;
            s1 += buffer[4], s2 += s1;
            s1 += buffer[5], s2 += s1;
            s1 += buffer[6], s2 += s1;
            s1 += buffer[7], s2 += s1;

            buffer += 8;
        }

        for (; i < blocklen; ++i)
            s1 += *buffer++, s2 += s1;

        s1 %= ADLER_MOD, s2 %= ADLER_MOD;
        buflen -= blocklen;
        blocklen = 5552;
    }
    return static_cast<unsigned int>(s2 << 16) + static_cast<unsigned int>(s1);
}

static unsigned int stb_decompress(unsigned char* output, const unsigned char* i, unsigned int /*length*/)
{
    if (stb__in4(0) != 0x57bC0000) return 0;
    if (stb__in4(4) != 0)          return 0; // error! stream is > 4GB
    const unsigned int olen = stb_decompress_length(i);
    stb__barrier_in_b = i;
    stb__barrier_out_e = output + olen;
    stb__barrier_out_b = output;
    i += 16;

    stb__dout = output;
    for (;;) {
        const unsigned char* old_i = i;
        i = stb_decompress_token(i);
        if (i == old_i) {
            if (*i == 0x05 && i[1] == 0xfa) {
                IM_ASSERT(stb__dout == output + olen);
                if (stb__dout != output + olen) return 0;
                if (stb_adler32(1, output, olen) != static_cast<unsigned int>(stb__in4(2)))
                    return 0;
                return olen;
            }
            else {
                IM_ASSERT(0); /* NOTREACHED */
                return 0;
            }
        }
        IM_ASSERT(stb__dout <= output + olen);
        if (stb__dout > output + olen)
            return 0;
    }
}

bool load_png_texture(const uint8_t* png_start, const int png_size, int& image_width, int& image_height, ID3D11ShaderResourceView*& pShaderResource)
{
	int channels_in_file;

	auto pixels = stbi_load_from_memory(png_start, png_size, &image_width, &image_height, &channels_in_file, STBI_rgb_alpha);
	if (!pixels)
	{
		//stbi error
		return false;
	}

	D3D11_TEXTURE2D_DESC desc{
		static_cast<UINT>(image_width),
		static_cast<UINT>(image_height),
		1,
		1,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		{1, 0},
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE,
		0,
		0
	};

	ID3D11Texture2D* pTexture = nullptr;
	D3D11_SUBRESOURCE_DATA subResource{ pixels, desc.Width * 4, 0 };

	UI::GetDevice()->CreateTexture2D(&desc, &subResource, &pTexture);

	if (!pTexture)
	{
		//cant create 2D texture
		return false;
	}

	// Create texture view
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;

	UI::GetDevice()->CreateShaderResourceView(pTexture, &srvDesc, &pShaderResource);
	pTexture->Release();
	if (pShaderResource)
	{
		return true;
	}

	//cant create shader resource view
	return false;
}

bool load_png_texture_from_memory_compressed(const void* compressed_ttf_data, int compressed_ttf_size, int& image_width, int& image_height, ID3D11ShaderResourceView*& pShaderResource)
{
	const auto buf_decompressed_size = stb_decompress_length(static_cast<const unsigned char*>(compressed_ttf_data));
	const auto buf_decompressed_data = static_cast<unsigned char*>(IM_ALLOC(buf_decompressed_size));
	stb_decompress(buf_decompressed_data, static_cast<const unsigned char*>(compressed_ttf_data), static_cast<unsigned int>(compressed_ttf_size));
	const auto ret = load_png_texture(buf_decompressed_data, buf_decompressed_size, image_width, image_height, pShaderResource);
	return ret && pShaderResource != nullptr;
}

//void imgui_render::Initialize()
//{
//    ImGui::CreateContext();
//    ImGui_ImplWin32_Init(globals::g_hWindow);
//    ImGui_ImplDX11_Init(globals::g_pd3dDevice, globals::g_pd3dDeviceContext);
//    ImGuiIO& IO = ImGui::GetIO();
//    IO.IniFilename = nullptr;
//    IO.LogFilename = nullptr;
//
//    ImVector<ImWchar> ranges;
//    ImFontGlyphRangesBuilder builder;
//
//    // Add your custom ranges right here.
//    // https://github.com/ocornut/imgui/blob/master/docs/FONTS.md#using-custom-glyph-ranges
//
//    builder.AddRanges(IO.Fonts->GetGlyphRangesCyrillic());
//    builder.BuildRanges(&ranges);
//
//    std::basic_string<char> font_path = std::string{};
//
//    ImFontConfig cfg{};
//    //cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_Bitmap;
//    cfg.SizePixels = 13.f;
//
//    IO.Fonts->Clear();
//
//    IO.Fonts->Build();
//}