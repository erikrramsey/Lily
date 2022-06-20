#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform float u_time;       

uniform sampler2D texture_diffuse1;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    vec3 lightColor;
    lightColor = vec3(1.0f);

    // ambient
    float ambientStrength = 0.2f;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float spec = max(dot(reflect(-lightDir, norm), viewDir), 0.0f);
    vec3 specular = pow(spec, 32) * lightColor;

    vec4 result = vec4((specular + diffuse + ambient), 1);
    float r = texture(texture_diffuse1, TexCoords).x;
    vec4 red = vec4(r, 0, 0, 1);
    FragColor = result * red;
}
