#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 0) in vec3 aNormal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 v_Normal;
out vec3 v_FragPos;// 世界空间的片元位置
void main()
{
	gl_Position = u_Projection * u_View * u_Model * vec4(aPos, 1.0);
    v_Normal = aNormal;
    v_FragPos = vec3(u_Model * vec4(aPos, 1.0)); // 通过顶点乘以模型矩阵得到世界空间的位置
}
