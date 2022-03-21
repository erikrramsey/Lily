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
    float ambientStrength = 0.2f;
    lightColor = vec3(1.0f);
    vec3 ambient = ambientStrength * lightColor;

    vec3 newlightPos = vec3(250, 250, 50);
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(newlightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    vec4 result = vec4((ambient + diffuse),1);
    vec4 tcolor = texture(texture_diffuse1, TexCoords);

    FragColor = result * texture(texture_diffuse1, TexCoords);
}