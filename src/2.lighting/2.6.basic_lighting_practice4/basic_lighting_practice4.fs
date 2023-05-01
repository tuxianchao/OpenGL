#version 330 core
out vec4 FragColor;
  
uniform vec3 objectColor;
in vec3 v_LightingColor;

void main()
{
    FragColor = vec4(v_LightingColor * objectColor, 1.0);
}
