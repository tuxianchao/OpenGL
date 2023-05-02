#version 330 core
out vec4 FragColor;

in vec2 v_TexCoords;

uniform sampler2D u_Texture1;

void main()
{
    vec4 texColor = texture(u_Texture1, v_TexCoords);
    FragColor = texColor;
}
