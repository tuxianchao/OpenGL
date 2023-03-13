#include "glad.h"
#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"
#include "learn_opengl/shader_s.h"

void frame_buffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;


int main()
{

    // glfw: initialize and configure
    // =============================
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__

    // create glfw window
    // =================
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "shaders_class", nullptr, nullptr);
    if (window == nullptr)
    {
        spdlog::error("Failed to create glfw window.");
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

    // glad: load all OpenGL function pointer
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        spdlog::error("Failed to initialize GLAD");
        glfwTerminate();
        return -1;
    }


    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    spdlog::info("OpenGL::GL_MAX_VERTEX_ATTRIBS = {0}", nrAttributes);

    Shader shader("shaders_class.vs", "shaders_class.fs");

    // setup vertex data (and buffer)
    // =============================

    float vertices[] = {
        // 位置              // 颜色
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
    };

    GLuint VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    // unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    unsigned int frame = 0;
    while (!glfwWindowShouldClose(window))
    {
        frame++;
        // input
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // glfw: swap buffer and poll io event
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    glDeleteShader(shader.ID);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    return 0;
}



void frame_buffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

}
void processInput(GLFWwindow* window)
{
    bool key_escape_pressed = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
    if (key_escape_pressed)
    {
        glfwSetWindowShouldClose(window, true);
        return;
    }


}
