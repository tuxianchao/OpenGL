#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture0; // 会使用默认的纹理单元0

void main()
{
	// FragColor = texture(texture1, TexCoord);
	FragColor = texture(texture0, TexCoord) * vec4(ourColor, 1.0);
}
