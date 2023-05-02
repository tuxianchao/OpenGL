#version 330 core
out vec4 FragColor;

in vec3 v_Normal;
in vec3 v_Position;


uniform vec3 u_CameraPos;
uniform samplerCube u_Skybox;

// 反射
// void main()
// {
//   vec3 I = normalize(v_Position - u_CameraPos);
//   vec3 R = reflect(I, normalize(v_Normal));
//   FragColor = vec4(texture(u_Skybox, R).rgb, 1.0);
// }

void main()
{
    float ratio = 1.00 / 1.52;
    vec3 I = normalize(v_Position - u_CameraPos);
    vec3 R = refract(I, normalize(v_Normal), ratio);
    FragColor = vec4(texture(u_Skybox, R).rgb, 1.0);
}
