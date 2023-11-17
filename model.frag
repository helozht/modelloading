#version 330 core
in vec3 Pos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D texture_diffuse0;
uniform sampler2D texture_specular0;

struct DirectionalLight
{
    vec3 direction;
    vec3 color;
};

struct PointLight
{
    vec3 position;

    vec3 color;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight
{
    vec3 direction;
    vec3 position;

    vec3 color;

    float constant;
    float linear;
    float quadratic;

    float cutOff;
    float outerCutOff;
};

uniform vec3 viewPos;

uniform DirectionalLight sun;
uniform PointLight light;
uniform SpotLight torch;

vec3 CalcLight(vec3 direction, vec3 color);
float Attenuation(vec3 position, float constant, float linear, float quadratic);

void main()
{
    vec3 ambient = 0.1 * texture(texture_diffuse0, TexCoords).rgb;

    vec3 sunLight = CalcLight(-sun.direction, sun.color);
    vec3 lightLight = Attenuation(light.position, light.constant, light.linear, light.quadratic) * CalcLight(light.position - Pos, light.color);

    gl_FragColor = vec4(ambient + sunLight + lightLight, 1.0);
}

vec3 CalcLight(vec3 direction, vec3 color)
{
    vec3 lightDir = normalize(direction);
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - Pos);

    // diffuse
    float diff = max(dot(norm, lightDir), 0.0);

    // specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0);

    vec3 diffuse_result = color * diff * texture(texture_diffuse0, TexCoords).rgb;
    vec3 specular_result = color * spec * texture(texture_specular0, TexCoords).rgb;

    return (diffuse_result + specular_result);
}

float Attenuation(vec3 position, float constant, float linear, float quadratic)
{
    float distance = length(position - Pos);
    return 1.0 / (constant + linear * distance +
        quadratic * (distance * distance));
}
