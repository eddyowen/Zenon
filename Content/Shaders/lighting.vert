#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;

void main()
{
    mat4 modelView = view * model;
    FragPos = vec3(modelView * vec4(aPos, 1.0));
    Normal = normalize(mat3(modelView) * aNormal);
    
    gl_Position = projection * vec4(FragPos, 1.0);
}