#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform float u_time;       

uniform sampler2D texture_diffuse1;
uniform vec3 lightPos;

void main()
{    
    vec3 lightColor;
    float ambientStrength = 0.0f;
    lightColor = vec3(1.0f);
    vec3 ambient = ambientStrength * lightColor;

    vec3 newlightPos = vec3(sin(u_time) * 5, 0.0f,cos(u_time) * 5);
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(newlightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    vec4 result = vec4((ambient + diffuse),1);

    FragColor = result;
}