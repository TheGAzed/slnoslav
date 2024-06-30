#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define WINDOW_WIDTH  1200
#define WINDOW_HEIGHT 800

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#define NK_KEYSTATE_BASED_INPUT
#include <nuklear.h>
#include <nuklear_glfw_gl3.h>

#undef NK_IMPLEMENTATION
#undef NK_GLFW_GL3_IMPLEMENTATION
#include <gui/graphics.h>
#include <gui/input.h>
#include <gui/interface.h>
#include <gui/draw.h>

#include <instance/expect.h>
#include <instance/settings.h>
#include <errno.h>

static void error_callback(int number, const char * description) {
    error_mode = ASSERT_E;
    expect(NULL, NO_ACTION, "Error %d: %s\n", number, description);
}

void _glew_initialize(void);

#define TEST
#ifndef TEST

void gui(void) {
    struct nk_glfw glfw = { 0 };
    static GLFWwindow * window;
    int width = 0, height = 0;
    struct nk_context * context;
    error_mode = ASSERT_E;

    glfwSetErrorCallback(error_callback);
    expect(glfwInit(), NO_ACTION, "GLFW failed to initialize");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Demo", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwGetWindowSize(window, &width, &height);

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glewExperimental = GL_TRUE;
    expect(glewInit() == GLEW_OK, NO_ACTION, "Failed to setup GLEW");

    context = nk_glfw3_init(&glfw, window, NK_GLFW3_INSTALL_CALLBACKS);
    struct nk_font_atlas atlas;
    {
        struct nk_font_atlas * atlas_ptr = &atlas;
        nk_glfw3_font_stash_begin(&glfw, &atlas_ptr);
        nk_glfw3_font_stash_end(&glfw);
    }

    FILE * fp = fopen(get_settings_singleton()->filepath, "rb");
    expect(fp, NO_ACTION, "file pointer variable (fp) is NULL (%p): %s", (void*)fp, strerror(errno));
    Kakuro board = init_kakuro(fp);

    nk_font_atlas_init_default(&atlas);
    nk_font_atlas_begin(&atlas);

    _glew_initialize();
    while (!glfwWindowShouldClose(window)) {
        input(&glfw);
        interface(context, atlas, &board);
        draw(&glfw, window);
    }

    nk_font_atlas_clear(&atlas);
    nk_glfw3_shutdown(&glfw);
    glfwTerminate();
}

#else 

void gui(void) {
    struct nk_glfw glfw = {0};
    static GLFWwindow *win;
    int width = 0, height = 0;
    struct nk_context *ctx;
    struct nk_colorf bg;

    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        fprintf(stdout, "[GFLW] failed to init!\n");
        exit(1);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    win = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Demo", NULL, NULL);
    glfwMakeContextCurrent(win);
    glfwGetWindowSize(win, &width, &height);

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glewExperimental = 1;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to setup GLEW\n");
        exit(1);
    }

    ctx = nk_glfw3_init(&glfw, win, NK_GLFW3_INSTALL_CALLBACKS);
    {
        struct nk_font_atlas *atlas;
        nk_glfw3_font_stash_begin(&glfw, &atlas);
        nk_glfw3_font_stash_end(&glfw);
    }

    bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;
    while (!glfwWindowShouldClose(win)) {
        glfwPollEvents();
        nk_glfw3_new_frame(&glfw);

        if (nk_begin(ctx, "Demo", nk_rect(50, 50, 230, 250),
            NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
            NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE))
        {
            enum {EASY, HARD};
            static int op = EASY;
            static int property = 20;
            nk_layout_row_static(ctx, 30, 80, 1);
            if (nk_button_label(ctx, "button"))
                fprintf(stdout, "button pressed\n");

            nk_layout_row_dynamic(ctx, 30, 2);
            if (nk_option_label(ctx, "easy", op == EASY)) op = EASY;
            if (nk_option_label(ctx, "hard", op == HARD)) op = HARD;

            nk_layout_row_dynamic(ctx, 25, 1);
            nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1);

            nk_layout_row_dynamic(ctx, 20, 1);
            nk_label(ctx, "background:", NK_TEXT_LEFT);
            nk_layout_row_dynamic(ctx, 25, 1);
            if (nk_combo_begin_color(ctx, nk_rgb_cf(bg), nk_vec2(nk_widget_width(ctx),400))) {
                nk_layout_row_dynamic(ctx, 120, 1);
                bg = nk_color_picker(ctx, bg, NK_RGBA);
                nk_layout_row_dynamic(ctx, 25, 1);
                bg.r = nk_propertyf(ctx, "#R:", 0, bg.r, 1.0f, 0.01f,0.005f);
                bg.g = nk_propertyf(ctx, "#G:", 0, bg.g, 1.0f, 0.01f,0.005f);
                bg.b = nk_propertyf(ctx, "#B:", 0, bg.b, 1.0f, 0.01f,0.005f);
                bg.a = nk_propertyf(ctx, "#A:", 0, bg.a, 1.0f, 0.01f,0.005f);
                nk_combo_end(ctx);
            }
        }
        nk_end(ctx);

        glfwGetWindowSize(win, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(bg.r, bg.g, bg.b, bg.a);
        nk_glfw3_render(&glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
        glfwSwapBuffers(win);
    }
    nk_glfw3_shutdown(&glfw);
    glfwTerminate();
}

#endif

void _glew_initialize(void) {
    glewExperimental = GL_TRUE;
    error_mode = EXIT_E;
    expect(glewInit() == GLEW_OK, NO_ACTION, "GLEW setup failed");
}
