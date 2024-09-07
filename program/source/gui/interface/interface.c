#include <gui/interface/interface.h>
#include <gui/interface/solver.h>
#include <gui/interface/grid.h>

#include <instance/settings.h>
#include <instance/statistics.h>

#include <wchar.h>

void _statistics(struct nk_context * context);
void _evaluation_stats(struct nk_context * context);
void _depth_first_search_stats(struct nk_context * context);

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

    if (nk_begin(context, "KAKURO BOARD", nk_rect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT), NK_WINDOW_BORDER)) {
        _statistics(context);
        grid(context, board);
        _player(context, super->media);
    }
    nk_end(super->context);
}

#define MAX_STAT_STRING_LENGTH 320
#define STAT_HEIGHT 15
void _statistics(struct nk_context * context) {
    float temp_height = context->style.font->height;
    struct nk_user_font * font = context->style.font;

    font->height = STAT_HEIGHT;

    _evaluation_stats(context);
    _depth_first_search_stats(context);

    font->height = temp_height;
}

void _evaluation_stats(struct nk_context * context) {
    struct {
        int  length[3];
        char string[3][MAX_STAT_STRING_LENGTH];
    } data;

    nk_layout_row_template_begin(context, 20);

    nk_layout_row_template_push_dynamic(context);
    nk_layout_row_template_push_dynamic(context);
    nk_layout_row_template_push_dynamic(context);
    nk_layout_row_template_push_dynamic(context);
    nk_layout_row_template_push_dynamic(context);
    nk_layout_row_template_push_dynamic(context);

    nk_layout_row_template_end(context);

    data.length[0] = get_settings_singleton()->is_backtrack ?
        sprintf(data.string[0], "%.0lf", get_stat_singleton()->backtrack_count) : sprintf(data.string[0], "n/a");
    data.length[1] = get_settings_singleton()->is_forward_check ?
        sprintf(data.string[1], "%.0lf", get_stat_singleton()->bad_forward_check_count) : sprintf(data.string[1], "n/a");
    data.length[2] = get_settings_singleton()->is_arc_consistency ?
        sprintf(data.string[2], "%.0lf", get_stat_singleton()->bad_arc_consistency_count) : sprintf(data.string[2], "n/a");

#define BT_TEXT "Backtracks:"
    nk_text(context, BT_TEXT, sizeof(BT_TEXT) - 1, NK_TEXT_RIGHT);
    nk_text(context, data.string[0], data.length[0], NK_TEXT_LEFT);
#define FCH_TEXT "Bad Forward checks:"
    nk_text(context, FCH_TEXT, sizeof(FCH_TEXT) - 1, NK_TEXT_RIGHT);
    nk_text(context, data.string[1], data.length[1], NK_TEXT_LEFT);
#define AC_TEXT "Arc inconsistency:"
    nk_text(context, AC_TEXT, sizeof(AC_TEXT) - 1, NK_TEXT_RIGHT);
    nk_text(context, data.string[2], data.length[2], NK_TEXT_LEFT);
}

void _depth_first_search_stats(struct nk_context * context) {
    struct {
        int  length[2];
        char string[2][MAX_STAT_STRING_LENGTH];
    } data;

    nk_layout_row_template_begin(context, 20);

    nk_layout_row_template_push_dynamic(context);
    nk_layout_row_template_push_dynamic(context);
    nk_layout_row_template_push_dynamic(context);
    nk_layout_row_template_push_dynamic(context);
    nk_layout_row_template_push_dynamic(context);
    nk_layout_row_template_push_dynamic(context);

    nk_layout_row_template_end(context);

    data.length[0] = sprintf(data.string[0], "%.0lf", get_stat_singleton()->dfs_iteration_count);
    data.length[1] = sprintf(data.string[1], "%.0lf", get_stat_singleton()->dfs_stack_max_size);

#define DFS_ITER "DFS iterations:"
    nk_text(context, DFS_ITER, sizeof(DFS_ITER) - 1, NK_TEXT_RIGHT);
    nk_text(context, data.string[0], data.length[0], NK_TEXT_LEFT);

    nk_text(context, "", 0, NK_TEXT_LEFT);
    nk_text(context, "", 0, NK_TEXT_LEFT);

#define DFS_MAX "DFS max stack:"
    nk_text(context, DFS_MAX, sizeof(DFS_MAX) - 1, NK_TEXT_RIGHT);
    nk_text(context, data.string[1], data.length[1], NK_TEXT_LEFT);
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

