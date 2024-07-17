#include <gui/interface/interface.h>
#include <gui/interface/solver.h>
#include <gui/interface/grid.h>

void _player(struct nk_context * context);
void _to_start(struct nk_context * context);
void _previus(struct nk_context * context);
void _playback(struct nk_context * context);
void _play(struct nk_context * context);
void _next(struct nk_context * context);
void _to_end(struct nk_context * context);

void interface(struct nk_super * super) {
    struct nk_context * context = super->context;
    board_s board = super->solver.board;

    if (nk_begin(super->context, "KAKURO BOARD", nk_rect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT), NK_WINDOW_BORDER)) {
        grid(context, board);
        _player(context);
    }
    nk_end(super->context);
}

void _player(struct nk_context * context) {
    nk_layout_row_dynamic(context, 50, 6);
    _to_start(context);
    _previus(context);
    _playback(context);
    _play(context);
    _next(context);
    _to_end(context);
}

void _to_start(struct nk_context * context) {
    if (nk_button_label(context, "START")) {
        state_provider(START_VALUE_DS);
    }
}

void _previus(struct nk_context * context) {
    if (nk_button_label(context, "PREV")) {
        get_player_singleton()->play_state = STOP_MIDDLE_E;
        state_provider(PREV_VALUE_DS);
    }
}

void _playback(struct nk_context * context) {
    if (get_player_singleton()->play_state != PLAY_BACKWARDS_E) {
        if (nk_button_label(context, "BACK") && get_player_singleton()->play_state != STOP_START_E) {
            get_player_singleton()->play_state = PLAY_BACKWARDS_E;
        }
    } else {
        if (nk_button_label(context, "STOP") && get_player_singleton()->play_state != STOP_START_E) {
            get_player_singleton()->play_state = STOP_MIDDLE_E;
        }
    }
}

void _play(struct nk_context * context) {
    if (get_player_singleton()->play_state != PLAY_FORWARDS_E) {
        if (nk_button_label(context, "FORW") && get_player_singleton()->play_state != STOP_END_E) {
            get_player_singleton()->play_state = PLAY_FORWARDS_E;
        }
    } else {
        if (nk_button_label(context, "STOP") && get_player_singleton()->play_state != STOP_END_E) {
            get_player_singleton()->play_state = STOP_MIDDLE_E;
        }
    }
}

void _next(struct nk_context * context) {
    if (nk_button_label(context, "NEXT")) {
        get_player_singleton()->play_state = STOP_MIDDLE_E;
        state_provider(NEXT_VALUE_DS);
    }
}

void _to_end(struct nk_context * context) {
    if (nk_button_label(context, "END") && get_player_singleton()->solve_state == SOLVE_FINISHED_E) {
        state_provider(END_VALUE_DS);
    }
}

