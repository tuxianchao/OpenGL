#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 v_TexCoords;

uniform mat4 u_Projection;
uniform mat4 u_View;

void main()
{
    v_TexCoords = aPos;
    vec4 pos = u_Projection * u_View * vec4(aPos, 1.0);
    gl_Position = pos.xyww; // 优化 最终的标准化设备坐标将永远会有一个等于1.0的z值：最大的深度值。结果就是天空盒只会在没有可见物体的地方渲染了（只有这样才能通过深度测试，其它所有的东西都在天空盒前面）
}  
