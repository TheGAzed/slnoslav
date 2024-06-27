#include <gui/graphics.h>

#include <GL/glew.h>

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

#include <instance/expect.h>

static void error_callback(int number, const char * description) {
    error_mode = DEFAULT_E;
    expect(NULL, NO_ACTION, "Error %d: %s\n", number, description);
}

void _glfw_initialize_window(GLFWwindow * window);
void _glew_initialize(void);
void _font_stash(struct nk_glfw * glfw);

void _input(struct nk_glfw * glfw);
void _interface(struct nk_context * context);
void _draw(struct nk_glfw * glfw, GLFWwindow * window);

void gui(void) {
    static GLFWwindow * window;
    _glfw_initialize_window(window);
    struct nk_glfw glfw = { 0 };
    struct nk_context * context = nk_glfw3_init(&glfw, window, NK_GLFW3_INSTALL_CALLBACKS);

    _glew_initialize();
    _font_stash(&glfw);

    while (!glfwWindowShouldClose(window)) {
        _input(&glfw);
        _interface(context);
        _draw(&glfw, window);
    }

    nk_glfw3_shutdown(&glfw);
    glfwTerminate();
}

void _glfw_initialize_window(GLFWwindow * window) {
    error_mode = EXIT_E;
    expect(window, NO_ACTION, "GLFWwindow pointer is NULL (%p)", (void*)window);
    expect(glfwInit(), NO_ACTION, "GLFW failed to initialize");
    
    glfwSetErrorCallback(error_callback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Demo", NULL, NULL);
    glfwMakeContextCurrent(window);
}

void _glew_initialize(void) {
    glewExperimental = GL_TRUE;
    error_mode = EXIT_E;
    expect(glewInit() == GLEW_OK, NO_ACTION, "GLEW setup failed");
}

void _font_stash(struct nk_glfw * glfw) {
    struct nk_font_atlas *atlas;
    nk_glfw3_font_stash_begin(glfw, &atlas);
    nk_glfw3_font_stash_end(glfw);
}

void _input(struct nk_glfw * glfw) {
    glfwPollEvents();
    nk_glfw3_new_frame(glfw);
}

void _interface(struct nk_context * context) {
    if (nk_begin(context, "Anything window", nk_rect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT), NK_WINDOW_BORDER)) {
        nk_layout_row_dynamic(context, 120, 1);
        nk_label(context, "Hello world!", NK_TEXT_CENTERED);
            
        nk_layout_row_dynamic(context, 50, 1);
        nk_label(context, "Hello world!", NK_TEXT_LEFT);
            
        nk_layout_row_static(context, 30, 80, 1);
        if (nk_button_label(context, "AnyButton")) fprintf(stdout, "button pressed\n");
    }
    nk_end(context);
}

void _draw(struct nk_glfw * glfw, GLFWwindow * window) {
    glViewport(0, 0, WINDOW_HEIGHT, WINDOW_WIDTH);
    glClear(GL_COLOR_BUFFER_BIT);
    nk_glfw3_render(glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
    glfwSwapBuffers(window);
}