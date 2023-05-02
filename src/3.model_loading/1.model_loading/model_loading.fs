#version 330 core
out vec4 FragColor;

in vec2 v_TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{
    FragColor = texture(texture_diffuse1, v_TexCoords);
    // FragColor = vec4(1.0);
}
