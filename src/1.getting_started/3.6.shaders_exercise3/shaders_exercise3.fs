#version 330 core
out vec4 FragColor;

in vec3 ourPosition;



void main()
{
    // 用位置来作为颜色
    FragColor = vec4(ourPosition, 1.0);
}
