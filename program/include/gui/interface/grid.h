#ifndef GUI_INTERFACE_GRID_H
#define GUI_INTERFACE_GRID_H

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_KEYSTATE_BASED_INPUT
#include <nuklear.h>

#include <structures/concrete/board.h>

void grid(struct nk_context * context, board_s board);

#endif //GUI_INTERFACE_GRID_H
