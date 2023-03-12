#include "glad.h"
#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"
#include "learn_opengl/shader_s.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_WIDTH, "shaders_exercise1", nullptr, nullptr);
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

    // print opengl info
    // ================
    spdlog::info("======================================================");
    spdlog::info("OpenGL Info");
    spdlog::info("GL_VENDOR: {0}", (char*)glGetString(GL_VENDOR));
    spdlog::info("GL_RENDERER: {0}", (char*)glGetString(GL_RENDERER));
    spdlog::info("GL_VERSION: {0}", (char*)glGetString(GL_VERSION));
    spdlog::info("GL_SHADING_LANGUAGE_VERSION: {0}", (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    spdlog::info("GL_MAX_VERTEX_ATTRIBS: {0}", nrAttributes);
    spdlog::info("======================================================");
    GLint num_of_extensions, i;
    glGetIntegerv(GL_NUM_EXTENSIONS, &num_of_extensions);
    for (i = 0; i < num_of_extensions; i++)
    {
        spdlog::info("GL_EXTENSIONS: {0}", (char*)glGetStringi(GL_EXTENSIONS, i));
    }

    Shader shader("shaders_exercise2.vs", "shaders_exercise2.fs");

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

  

    // setup imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    bool u_enableColor = false;
    ImVec4 u_Color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    float u_xOffset = 0;
    float u_yOffset = 0;

    unsigned int frame = 0;
    while (!glfwWindowShouldClose(window))
    {
        frame++;
        // input
        processInput(window);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            ImGui::Begin("This is some useful text.");
            ImGui::NewLine();
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::NewLine();
            ImGui::SliderFloat("u_xOffset", &u_xOffset, -1.0f, 1.0f);
            ImGui::SliderFloat("u_yOffset", &u_yOffset, -1.0f, 1.0f);
            ImGui::Checkbox("u_enableColor", &u_enableColor);
            ImGui::ColorEdit4("u_Color", (float*)&u_Color);

            ImGui::End();
        }

        glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        

        shader.use();
        shader.setBool("u_enableColor", u_enableColor);
        shader.setFloat("u_ColorX", u_Color.x);
        shader.setFloat("u_ColorY", u_Color.y);
        shader.setFloat("u_ColorZ", u_Color.z);
        shader.setFloat("u_ColorW", u_Color.w);
        shader.setFloat("u_xOffset", u_xOffset);
        shader.setFloat("u_yOffset", u_yOffset);


        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);


        // Rendering imgui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // glfw: swap buffer and poll io event
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    glfwDestroyWindow(window);
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
