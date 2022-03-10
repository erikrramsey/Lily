#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

const float outlineWidth = 1.0 / 20.0;
void main()
{
    TexCoords = aTexCoords;
    FragPos = vec3(modelMatrix * vec4(aPos, 1.0));  
    Normal = mat3(transpose(inverse(modelMatrix))) * aNormal;
    vec3 newPos = aPos * (1 + outlineWidth);
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(newPos, 1.0);
}
