#include "glad.h"
#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"
#include "learn_opengl/shader_m.h"

#include <stb_image.h>
#include <learn_opengl/file_system.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "camera_circle", nullptr, nullptr);
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
    spdlog::info("GL_VENDOR: {0}", (char *)glGetString(GL_VENDOR));
    spdlog::info("GL_RENDERER: {0}", (char *)glGetString(GL_RENDERER));
    spdlog::info("GL_VERSION: {0}", (char *)glGetString(GL_VERSION));
    spdlog::info("GL_SHADING_LANGUAGE_VERSION: {0}", (char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    spdlog::info("GL_MAX_VERTEX_ATTRIBS: {0}", nrAttributes);
    spdlog::info("======================================================");
    GLint num_of_extensions, i;
    glGetIntegerv(GL_NUM_EXTENSIONS, &num_of_extensions);
    for (i = 0; i < num_of_extensions; i++)
    {
        spdlog::info("GL_EXTENSIONS: {0}", (char *)glGetStringi(GL_EXTENSIONS, i));
    }

    Shader shader("camera_circle.vs", "camera_circle.fs");

    // setup vertex data (and buffer)
    // =============================
    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)0);
    glEnableVertexAttribArray(0);

    // texture coord
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    // unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // load texture
    // ===========
    unsigned int texture0;
    glGenTextures(1, &texture0);
    glBindTexture(GL_TEXTURE_2D, texture0); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char *data = stbi_load(FileSystem::getResPath("/textures/container.jpg").c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // load another texture
    // ===================
    stbi_set_flip_vertically_on_load(true);
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    data = stbi_load(FileSystem::getResPath("/textures/awesomeface.png").c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    shader.use();
    shader.setInt("texture0", 0);
    shader.setInt("texture1", 1);

    // setup imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    float viewTranslate = -3.0f;
    float fov = 45.0f;

    /*

    因为OpenGL是一个三角形一个三角形地来绘制你的立方体的，所以即便之前那里有东西它也会覆盖之前的像素。因为这个原因，有些三角形会被绘制在其它三角形上面，虽然它们本不应该是被覆盖的。


    OpenGL存储它的所有深度信息于一个Z缓冲(Z-buffer)中，也被称为深度缓冲(Depth Buffer)。GLFW会自动为你生成这样一个缓冲（就像它也有一个颜色缓冲来存储输出图像的颜色）。深度值存储在每个片段里面（作为片段的z值），当片段想要输出它的颜色时，OpenGL会将它的深度值和z缓冲进行比较，如果当前的片段在其它片段之后，它将会被丢弃，否则将会覆盖。这个过程称为深度测试(Depth Testing)，它是由OpenGL自动完成的。
    */
    glEnable(GL_DEPTH_TEST); // 开启深度测试

    glm::vec3 cubePositions[] = {
       glm::vec3(1.0f,  0.0f,  0.0f),
       glm::vec3(2.0f,  5.0f, -15.0f),
       glm::vec3(-1.5f, -2.2f, -2.5f),
       glm::vec3(-3.8f, -2.0f, -12.3f),
       glm::vec3(2.4f, -0.4f, -3.5f),
       glm::vec3(-1.7f,  3.0f, -7.5f),
       glm::vec3(1.3f, -2.0f, -2.5f),
       glm::vec3(1.5f,  2.0f, -2.5f),
       glm::vec3(1.5f,  0.2f, -1.5f),
       glm::vec3(-1.3f,  1.0f, -1.5f)
    };


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
            ImGui::SliderFloat("viewTranslate", &viewTranslate, -5.0f, -1.0f);
            ImGui::SliderFloat("fov", &fov, 0.0f, 90.0f);
            ImGui::End();
        }

        glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 因为开了深度缓冲

        glActiveTexture(GL_TEXTURE0); // 在绑定纹理之前先激活纹理单元
        glBindTexture(GL_TEXTURE_2D, texture0);

        glActiveTexture(GL_TEXTURE1); // 在绑定纹理之前先激活纹理单元
        glBindTexture(GL_TEXTURE_2D, texture1);

        glm::mat4 u_Model       = glm::mat4(1.0);
        glm::mat4 u_View        = glm::mat4(1.0);
        glm::mat4 u_Projection  = glm::mat4(1.0);

        // 旋转
        u_Model = glm::rotate(u_Model, glm::radians(50.0f) * (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
        // 平移：看起来物体后退
        u_View = glm::translate(u_View, glm::vec3(0.0f, 0.0f, viewTranslate));
        // 使用一个简单的透视投影
        float aspectRatio = SCREEN_WIDTH / SCREEN_HEIGHT;
        u_Projection = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 1000.0f);


        float radius = 10.0f;
        float camX = static_cast<float>(sin(glfwGetTime()) * radius);
        float camZ = static_cast<float>(cos(glfwGetTime()) * radius);
        // 摄像机位置 目标位置 up向量
        u_View = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        shader.use();
        unsigned int u_ModelLoc      = glGetUniformLocation(shader.ID, "u_Model");
        unsigned int u_ViewLoc       = glGetUniformLocation(shader.ID, "u_View");
        unsigned int u_ProjectionLoc = glGetUniformLocation(shader.ID, "u_Projection");
        // 三种方式设置unifrom
        glUniformMatrix4fv(u_ModelLoc, 1, GL_FALSE, glm::value_ptr(u_Model));
        glUniformMatrix4fv(u_ViewLoc, 1, GL_FALSE, &u_View[0][0]);
        shader.setMat4("u_Projection", u_Projection);


        // draw
        glBindVertexArray(VAO);
        // glDrawElements(GL_TRIANGLES, , GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 36); // 画面36个顶点的立方体
        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 u_Model = glm::mat4(1.0f);
            u_Model = glm::translate(u_Model, cubePositions[i]);
            u_Model = glm::rotate(u_Model, glm::radians(50.0f) * (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
            float angle = 20.0f * i;
            u_Model = glm::rotate(u_Model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.setMat4("u_Model", u_Model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }


        // Rendering imgui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // glfw: swap buffer and poll io event
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    glfwDestroyWindow(window);
    glDeleteShader(shader.ID);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    return 0;
}

void frame_buffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void processInput(GLFWwindow *window)
{
    bool key_escape_pressed = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
    if (key_escape_pressed)
    {
        glfwSetWindowShouldClose(window, true);
        return;
    }
}
