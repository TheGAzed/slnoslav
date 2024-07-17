#ifndef GUI_INTERFACE_BIGGEST_RECTANGLE_H
#define GUI_INTERFACE_BIGGEST_RECTANGLE_H

#include <stdlib.h>

#include <structures/concrete/board.h>

struct nk_rect_array {
    struct nk_rect * elements;
    size_t size;
};

struct nk_rect_array create_max_overlap_rectangles (board_s board);
void                 destroy_max_overlap_rectangles(struct nk_rect_array * array);

#endif //GUI_INTERFACE_BIGGEST_RECTANGLE_H
