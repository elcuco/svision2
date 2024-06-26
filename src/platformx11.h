/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <map>
#include <memory>

#include <X11/X.h>
#include <X11/Xlib.h>

#include <platform.h>

struct PlatformWindowX11;

#define SVISION_X11_TTF_PATH "/usr/share/fonts/truetype/"
#define SVISION_X11_TTF_FILENAME "freefont/FreeSans.ttf"
// #define SVISION_X11_TTF_FILENAME "dejavu/DejaVuSans.ttf"

struct PlatformX11 : Platform {
    std::map<Window, std::shared_ptr<PlatformWindowX11>> windows;
    std::map<MouseCursor, Cursor> cursor_cache;

    Display *dpy = nullptr;
    int screen = -1;
    Atom wmDeleteMessage = None;

    PlatformX11();

    virtual auto platform_init() -> void override;
    virtual auto done() -> void override;
    virtual auto open_window(int x, int y, int width, int height, const std::string_view title)
        -> std::shared_ptr<PlatformWindow> override;
    virtual auto show_window(std::shared_ptr<PlatformWindow> window) -> void override;

    virtual auto set_cursor(PlatformWindow &window, MouseCursor cursor) -> void override;
    virtual auto clear_cursor_cache() -> void override;
    virtual auto invalidate(PlatformWindow &window) -> void override;
    virtual auto main_loop() -> void override;
};

using ThePlatform = PlatformX11;
