/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "fontprovider.h"

// this file is autogenerated from old bitmap fonts
#include "fontdos.h"

auto static write_fixed_char(Bitmap &bitmap, Position position, char c, const uint32_t color)
    -> void {
    // const unsigned char *font = ATIx550_8x16_bin;
    const unsigned char *font = IBM_VGA_8x16_bin;

    for (auto y = 0; y < 16; y++) {
        unsigned char line = font[16 * (unsigned char)c + y];
        for (auto x = 0; x < 8; x++) {
            auto xx = 8 - x;
            if (get_bit(line, xx)) {
                bitmap.put_pixel(position.x + x, position.y + y, color);
            }
        }
    }
}

auto FontProviderFixed::write(Bitmap &bitmap, Position position, const std::string_view str,
                              const uint32_t color) -> void {
    for (auto c : str) {
        write_fixed_char(bitmap, position, c, color);
        position.x += 8;
    }
}
auto FontProviderFixed::text_size(const std::string_view str) -> Size {
    return Size{static_cast<int>(str.length()) * 8, 16};
}