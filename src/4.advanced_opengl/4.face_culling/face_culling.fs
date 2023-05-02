#version 330 core
out vec4 FragColor;

in vec2 v_TexCoords;

uniform sampler2D texture1;

void main()
{
    vec4 texColor = texture(texture1, v_TexCoords);
    FragColor = texColor;
}
