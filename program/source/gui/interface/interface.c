#include <gui/interface/interface.h>
#include <gui/interface/solver.h>
#include <gui/interface/grid.h>

#include <instance/settings.h>

#include <wchar.h>

void _player(struct nk_context * context, struct nk_media media);
void _to_start(struct nk_context * context, struct nk_image image);
void _previus(struct nk_context * context, struct nk_image image);
void _playback(struct nk_context * context, struct nk_image play, struct nk_image stop);
void _play(struct nk_context * context, struct nk_image play, struct nk_image stop);
void _next(struct nk_context * context, struct nk_image image);
void _to_end(struct nk_context * context, struct nk_image image);

void interface(struct nk_super * super) {
    struct nk_context * context = super->context;
    board_s board = super->solver.board;

    if (nk_begin(super->context, "KAKURO BOARD", nk_rect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT), NK_WINDOW_BORDER)) {
        grid(context, board);
        _player(context, super->media);
    }
    nk_end(super->context);
}

void _player(struct nk_context * context, struct nk_media media) {
    nk_layout_row_template_begin(context, 50);

    nk_layout_row_template_push_static(context, 50);
    nk_layout_row_template_push_static(context, 50);
    nk_layout_row_template_push_static(context, 50);
    nk_layout_row_template_push_static(context, 50);
    nk_layout_row_template_push_static(context, 50);
    nk_layout_row_template_push_static(context, 50);
    nk_layout_row_template_push_dynamic(context);

    nk_layout_row_template_end(context);

    _to_start(context, media.begin);
    _previus(context, media.previous);
    _playback(context, media.reverse, media.pause);
    _play(context, media.play, media.pause);
    _next(context, media.next);
    _to_end(context, media.end);

    get_settings_singleton()->time_ms = MAX_DELAY_MS - nk_slide_int(context, 0, MAX_DELAY_MS - get_settings_singleton()->time_ms, MAX_DELAY_MS, MAX_DELAY_MS / 10);
}

void _to_start(struct nk_context * context, struct nk_image image) {
    if (nk_button_image(context, image)) {
        state_provider(START_VALUE_DS);
    }
}

void _previus(struct nk_context * context, struct nk_image image) {
    if (nk_button_image(context, image)) {
        get_player_singleton()->play_state = STOP_MIDDLE_E;
        state_provider(PREV_VALUE_DS);
    }
}

void _playback(struct nk_context * context, struct nk_image play, struct nk_image stop) {
    if (get_player_singleton()->play_state != PLAY_BACKWARDS_E) {
        if (nk_button_image(context, play) && get_player_singleton()->play_state != STOP_START_E) {
            get_player_singleton()->play_state = PLAY_BACKWARDS_E;
        }
    } else {
        if (nk_button_image(context, stop) && get_player_singleton()->play_state != STOP_START_E) {
            get_player_singleton()->play_state = STOP_MIDDLE_E;
        }
    }
}

void _play(struct nk_context * context, struct nk_image play, struct nk_image stop) {
    if (get_player_singleton()->play_state != PLAY_FORWARDS_E) {
        if (nk_button_image(context, play) && get_player_singleton()->play_state != STOP_END_E) {
            get_player_singleton()->play_state = PLAY_FORWARDS_E;
        }
    } else {
        if (nk_button_image(context, stop) && get_player_singleton()->play_state != STOP_END_E) {
            get_player_singleton()->play_state = STOP_MIDDLE_E;
        }
    }
}

void _next(struct nk_context * context, struct nk_image image) {
    if (nk_button_image(context, image)) {
        get_player_singleton()->play_state = STOP_MIDDLE_E;
        state_provider(NEXT_VALUE_DS);
    }
}

void _to_end(struct nk_context * context, struct nk_image image) {
    if (nk_button_image(context, image) && get_player_singleton()->solve_state == SOLVE_FINISHED_E) {
        state_provider(END_VALUE_DS);
    }
}

