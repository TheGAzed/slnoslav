#ifndef STYLE_H
#define STYLE_H

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_KEYSTATE_BASED_INPUT
#include <nuklear.h>

enum theme {
    DARK_MODE, LIGHT_MODE,
};

static inline void set_style(struct nk_context *ctx, enum theme theme) {
    struct nk_color table[NK_COLOR_COUNT];

    table[NK_COLOR_TEXT] = nk_rgba(0x1B, 0x1B, 0x1B, 0xFF);
    table[NK_COLOR_WINDOW] = nk_rgba(0xD9, 0xD9, 0xD9, 0xFF);
    table[NK_COLOR_HEADER] = nk_rgba(175, 175, 175, 255);
    table[NK_COLOR_BORDER] = nk_rgba(0xD9, 0xD9, 0xD9, 255);
    table[NK_COLOR_BUTTON] = nk_rgba(0xD9, 0xD9, 0xD9, 255);
    table[NK_COLOR_BUTTON_HOVER] = nk_rgba(0xD9, 0xD9, 0xD9, 255);
    table[NK_COLOR_BUTTON_ACTIVE] = nk_rgba(0xD9, 0xD9, 0xD9, 255);
    table[NK_COLOR_SLIDER] = nk_rgba(0x82, 0x82, 0x82, 255);
    table[NK_COLOR_SLIDER_CURSOR] = nk_rgba(0x1B, 0x1B, 0x1B, 255);
    table[NK_COLOR_SLIDER_CURSOR_HOVER] = nk_rgba(0x1B, 0x1B, 0x1B, 255);
    table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = nk_rgba(0x1B, 0x1B, 0x1B, 255);
    table[NK_COLOR_PROPERTY] = nk_rgba(175, 175, 175, 255);
    table[NK_COLOR_EDIT] = nk_rgba(150, 150, 150, 255);
    table[NK_COLOR_EDIT_CURSOR] = nk_rgba(0, 0, 0, 255);
    table[NK_COLOR_COMBO] = nk_rgba(175, 175, 175, 255);
    table[NK_COLOR_CHART] = nk_rgba(160, 160, 160, 255);
    table[NK_COLOR_CHART_COLOR] = nk_rgba(45, 45, 45, 255);
    table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = nk_rgba( 255, 0, 0, 255);
    table[NK_COLOR_TAB_HEADER] = nk_rgba(180, 180, 180, 255);

    nk_style_from_table(ctx, table);
}

#endif