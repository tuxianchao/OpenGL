#version 330 core
out vec4 FragColor;

in vec3 v_TexCoords;

uniform samplerCube u_Skybox; // 立方体贴图的纹理采样器

void main()
{    
    FragColor = texture(u_Skybox, v_TexCoords);
}
