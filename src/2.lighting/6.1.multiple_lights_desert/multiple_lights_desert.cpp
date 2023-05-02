#include "glad.h"
#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"
#include <learn_opengl/shader_m.h>
#include <learn_opengl/camera.h>

#include <learn_opengl/file_system.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


void frame_buffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
unsigned int loadTexture(char const* path);


const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;


// camera
Camera camera(glm::vec3(0.0f, 0.0f, 6.0f));

float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastTime = 0.0f; // 上一帧的时间

bool firstMouse = true;
float lastX = SCREEN_WIDTH / 2.0;
float lastY = SCREEN_HEIGHT / 2.0;



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
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "multiple_lights_desert", nullptr, nullptr);
    if (window == nullptr)
    {
        spdlog::error("Failed to create glfw window.");
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
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

    Shader lightingShader("multiple_lights_desert.vs", "multiple_lights_desert.fs");
    Shader lightCubeShader("light_cube.vs", "light_cube.fs");

    // setup vertex data (and buffer)
    // =============================
    float vertices[] = {
            // positions          // normals           // texture coords
          -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
           0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
           0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
           0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
          -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
          -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

          -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
           0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
           0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
           0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
          -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
          -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

          -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
          -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
          -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
          -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
          -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
          -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

           0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
           0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
           0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
           0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
           0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
           0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

          -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
           0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
           0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
           0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
          -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
          -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

          -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
           0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
           0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
           0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
          -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
          -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    // positions all containers
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
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

    // positions of the point lights
    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f,  0.2f,  2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3(0.0f,  0.0f, -3.0f)
    };
    glm::vec3 pointLightColors[] = {
        glm::vec3(1.0f, 0.6f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.0, 0.0),
        glm::vec3(0.2f, 0.2f, 1.0f)
    };

    GLuint cubeVAO, VBO;
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
    glEnableVertexAttribArray(0);
    // normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // load textures (we now use a utility function to keep the code more organized)
    // -----------------------------------------------------------------------------
    unsigned int diffuseMap = loadTexture(FileSystem::getResPath("/textures/container2.png").c_str());
    unsigned int specularMap = loadTexture(FileSystem::getResPath("/textures/container2_specular.png").c_str());

    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0); // 设置0号纹理作为漫反射贴图
    lightingShader.setInt("material.specular", 1);
    // setup imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    /*

    因为OpenGL是一个三角形一个三角形地来绘制你的立方体的，所以即便之前那里有东西它也会覆盖之前的像素。因为这个原因，有些三角形会被绘制在其它三角形上面，虽然它们本不应该是被覆盖的。


    OpenGL存储它的所有深度信息于一个Z缓冲(Z-buffer)中，也被称为深度缓冲(Depth Buffer)。GLFW会自动为你生成这样一个缓冲（就像它也有一个颜色缓冲来存储输出图像的颜色）。深度值存储在每个片段里面（作为片段的z值），当片段想要输出它的颜色时，OpenGL会将它的深度值和z缓冲进行比较，如果当前的片段在其它片段之后，它将会被丢弃，否则将会覆盖。这个过程称为深度测试(Depth Testing)，它是由OpenGL自动完成的。
    */
    glEnable(GL_DEPTH_TEST); // 开启深度测试

    ImVec4 clearColor = ImVec4(0.75f, 0.52f, 0.3f, 1.0f);

    glm::vec3 material_diffuse(1.0f, 0.5f, 0.31f);
    glm::vec3 material_specular(0.5f, 0.5f, 0.5f);
    float material_shininess = 32.0f;

    glm::vec3 light_ambient(0.2f, 0.2f, 0.2f);
    glm::vec3 light_diffuse(0.5f, 0.5f, 0.5f);
    glm::vec3 light_specular(1.0f, 1.0f, 1.0f);
    // lighting
    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);


    bool  lightMove    = false;

    unsigned int frame = 0;
    while (!glfwWindowShouldClose(window))
    {

        float currentTime = (float)glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
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
            ImGui::ColorEdit4("clearColor", (float*)& clearColor);
            ImGui::SliderFloat3("camera.m_position",    (float*)&camera.m_position, -99.0f, 99.0f);
            ImGui::SliderFloat("material_shininess",    (float*)&material_shininess, 0.0f, 1024.0f);


            ImGui::Checkbox("lightMove", &lightMove);


            ImGui::NewLine();

            ImGui::End();
        }

        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 因为开了深度缓冲


        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.m_position);
        lightingShader.setFloat("material.shininess", 32.0f);


        // Directional light
        glUniform3f(glGetUniformLocation(lightingShader.ID
            , "dirLight.direction"), -0.2f, -1.0f, -0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.ID, "dirLight.ambient"), 0.3f, 0.24f, 0.14f);
        glUniform3f(glGetUniformLocation(lightingShader.ID, "dirLight.diffuse"), 0.7f, 0.42f, 0.26f);
        glUniform3f(glGetUniformLocation(lightingShader.ID, "dirLight.specular"), 0.5f, 0.5f, 0.5f);
        // Point light 1
        glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
        glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[0].ambient"), pointLightColors[0].x * 0.1, pointLightColors[0].y * 0.1, pointLightColors[0].z * 0.1);
        glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[0].diffuse"), pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
        glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[0].specular"), pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
        glUniform1f(glGetUniformLocation(lightingShader.ID, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.ID, "pointLights[0].linear"), 0.09);
        glUniform1f(glGetUniformLocation(lightingShader.ID, "pointLights[0].quadratic"), 0.032);
        // Point light 2
        glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
        glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[1].ambient"), pointLightColors[1].x * 0.1, pointLightColors[1].y * 0.1, pointLightColors[1].z * 0.1);
        glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[1].diffuse"), pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
        glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[1].specular"), pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
        glUniform1f(glGetUniformLocation(lightingShader.ID, "pointLights[1].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.ID, "pointLights[1].linear"), 0.09);
        glUniform1f(glGetUniformLocation(lightingShader.ID, "pointLights[1].quadratic"), 0.032);
        // Point light 3
        glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
        glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[2].ambient"), pointLightColors[2].x * 0.1, pointLightColors[2].y * 0.1, pointLightColors[2].z * 0.1);
        glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[2].diffuse"), pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
        glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[2].specular"), pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
        glUniform1f(glGetUniformLocation(lightingShader.ID, "pointLights[2].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.ID, "pointLights[2].linear"), 0.09);
        glUniform1f(glGetUniformLocation(lightingShader.ID, "pointLights[2].quadratic"), 0.032);
        // Point light 4
        glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
        glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[3].ambient"), pointLightColors[3].x * 0.1, pointLightColors[3].y * 0.1, pointLightColors[3].z * 0.1);
        glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[3].diffuse"), pointLightColors[3].x, pointLightColors[3].y, pointLightColors[3].z);
        glUniform3f(glGetUniformLocation(lightingShader.ID, "pointLights[3].specular"), pointLightColors[3].x, pointLightColors[3].y, pointLightColors[3].z);
        glUniform1f(glGetUniformLocation(lightingShader.ID, "pointLights[3].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.ID, "pointLights[3].linear"), 0.09);
        glUniform1f(glGetUniformLocation(lightingShader.ID, "pointLights[3].quadratic"), 0.032);
        // SpotLight
        glUniform3f(glGetUniformLocation(lightingShader.ID, "spotLight.position"), camera.m_position.x, camera.m_position.y, camera.m_position.z);
        glUniform3f(glGetUniformLocation(lightingShader.ID, "spotLight.direction"), camera.m_front.x, camera.m_front.y, camera.m_front.z);
        glUniform3f(glGetUniformLocation(lightingShader.ID, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.ID, "spotLight.diffuse"), 0.8f, 0.8f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.ID, "spotLight.specular"), 0.8f, 0.8f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.ID, "spotLight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.ID, "spotLight.linear"), 0.09);
        glUniform1f(glGetUniformLocation(lightingShader.ID, "spotLight.quadratic"), 0.032);
        glUniform1f(glGetUniformLocation(lightingShader.ID, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
        glUniform1f(glGetUniformLocation(lightingShader.ID, "spotLight.outerCutOff"), glm::cos(glm::radians(13.0f)));

        glm::mat4 u_Model = glm::mat4(1.0);
        glm::mat4 u_View = glm::mat4(1.0);
        glm::mat4 u_Projection = glm::mat4(1.0);


        // world transformation
        lightingShader.setMat4("u_Model", u_Model);
        // u_View
        // camera/view transformation
        u_View = camera.getViewMatrix();
        lightingShader.setMat4("u_View", u_View);
        // u_Projection
        u_Projection = glm::perspective(glm::radians(camera.m_zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

        lightingShader.setMat4("u_Projection", u_Projection);


        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        // render the cube
        glBindVertexArray(cubeVAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 u_Model = glm::mat4(1.0f);
            u_Model = glm::translate(u_Model, cubePositions[i]);
            float angle = 20.0f * i;
            u_Model = glm::rotate(u_Model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            lightingShader.setMat4("u_Model", u_Model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }



        // render lamp object
        lightCubeShader.use();
        u_Model = glm::mat4(1.0f);
        //u_Model = glm::rotate(u_Model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
        if (lightMove)
        {
            lightPos.x = 2.0f * sin((float)glfwGetTime());
            lightPos.z = 2.0f * cos((float)glfwGetTime());
        }

        u_Model = glm::translate(u_Model, lightPos);
        u_Model = glm::scale(u_Model, glm::vec3(0.2f)); // a smaller cube
        lightCubeShader.setMat4("u_Model", u_Model);
        lightCubeShader.setMat4("u_View", u_View);
        lightCubeShader.setMat4("u_Projection", u_Projection);

        glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < 4; i++)
        {
            u_Model = glm::mat4(1.0f);
            u_Model = glm::translate(u_Model, pointLightPositions[i]);
            u_Model = glm::scale(u_Model, glm::vec3(0.2f)); // Make it a smaller cube
            lightCubeShader.setMat4("u_Model", u_Model);
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
    glDeleteShader(lightCubeShader.ID);
    glDeleteShader(lightingShader.ID);
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
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
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.processKeyBoard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.processKeyBoard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.processKeyBoard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.processKeyBoard(RIGHT, deltaTime);
    }
}



void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{

    float f_Xpos = static_cast<float>(xPos);
    float f_Ypos = static_cast<float>(yPos);

    if (firstMouse)
    {
        lastX = f_Xpos;
        lastY = f_Ypos;
        firstMouse = false;
    }
    float xoffset = f_Xpos - lastX;
    float yoffset = lastY - f_Ypos; // reversed since y-coordinates go from bottom to top


    lastX = f_Xpos;
    lastY = f_Ypos;
    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camera.processMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
    camera.processMouseScroll((float)yOffset);
}



// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
