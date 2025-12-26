#pragma once

#include "../Texture/sfe_texture.hpp"
#include "../Geometry/sfe_geometry.hpp"
#include <stb_truetype.h>

namespace GFX {
    struct Glyph {
        int width, height;
        int x_baseline_offset;
        int y_baseline_offset;
        float x_advance;
        Texture texture;
        GFX::Geometry quad;
    };

    struct Font {
        stbtt_fontinfo info;
        float scale;

        float ascent;
        float descent;
        float line_gap;
        float line_height;

        DS::Hashmap<int, Glyph> glyphs;

        static Font Create(const char* font_path, float font_point_scale, int first_codepoint, int last_codepoint);
    };
}
