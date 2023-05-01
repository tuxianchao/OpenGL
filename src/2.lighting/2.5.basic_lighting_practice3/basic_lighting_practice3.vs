#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 0) in vec3 aNormal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform vec3 lightPos;

out vec3 v_Normal;
out vec3 v_FragPos;
out vec3 v_LightPos;
void main()
{
	gl_Position = u_Projection * u_View * u_Model * vec4(aPos, 1.0);
    v_FragPos = vec3(u_View * u_Model * vec4(aPos, 1.0));  // 在观察空间的片源位置
    v_Normal = mat3(transpose(inverse(u_View * u_Model))) * aNormal;;
    v_LightPos = vec3(u_View * vec4(lightPos, 1.0));
}
