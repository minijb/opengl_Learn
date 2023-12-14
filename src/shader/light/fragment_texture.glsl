#version 330 core
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
out vec4 FragColor;

uniform vec3 viewPos; //摄像机位置

struct Material {
    vec3 direction;

    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
    float emission_strength;
};

struct Light {
    vec3 direction;
    vec3 position;
    float cutOff;
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

void main()
{
    vec3 lightDir = normalize(light.position - FragPos);
    vec3 color = vec3(0.0f);
    float theta = dot(lightDir, normalize(-light.direction));
    if (theta > light.outerCutOff) {
        //环境光
        vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
        //漫反射
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(light.position - FragPos);
        // vec3 lightDir = normalize(-light.direction);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, TexCoords));

        //镜面反射
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = spec * light.specular * vec3(texture(material.specular, TexCoords));

        float theta = dot(lightDir, normalize(-light.direction));
        float epsilon = light.cutOff - light.outerCutOff;
        float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
        diffuse *= intensity;
        specular *= intensity;

        // attenuation
        float distance = length(light.position - FragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;
        color = ambient + diffuse + specular;
    } else {
        color = light.ambient * texture(material.diffuse, TexCoords).rgb, 1.0;
    }

    vec3 emission = vec3(0.0);
    if (texture(material.specular, TexCoords).r == 0.0) /*rough check for blackbox inside spec texture */
    {
        /*apply emission texture */
        emission = texture(material.emission, TexCoords).rgb * material.emission_strength;
    }
    FragColor = vec4(color + emission, 1.0f);
}
