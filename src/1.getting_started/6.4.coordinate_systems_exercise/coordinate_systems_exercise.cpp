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

/*

 局部空间 Local Space
 世界空间 World Space
 观察空间 View Space(Eye Space)
 裁剪空间 Clip Space 在一个顶点着色器运行的最后，OpenGL期望所有的坐标都能落在一个特定的范围内，且任何在这个范围之外的点都应该被裁剪掉(Clipped)。被裁剪掉的坐标就会被忽略，所以剩下的坐标就将变为屏幕上可见的片段。这也就是裁剪空间(Clip Space)名字的由来。
 屏幕空间 Screen Space

 Local Space ==(model matrix)       =>
 World Space ==(view matrix)        =>
 View Space  ==(projection matrix)  =>
 Clip Space  ==(viewport transform) => Screen Space

 1. 局部坐标是一个相对于局部原点的坐标，也就是是物体的起始坐标，有点像通常说的localX localY localZ
 2. 下一步就是将局部坐标变化为世界空间坐标，这个坐标是是相对于世界原摆放后的坐标，有点像通常说的worldX worldY worldZ
 3. 接下来就是将世界坐标变化到关键空间了，使得每一个坐标都是从观察者或者说摄像机的角度进行观察的
 4. 到了观察者空间后，就系要将其投影到裁剪坐标，裁剪坐标会被处理到-1.0到1.0之间，，并且会判断哪些坐标会出现在屏幕上
 5. 最后，将裁剪坐标变换为屏幕坐标，这个过程使用一个叫做视口变换的过程，将位于-1.0到1.0之间的左边变化到由glViewport函数做定义的坐标范围
 6. 最后进行光栅化，转化为片段


model matrix        ：模型矩阵，理解意义上的描述就是将物体的局部坐标通过平移旋转缩放等操作变换到场景(世界)的不同位置
view matrix         ：观察矩阵，理解意义上来说，观察空间是将世界空间的坐标转化为用户前方的坐标的结果，就是在世界空间坐标的基础上，
                      通过一列的位移和旋转来完成，平移或者旋转场景使得特定对象被变换到摄像机的前方。
projection matrix   ：投影矩阵，把顶点从观察空间变换到裁剪空间，投影矩阵相当于指定了一个坐标范围，例如在每个维度上的-1000到1000
                      接下来会按照这个范围把坐标变换到标准化范围(-1.0,1.0),所有在此之外的都会被裁剪掉，例如坐标（1250,750,500）
                     因为x超了，最后会转化出一个大于1.0的坐标，因此会被裁剪掉（不过如果只是图元的一部分超了，那么OpenGL会重建这个图元）

                     * 由投影矩阵定义的观察箱(view Box)被称为平截头体（frustum） 每个出现在平截头体范围内的坐标最后都会出现在用户的屏幕上
                     * 将特定范围内的坐标转换为标准化设备坐标的过程称之为投影，因为投影矩阵最终能将3d坐标很容易的投影到2d的表转化设备坐标系中
                     * 一旦所有的顶点被变换到裁剪空间，最终的操作，透视除法将会执行，在这个过程中我们将位置向量的x，y，z分量分别除以向量的齐次w分量；透视除法是将4D裁剪空间坐标变换为3D标准化设备坐标的过程。这一步会在每一个顶点着色器运行的最后被自动执行
                     * 最终的坐标会被映射到屏幕空间，并被变换为片段


正交投影于透视投影

* 正交投影，一个类似于立方体的平截头体，主要参数，宽，高，近裁面和远裁面，w分量全为1.0
* 透视投影，近大远小的，主要参数fov（视野field of view），近裁面和远裁面

最终  V(clip) =   M(projection) * M(view) * M(model) * V(local)


*/
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
    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "coordinate_systems_exercise", nullptr, nullptr);
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

    Shader shader("coordinate_systems_exercise.vs", "coordinate_systems_exercise.fs");

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
            if (i % 3 == 0)
            {
                u_Model = glm::rotate(u_Model, glm::radians(50.0f) * (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
            }
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
