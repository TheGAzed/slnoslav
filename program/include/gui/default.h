#ifndef GUI_DEFAULT_H
#define GUI_DEFAULT_H

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

#define WINDOW_WIDTH  500
#define WINDOW_HEIGHT 560

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

struct nk_solver {
    board_s board;
};

struct nk_super {
    struct nk_context * context;
    struct nk_solver    solver;
};

#endif /* GUI_DEFAULT_H */