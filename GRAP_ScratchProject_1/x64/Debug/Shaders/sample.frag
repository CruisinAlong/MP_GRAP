#version 330 core

struct PointLight {
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 3
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform DirectionalLight dirLight;

uniform sampler2D tex0;
uniform vec3 cameraPos;
uniform float specStr;
uniform float specPhong;
uniform bool useDirectionalLight;
in vec2 TexCoord;
in vec3 normCoord;
in vec3 fragPos;
out vec4 FragColor;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), specPhong);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // combine results
    vec3 ambient = light.ambient * vec3(texture(tex0, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(tex0, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(tex0, TexCoord));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), specPhong);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(tex0, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(tex0, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(tex0, TexCoord));
    return (ambient + diffuse + specular);
}

void main() {
    // Normalize the normal vector
    vec3 normal = normalize(normCoord);
    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 result = vec3(0.0);

    if (useDirectionalLight) {
        // Directional light calculations
        result = CalcDirLight(dirLight, normal, viewDir);
    } else {
        // Point light calculations
        for (int i = 0; i < NR_POINT_LIGHTS; i++) {
            result += CalcPointLight(pointLights[i], normal, fragPos, viewDir);
        }
    }

    result = clamp(result, 0.0, 1.0); // Ensure the result is within the valid range

    // Output the final color, combining the lighting result with the texture color
    FragColor = vec4(result, 1.0) * texture(tex0, TexCoord);
}
