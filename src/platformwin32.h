/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <map>
#include <platform.h>

struct PlatformWin32 : Platform {
    std::map<MouseCursor, void *> cursor_cache;

    PlatformWin32();
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

using ThePlatform = PlatformWin32;
