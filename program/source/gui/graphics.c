#include <GL/glew.h>
#include <GLFW/glfw3.h>

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
void _glfw_initialize(GLFWwindow ** window, int * width, int * height);

void gui(void) {
    struct nk_glfw glfw = { 0 };
    int width = 0, height = 0;
    struct nk_super super = { 0 };
    static GLFWwindow * window;

    _glfw_initialize(&window, &width, &height);
    _glew_initialize();

    struct nk_context * context = nk_glfw3_init(&glfw, window, NK_GLFW3_INSTALL_CALLBACKS);
    struct nk_font_atlas atlas;
    struct nk_font_atlas * atlas_ptr = &atlas;
    nk_glfw3_font_stash_begin(&glfw, &atlas_ptr);
    nk_glfw3_font_stash_end(&glfw);

    get_settings_singleton()->filepath = "test/puzzles/medium/5.kkr";
    FILE * fp = fopen(get_settings_singleton()->filepath, "rb");
    expect(fp, NO_ACTION, "file pointer variable (fp) is NULL (%p): %s", (void*)fp, strerror(errno));
    board_s board = create_board(fp);

    super.board   = &board;
    super.context = context;
    super.atlas   = &glfw.atlas;

    while (!glfwWindowShouldClose(window)) {
        input(&glfw);
        interface(&super);
        draw(&glfw);
    }

    nk_glfw3_shutdown(&glfw);
    glfwTerminate();
}

void _glfw_initialize(GLFWwindow ** window, int * width, int * height) {
    glfwSetErrorCallback(error_callback);

    error_mode = ASSERT_E;
    expect(glfwInit(), NO_ACTION, "GLFW failed to initialize");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "SLNOSLAV", NULL, NULL);
    glfwMakeContextCurrent(*window);
    glfwGetWindowSize(*window, width, height);
}

void _glew_initialize(void) {
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glewExperimental = GL_TRUE;
    error_mode = ASSERT_E;
    expect(glewInit() == GLEW_OK, NO_ACTION, "Failed to setup GLEW");
}
