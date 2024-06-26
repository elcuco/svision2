/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <bitmap.h>
#include <buttonstates.h>
#include <checkboxshape.h>
#include <fontprovider.h>
#include <memory>

struct ColorStyle {
    int32_t window_background = 0;

    int32_t frame_normal_color1 = 0;
    int32_t frame_normal_color2 = 0;
    int32_t frame_normal_color3 = 0;
    int32_t frame_normal_color4 = 0;

    int32_t frame_hover_color1 = 0;
    int32_t frame_hover_color2 = 0;
    int32_t frame_hover_color3 = 0;
    int32_t frame_hover_color4 = 0;

    int32_t frame_selected_color1 = 0;
    int32_t frame_selected_color2 = 0;
    int32_t frame_selected_color3 = 0;
    int32_t frame_selected_color4 = 0;

    int32_t frame_disabled_color1 = 0;
    int32_t frame_disabled_color2 = 0;
    int32_t frame_disabled_color3 = 0;
    int32_t frame_disabled_color4 = 0;

    int32_t input_background_normal = 0;
    int32_t input_background_hover = 0;
    int32_t input_background_selected = 0;
    int32_t input_background_disabled = 0;

    int32_t button_background_1 = 0;
    int32_t button_background_2 = 0;
    int32_t button_selected_background = 0;
    int32_t button_selected_text = 0;

    int32_t text_color = 0;
    int32_t text_color_disabled = 0;

    int32_t text_selection_color = 0;
    int32_t text_selection_background = 0;
    int32_t text_selection_background_hover = 0;
};

struct TabHeaderOffsets {
    int offset;
    int width;
};

enum class PaddingStyle { Label, Button, Checkbox, ScrollBar, TabHeader };

struct Theme {
    ColorStyle colors = {};
    std::shared_ptr<FontProvider> font;
    LayoutParams defaultPadding = {10, 10, 10, 10};

    explicit Theme(std::shared_ptr<FontProvider> f) : font(f) {}
    virtual ~Theme() = default;

    auto draw_frame(Bitmap &content, Position position, Size size, FrameStyles style,
                    FrameSize frame_size) -> void;
    auto draw_tabs(Bitmap &content, bool has_focus, int selected_index, int hover_index,
                   const LayoutParams &padding, const std::vector<std::string> &names)
        -> std::vector<TabHeaderOffsets>;

    virtual auto init() -> void = 0;
    virtual auto draw_widget_background(Bitmap &content, bool has_focus) -> void = 0;
    virtual auto draw_window_background(Bitmap &content) -> void = 0;
    virtual auto draw_scrollbar_background(Bitmap &content) -> void = 0;
    virtual auto draw_button(Bitmap &content, bool has_focus, bool is_default, bool is_enabled,
                             bool has_frame, ButtonStates state, const std::string_view text,
                             const std::shared_ptr<Bitmap> bitmap) -> void = 0;
    virtual auto draw_checkbox(Bitmap &content, bool has_focus, bool is_enabled, bool is_checked,
                               ButtonStates state, const std::string_view text, CheckboxShape shape,
                               const LayoutParams &padding) -> void = 0;

    // TODO - missing disabled state
    virtual auto draw_input_background(Bitmap &content, const bool has_focus) -> void = 0;

    virtual auto draw_listview_background(Bitmap &content, const bool has_focus,
                                          bool draw_background) -> void = 0;
    virtual auto draw_listview_item(Bitmap &content, const std::string_view text,
                                    const ItemStatus status, const bool is_hover) -> void = 0;
    virtual auto draw_single_tab(Bitmap &content, const int offset, const bool is_active,
                                 const bool is_hover, const LayoutParams &padding,
                                 const std::string_view name) -> int = 0;

    virtual auto needs_frame_for_focus() const -> bool = 0;
    virtual auto scrollbar_size() const -> int = 0;

    virtual auto modify_frame_on_hover() const -> bool { return true; }

    virtual auto get_padding(PaddingStyle t = PaddingStyle::Label) -> LayoutParams {
        (void)(t);
        return defaultPadding;
    }
};

// A windows 9x look and feel based theme
struct ThemeRedmond : Theme {
    static auto get_light_colors() -> ColorStyle;
    static auto get_dark_colors() -> ColorStyle;

    explicit ThemeRedmond(std::shared_ptr<FontProvider> f);

    virtual auto init() -> void override{};
    virtual auto draw_widget_background(Bitmap &content, bool has_focus) -> void override;
    virtual auto draw_window_background(Bitmap &content) -> void override;
    virtual auto draw_scrollbar_background(Bitmap &content) -> void override;
    virtual auto draw_button(Bitmap &content, bool has_focus, bool is_default, bool is_enabled,
                             bool has_frame, ButtonStates state, const std::string_view text,
                             const std::shared_ptr<Bitmap> bitmap) -> void override;
    virtual auto draw_checkbox(Bitmap &content, bool has_focus, bool is_enabled, bool is_checked,
                               ButtonStates state, const std::string_view text, CheckboxShape shape,
                               const LayoutParams &padding) -> void override;
    virtual auto draw_input_background(Bitmap &content, const bool has_focus) -> void override;
    virtual auto draw_listview_background(Bitmap &content, const bool has_focus,
                                          bool draw_background) -> void override;
    virtual auto draw_listview_item(Bitmap &content, const std::string_view text,
                                    const ItemStatus status, const bool is_hover) -> void override;

    virtual auto draw_single_tab(Bitmap &content, const int offset, const bool is_active,
                                 const bool is_hover, const LayoutParams &padding,
                                 const std::string_view name) -> int override;

    virtual auto needs_frame_for_focus() const -> bool override { return true; };
    virtual auto scrollbar_size() const -> int override { return 24; };

    virtual auto modify_frame_on_hover() const -> bool override { return false; }
    virtual auto get_padding(PaddingStyle t = PaddingStyle::Label) -> LayoutParams override;
};

struct ThemePlasma : Theme {
    static constexpr int32_t DefaultAccentLight = 0x3daee9;
    static auto get_light_colors(int32_t accent = DefaultAccentLight) -> ColorStyle;
    static auto get_dark_colors(int32_t accent = DefaultAccentLight) -> ColorStyle;

    explicit ThemePlasma(std::shared_ptr<FontProvider> f, int32_t accent = DefaultAccentLight)
        : Theme(f) {
        colors = get_light_colors(accent);
    }

    virtual auto init() -> void override{};
    virtual auto draw_widget_background(Bitmap &content, bool has_focus) -> void override;
    virtual auto draw_window_background(Bitmap &content) -> void override;
    virtual auto draw_scrollbar_background(Bitmap &content) -> void override;
    virtual auto draw_button(Bitmap &content, bool has_focus, bool is_default, bool is_enabled,
                             bool has_frame, ButtonStates state, const std::string_view text,
                             const std::shared_ptr<Bitmap> icon) -> void override;
    virtual auto draw_checkbox(Bitmap &content, bool has_focus, bool is_enabled, bool is_checked,
                               ButtonStates state, const std::string_view text, CheckboxShape shape,
                               const LayoutParams &padding) -> void override;
    virtual auto draw_input_background(Bitmap &content, const bool has_focus) -> void override;
    virtual auto draw_listview_background(Bitmap &content, const bool has_focus,
                                          const bool draw_background) -> void override;
    virtual auto draw_listview_item(Bitmap &content, const std::string_view text,
                                    const ItemStatus status, const bool is_hover) -> void override;
    virtual auto draw_single_tab(Bitmap &content, const int offset, const bool is_active,
                                 const bool is_hover, const LayoutParams &padding,
                                 const std::string_view name) -> int override;

    virtual auto needs_frame_for_focus() const -> bool override { return false; };
    virtual auto scrollbar_size() const -> int override { return 16; };
    virtual auto get_padding(PaddingStyle t = PaddingStyle::Label) -> LayoutParams override;
};
