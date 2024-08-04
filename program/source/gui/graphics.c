#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <style.h>

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
#include <gui/draw.h>
#include <gui/interface/interface.h>
#include <gui/interface/solver.h>

#include <instance/expect.h>
#include <instance/settings.h>
#include <errno.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

static void error_callback(int number, const char * description) {
    error_mode = ASSERT_E;
    expect(NULL, NO_ACTION, "Error %d: %s\n", number, description);
}

void _glew_initialize(void);
void _glfw_initialize(GLFWwindow ** window, int * width, int * height);
struct nk_super _create_super(struct nk_glfw * glfw, GLFWwindow * window);
void _destroy_super(struct nk_super * super);
struct nk_media _create_media(void);
struct nk_image icon_load(const char *filename);

void gui(void) {
    struct nk_glfw glfw = { 0 };
    int width = 0, height = 0;
    static GLFWwindow * window;

    _glfw_initialize(&window, &width, &height);
    _glew_initialize();
    struct nk_super super = _create_super(&glfw, window);

    solve(&super.solver);
    while (!glfwWindowShouldClose(window)) {
        input(&glfw);
        interface(&super);
        draw(&glfw);
    }
    _destroy_super(&super);

    nk_glfw3_shutdown(&glfw);
    glfwTerminate();
}

void _glfw_initialize(GLFWwindow ** window, int * width, int * height) {
    glfwSetErrorCallback(error_callback);

    error_mode = ASSERT_E;
    expect(glfwInit(), NO_ACTION, "[ERROR] GLFW failed to initialize");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "SLNOSLAV", NULL, NULL);
    glfwMakeContextCurrent(*window);
    glfwGetWindowSize(*window, width, height);

    GLFWimage favicon;

    favicon.pixels = stbi_load("./assets/favicon.png", &favicon.width, &favicon.height, 0, 4);
    error_mode = ASSERT_E;
    expect(favicon.pixels, NO_ACTION, "[ERROR] Failed to load favicon image");

    glfwSetWindowIcon(*window, 1, &favicon);
    stbi_image_free(favicon.pixels);
}

void _glew_initialize(void) {
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glewExperimental = GL_TRUE;
    GLenum error = glewInit();

    error_mode = ASSERT_E;
    expect(error == GLEW_OK, NO_ACTION, "[ERROR] Failed to setup GLEW: %s", (char*)glewGetErrorString(error));
}

struct nk_super _create_super(struct nk_glfw * glfw, GLFWwindow * window) {
    struct nk_context * context = nk_glfw3_init(glfw, window, NK_GLFW3_INSTALL_CALLBACKS);
    struct nk_font_config cfg = nk_font_config(0);
    cfg.oversample_h = 3;
    cfg.oversample_v = 2;
    struct nk_font_atlas * atlas;

    nk_glfw3_font_stash_begin(glfw, &atlas);
    struct nk_font * roboto = nk_font_atlas_add_from_file(atlas, "./assets/font/Roboto-Regular.ttf", 20.0f, &cfg);
    nk_glfw3_font_stash_end(glfw);
    nk_init_default(context, &roboto->handle);

    set_style(context, LIGHT_MODE);

    FILE * fp = fopen(get_settings_singleton()->filepath, "rb");
    expect(fp, NO_ACTION, "[ERROR] File pointer variable (fp) is NULL (%p): %s", (void*)fp, strerror(errno));
    board_s board = create_board(fp);
    fclose(fp);

    struct nk_media media = _create_media();

    return (struct nk_super) {
        .context = context,
        .solver = {
            .board = board,
        },
        .media = media,
    };
}

void _destroy_super(struct nk_super * super) {
    destroy_board(&super->solver.board);
}

struct nk_media _create_media(void) {
    glEnable(GL_TEXTURE_2D);

    struct nk_media media = { 0 };
    media.play = icon_load("./assets/icons/forwards.png");
    media.pause = icon_load("./assets/icons/stop.png");
    media.begin = icon_load("./assets/icons/start.png");
    media.end = icon_load("./assets/icons/end.png");
    media.reverse = icon_load("./assets/icons/backwards.png");
    media.next = icon_load("./assets/icons/next.png");
    media.previous = icon_load("./assets/icons/previous.png");

    return media;
}

struct nk_image icon_load(const char *filename) {
    int x,y,n;
    GLuint tex;
    unsigned char *data = stbi_load(filename, &x, &y, &n, 0);

    error_mode = ASSERT_E;
    expect(data, NO_ACTION, "[ERROR]: failed to load image: %s", filename);

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    return nk_image_id((int)tex);
}