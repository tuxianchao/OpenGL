#version 330 core
out vec4 FragColor;

in vec2 v_TexCoord;

// texture sampler
uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
     // linearly interpolate between both textures (80% container, 20% awesomeface)
	FragColor = mix(texture(texture0, v_TexCoord), texture(texture1, v_TexCoord), 0.2);
}

