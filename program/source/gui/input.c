#include <gui/input.h>

void input(struct nk_glfw * glfw) {
    glfwPollEvents();
    nk_glfw3_new_frame(glfw);
}