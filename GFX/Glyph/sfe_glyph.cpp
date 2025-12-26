#include "sfe_glyph.hpp"
#include "../../Platform/sfe_platform.hpp"

namespace GFX {
    Font Font::Create(const char* font_path, float font_point_scale, int first_codepoint, int last_codepoint) {
        Font font;

        size_t file_size = 0;
        Error error = Error::SUCCESS;
        u8* ttf_data = Platform::ReadEntireFile(font_path, file_size, error);
        RUNTIME_ASSERT_MSG(error == Error::SUCCESS, "%s\n", getErrorString(error));   
        RUNTIME_ASSERT(stbtt_InitFont(&font.info, ttf_data, stbtt_GetFontOffsetForIndex(ttf_data, 0)));

        font.scale = stbtt_ScaleForPixelHeight(&font.info, font_point_scale);

        int ascent, descent, lineGap;
        stbtt_GetFontVMetrics(&font.info, &ascent, &descent, &lineGap);
        font.ascent  = ascent  * font.scale;
        font.descent = descent * font.scale;
        font.line_gap = lineGap * font.scale;
        font.line_height = (ascent - descent + lineGap) * font.scale;

        font.glyphs = DS::Hashmap<int, Glyph>(last_codepoint - first_codepoint);
        for (int codepoint = first_codepoint; codepoint <= last_codepoint; codepoint++) {
            Glyph glyph;

            int advance, lsb;
            stbtt_GetCodepointHMetrics(&font.info, codepoint, &advance, &lsb);
            glyph.x_advance = advance * font.scale;

            if (codepoint == ' ') {
                font.glyphs.put(codepoint, glyph);
                continue;
            }

            u8* bmp = stbtt_GetCodepointBitmap(
                &font.info,
                0, font.scale,
                codepoint,
                &glyph.width,
                &glyph.height,
                &glyph.x_baseline_offset,
                &glyph.y_baseline_offset
            );

            // Texture::FlipVerticallyInPlace(bmp, glyph.width, glyph.height);
            glyph.texture = Texture::LoadFromMemory(bmp, glyph.width, glyph.height, 1, false);

            float xpos = 0;
            float ypos = glyph.y_baseline_offset;
            float w = glyph.width;
            float h = glyph.height;

            Math::Vec3 tr = Math::Vec3(xpos + w, ypos + h, +0.0f);
            Math::Vec3 br = Math::Vec3(xpos + w, ypos,     +0.0f);
            Math::Vec3 bl = Math::Vec3(xpos,     ypos,     +0.0f);
            Math::Vec3 tl = Math::Vec3(xpos,     ypos + h, +0.0f);
            glyph.quad = GFX::Geometry::Quad(tr, br, bl, tl);
            font.glyphs.put(codepoint, glyph);
            stbtt_FreeBitmap(bmp, nullptr);
        }

        return font;
    }
}