#version 330 core

uniform sampler2D tex0;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float ambientStr;
uniform vec3 ambientColor;
uniform vec3 cameraPos;
uniform float specStr;
uniform float specPhong;
in vec2 TexCoord;
in vec3 normCoord;
in vec3 fragPos;
out vec4 FragColor;

void main() {


    // Normalize the normal vector
    vec3 normal = normalize(normCoord);
    
    // Calculate the direction from the fragment to the light source
    vec3 lightDir = normalize(lightPos - fragPos);

    // Diffuse shading: calculate the dot product of the normal and light direction
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Ambient shading: multiply the ambient strength by the ambient color
    vec3 ambientCol = ambientStr * ambientColor;

    // Specular shading: calculate the view direction and reflection direction
    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), specPhong);
    vec3 specColor = spec * specStr * lightColor;

    // Attenuation: calculate the distance from the light source to the fragment
    float distance = length(lightPos - fragPos);
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

    // Combine the results: add ambient, diffuse, and specular components, then apply attenuation
    vec3 result = (ambientCol + diffuse + specColor) * attenuation;
    result = clamp(result, 0.0, 1.0); // Ensure the result is within the valid range

    // Output the final color, combining the lighting result with the texture color
    FragColor = vec4(result, 1.0) * texture(tex0, TexCoord);
}
