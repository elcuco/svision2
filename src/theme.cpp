/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "theme.h"
#include <string>

auto Theme::draw_frame(Bitmap &content, Position position, Size size, FrameStyles style,
                       FrameSize frame_size) -> void {

    switch (style) {
    case FrameStyles::Normal:
        switch (frame_size) {
        case FrameSize::TrippleFrame:
            content.draw_rectangle(2, 2, size.width - 4, size.height - 4,
                                   colors.frame_normal_color4, colors.frame_normal_color4);
            [[fallthrough]];
        case FrameSize::DoubleFrame:
            content.draw_rectangle(1, 1, size.width - 2, size.height - 2,
                                   colors.frame_normal_color3, colors.frame_normal_color4);
            [[fallthrough]];
        case FrameSize::SingleFrame:
            content.draw_rectangle(position.x, position.y, size.width, size.height,
                                   colors.frame_normal_color1, colors.frame_normal_color2);
            [[fallthrough]];
        case FrameSize::NoFrame:
            break;
        }
        break;
    case FrameStyles::Reversed:
        switch (frame_size) {
        case FrameSize::TrippleFrame:
            // Not really supported
            [[fallthrough]];
        case FrameSize::DoubleFrame:
            content.draw_rectangle(1, 1, size.width - 2, size.height - 2,
                                   colors.frame_normal_color1, colors.frame_normal_color2);
            [[fallthrough]];
        case FrameSize::SingleFrame:
            content.draw_rectangle(position.x, position.y, size.width, size.height,
                                   colors.frame_normal_color2, colors.frame_normal_color1);
            [[fallthrough]];
        case FrameSize::NoFrame:
            break;
        }
        break;
    case FrameStyles::Disabled:
        switch (frame_size) {
        case FrameSize::TrippleFrame:
            // Not really supported
            [[fallthrough]];
        case FrameSize::DoubleFrame:
            content.draw_rectangle(position.x + 1, position.y + 1, size.width - 2, size.height - 2,
                                   colors.frame_disabled_color3, colors.frame_disabled_color4);
            [[fallthrough]];
        case FrameSize::SingleFrame:
            content.draw_rectangle(position.x, position.y, size.width, size.height,
                                   colors.frame_disabled_color1, colors.frame_disabled_color2);
            [[fallthrough]];
        case FrameSize::NoFrame:
            break;
        }
        break;
    case FrameStyles::Hover:
        switch (frame_size) {
        case FrameSize::TrippleFrame:
            // Not really supported
            [[fallthrough]];
        case FrameSize::DoubleFrame:
            content.draw_rectangle(position.x + 1, position.y + 1, size.width - 2, size.height - 2,
                                   colors.frame_hover_color3, colors.frame_hover_color4);
            [[fallthrough]];
        case FrameSize::SingleFrame:
            content.draw_rectangle(position.x, position.y, size.width, size.height,
                                   colors.frame_hover_color1, colors.frame_hover_color2);
            [[fallthrough]];
        case FrameSize::NoFrame:
            break;
        }

        break;
    case FrameStyles::NoFrame:
        break;
    }
}

auto Theme::draw_tabs(Bitmap &content, bool has_focus, int selected_index, int hover_index,
                      const LayoutParams &padding, const std::vector<std::string> &names)
    -> std::vector<TabHeaderOffsets> {
    auto tab_offset = std::vector<TabHeaderOffsets>();

    auto offset = 0;
    auto bottom_frame_color1 = colors.frame_normal_color3;
    auto bottom_frame_color2 = colors.window_background;

    draw_widget_background(content, has_focus);
    content.fill_rect(0, content.size.height - 2, content.size.width, 2, bottom_frame_color2);
    content.fill_rect(0, content.size.height - 3, content.size.width, 1, bottom_frame_color1);
    tab_offset.clear();
    tab_offset.resize(names.size());
    auto i = 0;
    for (auto &tab_name : names) {
        auto is_active_tab = i == selected_index;
        auto is_hover = i == hover_index;
        auto size = draw_single_tab(content, offset, is_active_tab, is_hover, padding, tab_name);
        tab_offset[i] = {offset, size};
        offset += size;
        i++;
    }

    return tab_offset;
}

auto ThemeRedmond::get_light_colors() -> ColorStyle {
    auto colors = ColorStyle();
    auto constexpr white = 0xFFFFFF;
    auto constexpr black = 0x000000;
    auto constexpr blue = 0x303080;

    colors.window_background = 0xc0c0c0;

    colors.frame_normal_color1 = white;
    colors.frame_normal_color2 = black;
    colors.frame_normal_color3 = white;
    colors.frame_normal_color4 = 0x808080;

    colors.frame_hover_color1 = colors.frame_normal_color1;
    colors.frame_hover_color2 = colors.frame_normal_color2;
    colors.frame_hover_color3 = colors.frame_normal_color3;
    colors.frame_hover_color4 = colors.frame_normal_color4;

    colors.frame_selected_color1 = colors.frame_normal_color1;
    colors.frame_selected_color2 = colors.frame_normal_color2;
    colors.frame_selected_color3 = colors.frame_normal_color3;
    colors.frame_selected_color4 = colors.frame_normal_color4;

    colors.frame_disabled_color1 = 0xd1d2d3;
    colors.frame_disabled_color2 = 0xd1d2d3;
    colors.frame_disabled_color3 = 0xd1d2d3;
    colors.frame_disabled_color4 = 0xd1d2d3;

    colors.input_background_normal = white;
    colors.input_background_selected = white;
    colors.input_background_hover = colors.input_background_normal;
    colors.input_background_disabled = colors.window_background;

    colors.button_background_1 = colors.window_background;
    colors.button_background_2 = colors.window_background;
    colors.button_selected_background = 0xc8c8c8;
    colors.button_selected_text = white;

    colors.text_color = black;
    colors.text_color_disabled = 0x606060;

    colors.text_selection_color = white;
    colors.text_selection_background = blue;
    colors.text_selection_background_hover = white;
    return colors;
}

auto ThemeRedmond::get_dark_colors() -> ColorStyle {
    ColorStyle colors;
    return colors;
}

ThemeRedmond::ThemeRedmond(std::shared_ptr<FontProvider> f) : Theme(f) {
    colors = get_light_colors();
    defaultPadding.set_vertical(2);
    defaultPadding.set_horizontal(5);
}

auto ThemeRedmond::draw_widget_background(Bitmap &content, bool has_focus) -> void {
    (void)(has_focus);
    content.fill(colors.window_background);
}

auto ThemeRedmond::draw_window_background(Bitmap &content) -> void {
    content.fill(colors.window_background);
}

auto ThemeRedmond::draw_scrollbar_background(Bitmap &content) -> void {
    auto on_color = colors.frame_normal_color1;
    auto off_color = colors.frame_normal_color2;
    for (auto y = 1; y < content.size.height - 1; y++) {
        for (auto x = 1; x < content.size.width - 1; x++) {
            auto on = (x + y) % 2 == 0;
            content.put_pixel(x, y, on ? on_color : off_color);
        }
    }
    auto border_color = colors.frame_normal_color4;
    content.draw_rectangle(0, 0, content.size.width, content.size.height, border_color,
                           border_color);
}

auto ThemeRedmond::draw_button(Bitmap &content, bool has_focus, bool is_default, bool is_enabled,
                               bool has_frame, ButtonStates state, const std::string_view text,
                               const std::shared_ptr<Bitmap> bitmap) -> void {
    (void)(has_focus);
    auto text_padding = 5;
    auto background_color = 0;
    auto shadow_offset = -1;
    auto topleft = Position{0, 0};
    auto frame_size = is_default ? FrameSize::TrippleFrame : FrameSize::DoubleFrame;
    auto frame_style = FrameStyles::NoFrame;

    switch (state) {
    case ButtonStates::Normal:
        if (is_enabled) {
            frame_style = FrameStyles::Normal;
        } else {
            frame_style = FrameStyles::Disabled;
        }

        if (!has_frame) {
            frame_style = FrameStyles::NoFrame;
        }
        background_color = colors.window_background;
        break;
    case ButtonStates::Hovered:
        if (is_enabled) {
            frame_style = FrameStyles::Hover;
        } else {
            frame_style = FrameStyles::Disabled;
        }
        // TODO - are we missing a color in the theme?
        background_color = colors.button_selected_background;
        break;
    case ButtonStates::ClickedInside:
        if (is_enabled) {
            frame_style = FrameStyles::Reversed;
        } else {
            frame_style = FrameStyles::Disabled;
        }
        background_color = colors.button_background_1;
        shadow_offset = is_default ? -3 : -2;
        break;
    case ButtonStates::ClickedOutside:
        background_color = colors.window_background;
        break;
    default:
        break;
    }

    if (!is_default) {
        content.fill(background_color);
    } else {
        content.fill(background_color);
    }
    draw_frame(content, topleft, content.size, frame_style, frame_size);

    auto text_size = font->text_size(text);
    auto content_rect = content.size - (text_padding);
    auto centered = content_rect.centered(text_size);

    font->write(content, centered - shadow_offset, text, 0x00);
    font->write(content, centered, text, 0xffffff);
}

auto ThemeRedmond::draw_checkbox(Bitmap &content, bool has_focus, bool is_enabled, bool is_checked,
                                 ButtonStates state, const std::string_view text,
                                 CheckboxShape shape, const LayoutParams &padding) -> void {
    auto background_color = colors.input_background_normal;
    auto foreground_color = colors.text_color;
    auto checkbox_size = content.size.height;
    (void)(has_focus);

    switch (state) {
    case ButtonStates::ClickedInside:
        // TODO - missing a theme color
        foreground_color = colors.text_color;
        break;
    case ButtonStates::ClickedOutside:
        foreground_color = colors.input_background_hover;
        break;
    case ButtonStates::Hovered:
        if (!is_enabled) {
            foreground_color = colors.text_color_disabled;
        } else {
            foreground_color = colors.text_color;
        }
        break;
    case ButtonStates::Normal:
        if (!is_enabled) {
            foreground_color = colors.text_color_disabled;
        }
        break;
    }

    auto m = checkbox_size / 2;
    auto icon_padding = 1;

    content.fill(colors.window_background);

    switch (shape) {
    case CheckboxShape::Checkbox:
        content.fill_rect(icon_padding, icon_padding, checkbox_size - icon_padding * 2,
                          checkbox_size - icon_padding * 2, background_color);
        draw_frame(content, {0, 0}, {checkbox_size, checkbox_size}, FrameStyles::Normal,
                   FrameSize::SingleFrame);
        break;
    case CheckboxShape::RadioButton:
        content.fill_circle(m, m, checkbox_size / 2 - icon_padding, background_color);

        if (is_enabled) {
            content.draw_circle(m, m, checkbox_size / 2 - icon_padding, colors.frame_normal_color1);
            content.draw_circle(m, m, checkbox_size / 2 - icon_padding - 1,
                                colors.frame_normal_color2);
        } else {
            content.draw_circle(m, m, checkbox_size / 2 - icon_padding,
                                colors.frame_disabled_color1);
        }
        break;
    }

    auto text_size = font->text_size(text);
    auto content_rect = content.size;
    auto centered = content_rect.centeredY(text_size);
    content_rect.width -= padding.get_horizontal();
    content_rect.height -= padding.get_vertical();
    centered.x += checkbox_size + padding.start;
    font->write(content, centered, text,
                is_enabled ? foreground_color : colors.text_color_disabled);

    if (is_checked) {
        auto padding = 4;

        switch (shape) {
        case CheckboxShape::Checkbox:
            content.line_thikness(padding, padding, checkbox_size - padding,
                                  checkbox_size - padding, 2, foreground_color);
            content.line_thikness(checkbox_size - padding, padding, padding,
                                  checkbox_size - padding, 2, foreground_color);
            break;
        case CheckboxShape::RadioButton:
            content.fill_circle(m + 1, m + 1, checkbox_size / 2 - padding - 3,
                                colors.frame_normal_color2);
            //            content.fill_circle(m+2, m+2, checkbox_size/2 - padding-4,
            //            ThemeRedmond::line_color2);
            break;
        }
    }
}

auto ThemeRedmond::draw_input_background(Bitmap &content, const bool has_focus) -> void {
    auto background = has_focus ? colors.input_background_selected : colors.input_background_normal;
    content.fill(background);
}

auto ThemeRedmond::draw_listview_background(Bitmap &content, const bool has_focus,
                                            bool draw_background) -> void {
    draw_frame(content, {0, 0}, content.size, has_focus ? FrameStyles::Hover : FrameStyles::Normal,
               FrameSize::SingleFrame);
    if (draw_background) {
        auto background =
            has_focus ? colors.input_background_selected : colors.input_background_normal;
        content.fill_rect(1, 1, content.size.width - 2, content.size.height - 2, background);
    }
}

void ThemeRedmond::draw_listview_item(Bitmap &content, const std::string_view text,
                                      const ItemStatus status, const bool is_hover) {
    auto padding = Position{defaultPadding.start, defaultPadding.top};
    auto text_color = status.is_active ? colors.text_selection_color : colors.text_color;
    auto background_color =
        status.is_active ? colors.text_selection_background : colors.input_background_normal;
    if (is_hover && !status.is_active) {
        background_color = colors.input_background_hover;
    }
    content.fill(background_color);
    font->write(content, padding, text, text_color);
}

auto ThemeRedmond::draw_single_tab(Bitmap &content, const int offset, const bool is_active,
                                   const bool is_hover, const LayoutParams &padding,
                                   const std::string_view name) -> int {
    auto active_bg = (colors.window_background);
    auto tab_size = font->text_size(name);

    tab_size.width += padding.get_horizontal();
    tab_size.height += padding.get_vertical();

    if (is_active) {
        content.fill_rect(offset, 0, tab_size.width, tab_size.height, active_bg);
        draw_frame(content, {offset, 0}, {tab_size.width, tab_size.height}, FrameStyles::Normal,
                   FrameSize::SingleFrame);
        content.fill_rect(offset, tab_size.height - 1, tab_size.width, 2, active_bg);

    } else {
        if (is_hover) {
            content.fill_rect(offset, 0, tab_size.width, tab_size.height - 3,
                              Lighter(active_bg, 0.05));
        }
    }
    font->write(content, {offset + padding.start, padding.top}, name, colors.text_color);
    return tab_size.width;
}

LayoutParams ThemeRedmond::get_padding(PaddingStyle t) {
    switch (t) {
    case PaddingStyle::Button:
        return {8, 8, 8, 8};
    case PaddingStyle::ScrollBar:
        return {10, 10, 10, 10};
    case PaddingStyle::TabHeader:
        return {5, 5, 10, 10};
    case PaddingStyle::Checkbox:
        break;
    case PaddingStyle::Label:
        break;
    }
    return defaultPadding;
}

// static constexpr int32_t DefaultAccentLight = 0x3daee9;

auto ThemePlasma::get_light_colors(int32_t accent) -> ColorStyle {
    auto background = 0xeff0f1;
    auto disabled = 0x737577;

    auto colors = ColorStyle();
    colors.window_background = background;

    colors.frame_normal_color1 = 0xbbbcbd;
    colors.frame_normal_color2 = colors.frame_normal_color1;
    colors.frame_normal_color3 = colors.frame_normal_color1;
    colors.frame_normal_color4 = colors.frame_normal_color1;

    colors.frame_hover_color1 = accent;
    colors.frame_hover_color2 = colors.frame_hover_color1;
    colors.frame_hover_color3 = colors.frame_hover_color1;
    colors.frame_hover_color4 = colors.frame_hover_color1;

    colors.frame_selected_color1 = accent;
    colors.frame_selected_color2 = colors.frame_selected_color1;
    colors.frame_selected_color3 = colors.frame_selected_color1;
    colors.frame_selected_color4 = colors.frame_selected_color1;

    colors.frame_disabled_color1 = disabled;
    colors.frame_disabled_color2 = colors.frame_disabled_color1;
    colors.frame_disabled_color3 = colors.frame_disabled_color1;
    colors.frame_disabled_color4 = colors.frame_disabled_color1;

    colors.input_background_normal = Darker(0xffffff, 0.002);
    colors.input_background_hover = 0xffffff;
    colors.input_background_disabled = disabled;
    colors.input_background_selected = background;

    colors.button_background_1 = Lighter(background);
    colors.button_background_2 = Lighter(background, 0.05);
    colors.button_selected_background = Darker(colors.button_background_1);
    colors.button_selected_text = 0x31373b;

    colors.text_color = 0x2b2e31;
    colors.text_color_disabled = disabled;

    colors.text_selection_color = 0xffffff;
    colors.text_selection_background = accent;
    colors.text_selection_background_hover = Lighter(accent);
    return colors;
}

auto ThemePlasma::get_dark_colors(int32_t accent) -> ColorStyle {
    auto colors = ColorStyle();
    colors.window_background = 0xeff0f1;

    colors.frame_normal_color1 = 0xbbbcbd;
    colors.frame_normal_color2 = 0xbbbcbd;
    colors.frame_normal_color3 = 0xbbbcbd;
    colors.frame_normal_color4 = 0xbbbcbd;

    colors.frame_hover_color1 = 0x3daee9;
    colors.frame_hover_color2 = 0x3daee9;
    colors.frame_hover_color3 = 0x3daee9;
    colors.frame_hover_color4 = 0x3daee9;

    colors.frame_selected_color1 = 0x3daee9;
    colors.frame_selected_color2 = 0x3daee9;
    colors.frame_selected_color3 = 0x3daee9;
    colors.frame_selected_color4 = 0x3daee9;

    colors.frame_disabled_color1 = 0xd1d2d3;
    colors.frame_disabled_color2 = 0xd1d2d3;
    colors.frame_disabled_color3 = 0xd1d2d3;
    colors.frame_disabled_color4 = 0xd1d2d3;

    colors.input_background_normal = 0xffffff;
    colors.input_background_hover = 0x0;
    colors.input_background_disabled = 0x0;
    colors.input_background_selected = 0x0;

    colors.button_background_1 = 0xfcfcfc;
    colors.button_background_2 = 0xf5f5f5;
    colors.button_selected_background = 0xd6ecf8;
    colors.button_selected_text = 0x31373b;
    //    colors.button_selected_border = 0x3daee9; // todo - needed?

    colors.text_color = 0x2b2e31;
    colors.text_color_disabled = 0x737577;

    colors.text_selection_color = 0xffffff;
    colors.text_selection_background = 0x3daee9;
    return colors;
}

auto ThemePlasma::draw_widget_background(Bitmap &content, bool has_focus) -> void {
    (void)(has_focus);
    content.fill(colors.window_background);
}

auto ThemePlasma::draw_window_background(Bitmap &content) -> void {
    content.fill(colors.window_background);
    content.line(0, 0, content.size.width - 1, 0, colors.frame_disabled_color1);
}

auto ThemePlasma::draw_scrollbar_background(Bitmap &content) -> void {}

auto ThemePlasma::draw_button(Bitmap &content, bool has_focus, bool is_default, bool is_enabled,
                              bool has_frame, ButtonStates state, const std::string_view text,
                              const std::shared_ptr<Bitmap> icon) -> void {
    auto background1 = colors.button_background_1;
    auto background2 = colors.button_background_2;
    auto border = colors.frame_normal_color1;
    auto color = colors.text_color;

    switch (state) {
    case ButtonStates::Normal:
        background1 = has_frame ? colors.button_background_1 : colors.window_background;
        break;
    case ButtonStates::Hovered:
        if (is_enabled) {
            background1 = colors.button_background_1;
            background2 = colors.button_background_2;
            border = colors.frame_hover_color1;
        } else {
            border = colors.frame_disabled_color1;
        }
        break;
    case ButtonStates::ClickedInside:
        background1 = colors.button_selected_background;
        background2 = colors.button_selected_background;
        border = colors.frame_hover_color1;
        color = colors.button_selected_text;
        break;
    case ButtonStates::ClickedOutside:
        background1 = colors.button_background_1;
        background2 = colors.button_background_2;
        border = colors.frame_hover_color1;
        color = colors.button_selected_text;
        break;
    default:
        break;
    }
    if (!is_enabled) {
        border = colors.frame_disabled_color1;
    } else {
        if (has_focus) {
            border = colors.frame_hover_color1;
        }
    }

    if (background1 == background2) {
        content.fill(background1);
    } else {
        content.fill_rect_gradient(0, 0, content.size.width, content.size.height, background1,
                                   background2);
    }

    if (has_frame || state != ButtonStates::Normal) {
        content.draw_rounded_rectangle(0, 0, content.size.width, content.size.height - 1, 5, border,
                                       border);
        content.line(2, content.size.height - 1, content.size.width - 2, content.size.height - 1,
                     colors.frame_disabled_color1);
    }

    // TODO - widget should be filled with real content from parent
    content.put_pixel(0, content.size.height - 1, colors.window_background);
    content.put_pixel(1, content.size.height - 1, colors.window_background);
    content.put_pixel(content.size.width - 1, content.size.height - 1, colors.window_background);

    if (is_enabled) {
        // TODO properly center
        auto text_padding = 5;

        if (icon && icon->size.height != 0 && icon->size.width != 0) {
            auto centered = content.size.centered(icon->size, text_padding);
            content.draw(centered, *icon.get(), true);
        } else {
            auto text_size = font->text_size(text);
            auto centered = content.size.centered(text_size, text_padding);
            font->write(content, centered, text, color);
        }
    }
}

auto ThemePlasma::draw_checkbox(Bitmap &content, bool has_focus, bool is_enabled, bool is_checked,
                                ButtonStates state, const std::string_view text,
                                CheckboxShape shape, const LayoutParams &padding) -> void {
    auto background_color = colors.window_background;
    auto foreground_color = colors.text_color;
    auto checkbox_size = content.size.height;
    auto checkbox_border = colors.frame_disabled_color1;

    switch (state) {
    case ButtonStates::ClickedInside:
        checkbox_border = colors.frame_normal_color1;
        is_checked = true;
        break;
    case ButtonStates::ClickedOutside:
        checkbox_border = colors.frame_hover_color1;
        break;
    case ButtonStates::Hovered:
        if (is_enabled) {
            checkbox_border = colors.frame_hover_color1;
        }
        break;
    case ButtonStates::Normal:
        checkbox_border = has_focus ? colors.frame_hover_color1 : colors.frame_normal_color1;
        break;
    }

    content.fill(background_color);
    {
        auto icon_padding = 3;
        auto p = Position{icon_padding, icon_padding};
        auto w = Size{checkbox_size - icon_padding * 2, checkbox_size - icon_padding * 2};
        auto m = checkbox_size / 2;

        switch (shape) {
        case CheckboxShape::Checkbox:
            content.draw_rounded_rectangle(p.x, p.y, w.width, w.height, 1, checkbox_border,
                                           checkbox_border);
            break;
        case CheckboxShape::RadioButton:
            if (is_checked) {
                if (is_enabled) {
                    content.fill_circle(m, m, checkbox_size / 2 - icon_padding,
                                        colors.button_selected_background);
                    content.draw_circle(m, m, checkbox_size / 2 - icon_padding,
                                        colors.frame_hover_color1);
                } else {
                    content.fill_circle(m, m, checkbox_size / 2 - icon_padding, checkbox_border);
                    content.draw_circle(m, m, checkbox_size / 2 - icon_padding,
                                        colors.text_color_disabled);
                }
            } else {
                content.draw_circle(m, m, checkbox_size / 2 - icon_padding, checkbox_border);
            }
            break;
        }
    }

    {
        auto icon_padding = 5;
        auto p = Position{icon_padding, icon_padding};
        auto w = Size{checkbox_size - icon_padding * 2, checkbox_size - icon_padding * 2};
        auto m = checkbox_size / 2;

        switch (shape) {
        case CheckboxShape::Checkbox:
            content.fill_rect(p.x, p.y, w.width, w.height, checkbox_border);
            break;
        case CheckboxShape::RadioButton:
            if (is_checked) {
                content.fill_circle(m, m, 4, checkbox_border);
            }
            break;
        }
    }

    {
        auto text_size = font->text_size(text);
        auto text_block_size = Size{content.size.width - checkbox_size, checkbox_size};
        auto centered = text_block_size.centeredY(text_size);
        centered.x += checkbox_size;
        font->write(content, centered, text,
                    is_enabled ? foreground_color : colors.text_color_disabled);
    }
}

auto ThemePlasma::draw_input_background(Bitmap &content, const bool has_focus) -> void {
    auto background = has_focus ? colors.input_background_hover : colors.input_background_normal;
    content.fill(background);
}

auto ThemePlasma::draw_listview_background(Bitmap &content, const bool has_focus,
                                           const bool draw_background) -> void {
    draw_frame(content, {0, 0}, content.size, has_focus ? FrameStyles::Hover : FrameStyles::Normal,
               FrameSize::SingleFrame);
    if (draw_background) {
        auto background =
            has_focus ? colors.input_background_selected : colors.input_background_normal;
        auto padding = 1;
        content.fill_rect(padding, padding, content.size.width - padding * 2,
                          content.size.height - padding * 2, background);
    }
}

void ThemePlasma::draw_listview_item(Bitmap &content, const std::string_view text,
                                     const ItemStatus status, const bool is_hover) {

    auto text_color = status.is_active ? colors.text_selection_color : colors.text_color;
    auto background_color =
        status.is_active ? colors.text_selection_background : colors.input_background_normal;
    if (is_hover && !status.is_active) {
        background_color = colors.text_selection_background_hover;
    }

    content.fill(background_color);
    auto text_padding = 5;
    auto text_size = font->text_size(text);
    auto centered = content.size.centeredY(text_size, text_padding);
    font->write(content, centered, text, text_color);
}

auto ThemePlasma::draw_single_tab(Bitmap &content, const int offset, const bool is_active,
                                  const bool is_hover, const LayoutParams &padding,
                                  const std::string_view name) -> int {
    auto is_tab_hover = is_hover;
    auto active_bg = colors.window_background;
    auto tab_size = font->text_size(name);
    tab_size.width += padding.get_horizontal();
    tab_size.height += padding.get_vertical();

    if (is_active) {
        content.fill_rect(offset, 0, tab_size.width, tab_size.height, active_bg);
        //        draw_frame(content, {offset, 0}, {tab_size.width, tab_size.height},
        //        FrameStyles::Hover,
        //                   FrameSize::SingleFrame);
        content.fill_rect(offset, 0, tab_size.width, 3, colors.frame_hover_color3);
        content.fill_rect(offset, tab_size.height - 2, tab_size.width, 2, active_bg);

        content.line(offset, 0, offset, tab_size.height - 3, colors.frame_normal_color3);
        content.line(offset + tab_size.width - 1, 0, offset + tab_size.width - 1,
                     tab_size.height - 3, colors.frame_normal_color3);

        content.line(0, 0, 0, tab_size.height, colors.frame_normal_color3);
    } else {
        content.fill_rect(offset, 0, tab_size.width, tab_size.height, Darker(active_bg));
        auto bottom_frame_color1 = colors.frame_normal_color3;
        auto bottom_frame_color2 = colors.window_background;

        content.fill_rect(offset, tab_size.height - 2, tab_size.width, 2, bottom_frame_color2);
        content.fill_rect(offset, tab_size.height - 3, tab_size.width, 1, bottom_frame_color1);
    }
    if (is_active) {
        is_tab_hover = false;
    }
    font->write(content, {offset + padding.start, padding.top}, name,
                is_tab_hover ? Lighter(colors.button_selected_text, 0.3)
                             : colors.button_selected_text);
    return tab_size.width;
}

LayoutParams ThemePlasma::get_padding(PaddingStyle t) {
    switch (t) {
    case PaddingStyle::Button:
        return {10, 10, 10, 10};
    case PaddingStyle::Checkbox:
        return {5, 5, 5, 5};
    case PaddingStyle::Label:
        return {5, 5, 5, 5};
    case PaddingStyle::ScrollBar:
        return {12, 12, 12, 12};
    case PaddingStyle::TabHeader:
        return {10, 10, 10, 10};
    }
    return defaultPadding;
}
