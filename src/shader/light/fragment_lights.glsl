#version 330 core
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
    float emission_strength;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    //衰减常数
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Camera {
    vec3 position;
    vec3 direction;

    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirLight dirLight;
uniform Material material;
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform Camera camera;

vec3 GetTexture_vec3(sampler2D num, vec2 coords);
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight pointLight, vec3 normal, vec3 fragPost, vec3 viewDir);

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(camera.position - FragPos);
    vec3 result = CalcDirLight(dirLight, norm, viewDir);

    for (int i = 0; i < NR_POINT_LIGHTS; i++) {
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }

    if (GetTexture_vec3(material.specular, TexCoords).r == 0.0) {
        result += material.emission_strength * GetTexture_vec3(material.emission, TexCoords);
    }
    FragColor = vec4(result, 1.0);
}

vec3 GetTexture_vec3(sampler2D num, vec2 coords) {
    vec3 texture_color = vec3(texture(num, coords).rgb);
    return texture_color;
}

//定向光
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);

    //漫反射
    float diff = max(dot(lightDir, normal), 0.0);
    //镜面反射
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);

    vec3 ambient = light.ambient * GetTexture_vec3(material.diffuse, TexCoords);
    vec3 diffuse = diff * light.diffuse * GetTexture_vec3(material.diffuse, TexCoords);
    vec3 specular = spec * light.specular * GetTexture_vec3(material.specular, TexCoords);
    return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 refectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(refectDir, viewDir), 0.0), material.shininess);

    //衰减
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    vec3 ambient = light.ambient * GetTexture_vec3(material.diffuse, TexCoords);
    vec3 diffuse = diff * light.diffuse * GetTexture_vec3(material.diffuse, TexCoords);
    vec3 specular = spec * light.specular * GetTexture_vec3(material.specular, TexCoords);

    return attenuation * (ambient + diffuse + specular);
}
