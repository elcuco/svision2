/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "tabheader.h"
#include "theme.h"
#include <iso646.h>
#include <spdlog/spdlog.h>

TabHeader::TabHeader() : Widget() { this->padding_style = PaddingStyle::TabHeader; }

auto TabHeader::add_tab(const std::string_view name) -> int {
    this->needs_redraw = true;
    this->names.push_back(std::string(name));
    return this->names.size() - 1;
}

auto TabHeader::remove_tab(int index) -> void {
    this->names.erase(this->names.begin() + index);
    this->needs_redraw = true;
}

auto TabHeader::set_active_tab(int index) -> void {
    if (this->active_tab == index) {
        return;
    }
    this->active_tab = index;
    this->invalidate();
    if (this->on_item_selected) {
        on_item_selected(*this, index);
    }
}

auto TabHeader::get_tab_string(int index) const -> std::string_view {
    if (index < 0 || index >= names.size()) {
        return "";
    }
    return this->names[index];
}

auto TabHeader::draw() -> void {
    auto my_theme = get_theme();
    auto hover_tab_index = mouse_over ? hover_tab : -1;
    tab_offset =
        my_theme->draw_tabs(content, has_focus, active_tab, hover_tab_index, get_padding(), names);
}

auto TabHeader::on_mouse_click(const EventMouse &event) -> EventPropagation {
    if (event.pressed) {
        auto index = 0;
        for (const auto &offset : tab_offset) {
            if (event.x > offset.offset && event.x < offset.offset + offset.width) {
                this->needs_redraw = true;
                this->active_tab = index;
                if (this->on_item_selected) {
                    on_item_selected(*this, index);
                }
                return EventPropagation::handled;
            }
            index++;
        }
    }
    return Widget::on_mouse_click(event);
}

auto TabHeader::on_hover(const EventMouse &event) -> void {
    auto my_hover_tab = -1;
    auto index = 0;
    for (const auto &offset : tab_offset) {
        if (event.x > offset.offset && event.x < offset.offset + offset.width) {
            //            this->needs_redraw = true;
            my_hover_tab = index;
        }
        index++;
    }

    if (my_hover_tab >= 0 and my_hover_tab != this->hover_tab) {
        this->hover_tab = my_hover_tab;
        auto t = get_tab_string(my_hover_tab);
        spdlog::info("Mouse under {}", t);
        invalidate();
    }
}

auto TabHeader::size_hint() const -> Size {
    auto padding_y = this->get_padding().get_vertical();
    auto hint = this->get_theme()->font->text_size("X");
    hint.width = 0;
    hint.height += padding_y;
    return hint;
}
