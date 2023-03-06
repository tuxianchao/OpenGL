#include "glad.h"
#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"
#include <iostream>

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;
const char *WINDOW_TITLE = "1.1.hello_window";

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

int main()
{
    spdlog::info("start.....");
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    //  glfw create window
    //  ------------------
    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
    if (window == NULL)
    {
        std::cout << "ERROR::Failed to create glfw window" << std::endl;
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointer
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        spdlog::info("Faile to initialize GLAD.");
        glfwTerminate();
        return -1;
    }

    // render loop

    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // glfw: swap buffer and poll IO event(key pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // exit
    glfwTerminate();
    return 0;

    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    spdlog::info("frame buffer resize width: {0}, height: {1}", width, height);
    glViewport(0, 0, width, height);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
        return;
    }
}
