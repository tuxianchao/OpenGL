#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 0) in vec3 aNormal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

out vec3 v_LightingColor;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * vec4(aPos, 1.0);


    // gouraud shading
    // ------------------------
    vec3 Position = vec3(u_Model * vec4(aPos, 1.0));
    vec3 Normal = mat3(transpose(inverse(u_Model))) * aNormal;


    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - Position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    float specularStrength = 1.0; // this is set higher to better show the effect of Gouraud shading 
    vec3 viewDir = normalize(viewPos - Position);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;      

    v_LightingColor = ambient + diffuse + specular;
}
