#version 330 core
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
out vec4 FragColor;

uniform vec3 viewPos; //摄像机位置

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
    float emission_strength;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;

void main()
{
    //环境光
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    //漫反射
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, TexCoords));

    //镜面反射
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spec * light.specular * vec3(texture(material.specular, TexCoords));

    // vec3 emission = texture(material.emission, TexCoords).rgb;
    vec3 emission = vec3(0.0);
    if (texture(material.specular, TexCoords).r == 0.0) /*rough check for blackbox inside spec texture */
    {
        /*apply emission texture */
        emission = texture(material.emission, TexCoords).rgb * material.emission_strength;

        // /*some extra fun stuff with "time uniform" */
        // emission = texture(material.emission, TexCoords + vec2(0.0, 0.5)).rgb; /*moving */
        // emission = emission * (sin(0.5) * 0.5 + 0.5) * 2.0; /*fading */
    }
    FragColor = vec4(ambient + diffuse + specular + emission, 1.0);
}
