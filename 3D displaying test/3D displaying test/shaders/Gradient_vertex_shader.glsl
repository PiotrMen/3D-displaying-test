#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aColor;

out vec3 vertexColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraPos;
uniform float maxDistance;
uniform float gradientPow;

void main()
{
    vec4 worldPosition = model * vec4(aPos, 1.0);
    gl_Position = projection * view * worldPosition;
    
    // Przelicz odległość od kamery do pozycji w świecie
    float distance = length(cameraPos - vec3(worldPosition));
    
    // Znormalizuj odległość do zakresu [0, 1] na podstawie maxDistance
    float normalizedDistance = distance / maxDistance;

    // Upewnij się, że znormalizowana odległość jest w zakresie [0, 1]
    normalizedDistance = clamp(normalizedDistance, 0.0, 1.0);

    // Użyj funkcji pow, aby zwiększyć kontrast gradientu
    float contrast = pow(normalizedDistance, gradientPow); 

    // Ustaw kolor na podstawie znormalizowanej odległości i zwiększonego kontrastu
    vertexColor = vec3(1.0 - contrast, 0.0, contrast);
}
