#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture0; // 会使用默认的纹理单元0
uniform sampler2D texture1; // 会使用默认的纹理单元0
uniform float u_MixAlpha;

void main()
{
     // linearly interpolate between both textures (80% container, 20% awesomeface)
	FragColor = mix(texture(texture0, TexCoord), texture(texture1, TexCoord), u_MixAlpha);
}

