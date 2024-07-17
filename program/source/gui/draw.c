#include <gui/draw.h>
#include <gui/default.h>
#include <instance/settings.h>

void draw(struct nk_glfw * glfw) {
    glViewport(0, 0, WINDOW_HEIGHT, WINDOW_WIDTH);
    glClear(GL_COLOR_BUFFER_BIT);
    nk_glfw3_render(glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
    glfwSwapBuffers(glfw->win);
}