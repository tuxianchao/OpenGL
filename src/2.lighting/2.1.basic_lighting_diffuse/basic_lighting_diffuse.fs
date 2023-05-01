#version 330 core
out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

in vec3 v_Normal;
in vec3 v_FragPos;
void main()
{


    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;


    // 计算光源和片段位置之间的方向向量
    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(lightPos - v_FragPos);

    // 对norm和lightDir向量进行点乘，计算光源对当前片段实际的漫反射影响。
    // 结果值再乘以光的颜色，得到漫反射分量。两个向量之间的角度越大，漫反射分量就会越小
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;

    FragColor = vec4(result, 1.0);
}
