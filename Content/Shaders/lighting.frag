#version 450 core

out vec4 FragColor;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
  
 struct Material {
     vec3 ambient;
     vec3 diffuse;
     vec3 specular;
     float shininess;
 };
  
uniform Material material;
uniform Light light;
uniform vec3 viewPosition;

in vec3 FragPos;
in vec3 Normal;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    
    float ambientStrength = 0.1;
    vec3 ambient = light.ambient * material.ambient;
    
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    
    float specularStrength = 0.5;
    
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  
   
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}