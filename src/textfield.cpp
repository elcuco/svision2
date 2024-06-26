/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "textfield.h"
#include <spdlog/spdlog.h>
#include <theme.h>

#include <spdlog/spdlog.h>

TextField::TextField() : TextField({}, {}) {}

TextField::TextField(Position position, Size size) : Widget(position, size, 0) {
    timer.millies = 750;
    timer.repeating = true;
    timer.callback = [this]() {
        this->cursor_on = !this->cursor_on;
        invalidate();
    };
    timer.initialize();
    this->can_focus = true;
    this->draw_background = false;
    this->frame = {FrameStyles::Reversed, FrameSize::SingleFrame};
    this->mouse_cursor = MouseCursor::Edit;
}

TextField::~TextField() { timer.stop(); }

auto TextField::draw() -> void {
    theme->draw_input_background(content, has_focus);
    Widget::draw();

    auto text_size = theme->font->text_size(text);
    auto p = get_padding();

    // TODO - we need to find where the text clips - properly
    auto display_text_logical = (content.size.width - p.get_horizontal()) / 8;
    auto display_text = this->text.substr(display_from, display_from + display_text_logical);
    auto center_y = (content.size.height - text_size.height) / 2;
    auto selection_width = (selection.end - selection.start) - display_from;
    selection_width *= 8;

    // TODO - unmanaged color writing in a widget
    // TODO handle partial selection
    if (selection_width != 0) {
        content.fill_rect(p.start, p.top, selection_width, content.size.height - p.get_vertical(),
                          theme->colors.text_selection_background);
    }
    theme->font->write(content, Position{p.start, center_y}, display_text,
                       theme->colors.text_color);

    if (this->cursor_on && this->has_focus) {
        auto position_x = p.start + (cursor_position - display_from) * 8;
        content.draw_rectangle(position_x, p.top, 1, content.size.height - p.get_vertical(), 0, 0);
    }
}

auto TextField::on_keyboard(const EventKeyboard &event) -> EventPropagation {
    auto result = EventPropagation::propagate;
    switch (event.key) {
    case KeyCodes::Enter:
        spdlog::info("Pressed enter");
        break;
    case KeyCodes::ArrowLeft:
        if (cursor_position > 0) {
            // TODO - move to next word
            // TODO - change selection
            cursor_position--;
            cursor_on = true;
            select_none();
            ensure_cursor_visible();
            invalidate();
        }
        result = EventPropagation::handled;
        break;
    case KeyCodes::ArrowRight:
        if (cursor_position < text.length()) {
            // TODO - move to next word
            // TODO - change selection
            cursor_position++;
            cursor_on = true;
            select_none();
            ensure_cursor_visible();
            invalidate();
        }
        result = EventPropagation::handled;
        break;
    case KeyCodes::Home:
        // TODO - change selection
        if (cursor_position != 0) {
            display_from = 0;
            cursor_position = 0;
            cursor_on = true;
            select_none();
            invalidate();
        }
        result = EventPropagation::handled;
        break;
    case KeyCodes::End:
        // TODO - change selection
        cursor_position = text.length();
        cursor_on = true;
        ensure_cursor_visible();
        select_none();
        invalidate();
        return EventPropagation::handled;
    case KeyCodes::Delete:
        if (has_selection()) {
            text.erase(selection.start, selection.end);
        } else {
            text.erase(cursor_position, 1);
        }
        cursor_on = true;
        ensure_cursor_visible();
        select_none();
        invalidate();
        result = EventPropagation::handled;
        break;
    case KeyCodes::Backspace:
        if (has_selection()) {
            text.erase(selection.start, selection.end);
            cursor_on = true;
            ensure_cursor_visible();
            select_none();
            invalidate();
        } else {
            if (cursor_position > 0) {
                text.erase(cursor_position - 1, 1);
                cursor_position--;
                cursor_on = true;
                ensure_cursor_visible();
                select_none();
                invalidate();
            }
        }
        result = EventPropagation::handled;
        break;
    case KeyCodes::Escape:
        select_none();
        invalidate();
        break;
    default:
        // TODO handle non ascii input
        auto is_control_pressed = event.is_control_pressed();
        auto is_a_pressed = event.key == (KeyCodes)'A' || event.key == (KeyCodes)'a';
        if (is_a_pressed && is_control_pressed) {
            spdlog::info("Selecting all");
            select_all();
            result = EventPropagation::handled;
            break;
        }
        if ((int)event.key >= ' ' && (int)event.key <= 128) {
            if (validator) {
                if (!validator->is_keyboard_input_valid(event.key, cursor_position)) {
                    break;
                }
            }
            char ascii = (char)event.key;
            spdlog::info("new ascii char: {}", ascii);

            // TODO replace selection
            if (cursor_position > text.length()) {
                text += (char)event.key;
            }
            { text.insert(cursor_position, 1, ascii); }
            cursor_position += 1;
            cursor_on = true;
            select_none();
            ensure_cursor_visible();
            invalidate();
            result = EventPropagation::handled;
        } else {
            spdlog::info("KeyCode = {:x}", (int)event.key);
        }
        break;
    }
    return result;
}

auto TextField::on_mouse_click(const EventMouse &event) -> EventPropagation {
    if (!event.pressed || event.button != 1 || !event.is_local) {
        return EventPropagation::propagate;
    }

    auto pos = (event.x - padding.start) / 8;
    cursor_position = display_from + pos;
    cursor_position = std::min((size_t)cursor_position, text.length());
    cursor_on = true;
    invalidate();
    return EventPropagation::handled;
}

auto TextField::on_focus_change(bool new_state) -> void {
    if (new_state) {
        timer.start();
        this->cursor_on = true;
    } else {
        timer.stop();
    }

    invalidate();
}

auto TextField::on_remove() -> void { timer.stop(); }

auto TextField::size_hint() const -> Size {
    // TODO: Size of text is not correct. We also need to calculate the yMin and yMax for example
    auto t = get_text().size() != 0 ? get_text() : "X";
    auto s = get_theme()->font->text_size(t);
    return {0, s.height + this->get_padding().get_vertical()};
}

auto TextField::select_all() -> void {
    selection.start = 0;
    selection.end = text.length();
    if (selection.end < selection.start) {
        selection.end = selection.start;
    }
    cursor_on = true;
    invalidate();
}

auto TextField::select_none() -> void {
    selection.start = 0;
    selection.end = 0;
}

auto TextField::get_selected_text() -> const std::string {
    return this->text.substr(selection.start, selection.end);
}

auto TextField::ensure_cursor_visible() -> void {
    auto padding_y = this->padding.get_horizontal();
    auto max_x_position = content.size.width - padding_y;
    auto cursor_visual_position = (cursor_position - display_from) * 8 + padding_y;

    while (cursor_visual_position > max_x_position) {
        display_from++;
        cursor_visual_position = (cursor_position - display_from) * 8 + padding_y;
    }
    while (display_from > cursor_position) {
        display_from = cursor_position - 1;
    }
    if (cursor_position > text.length()) {
        cursor_position = text.length();
    }
    if (display_from < 0) {
        display_from = 0;
    }
    if (selection.end < selection.start) {
        selection.end = selection.start;
    }
}

auto TextField::set_text(const std::string_view new_text) -> void {
    if (validator) {
        if (!validator->is_string_valid(new_text)) {
            return;
        }
    }

    this->text = new_text;
    this->cursor_position = 0;
    this->selection.start = 0;
    this->selection.end = 0;
    invalidate();
}
