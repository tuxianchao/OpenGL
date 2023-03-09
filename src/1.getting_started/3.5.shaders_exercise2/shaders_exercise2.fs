#version 330 core
out vec4 FragColor;

in vec3 v_Color;

uniform float u_ColorX;
uniform float u_ColorY;
uniform float u_ColorZ;
uniform float u_ColorW;

uniform bool u_enableColor;

void main()
{
    if (u_enableColor)
    {
        FragColor = vec4(u_ColorX, u_ColorY, u_ColorZ, u_ColorW);

    } else
    {
        FragColor = vec4(v_Color.x, v_Color.y, v_Color.z,  1.0f);
    }
}
