#version 330 core
out vec4 FragColor;

struct Material {
    vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position; // 点光源位置
    vec3 direction; // 点光源方向
    float cutOff; // 点光源切光角
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

in vec2 v_TexCoords;
in vec3 v_Normal;
in vec3 v_FragPos;

void main()
{

    // 环境光
    vec3 ambient    = light.ambient * texture(material.diffuse, v_TexCoords).rgb;

    // 漫反射
    vec3 norm       = normalize(v_Normal);
    vec3 lightDir   = normalize(light.position - v_FragPos);
    
    float diff      = max(dot(norm, lightDir), 0.0);
    vec3 diffuse    = light.diffuse * diff * texture(material.diffuse, v_TexCoords).rgb;

    // 镜面反射高光
    vec3 viewDir    = normalize(viewPos - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec      = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular   = light.specular * spec * texture(material.specular, v_TexCoords).rgb;


    // 聚光
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;

    // attenuation
    float distance    = length(light.position - v_FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);

}