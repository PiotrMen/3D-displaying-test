#version 330 core
out vec4 FragColor;

in float distanceFromCamera;

void main()
{
    // Normalize the distance value for color calculation
    float normalizedDistance = distanceFromCamera / 500.0; // Assuming max distance is 1000
    normalizedDistance = clamp(normalizedDistance, 0.0, 1.0);

    // Gradient color based on distance
    vec3 color = mix(vec3(1.0, 0.0, 0.0), vec3(0.0, 0.0, 1.0), normalizedDistance);
    FragColor = vec4(color, 1.0);
}
