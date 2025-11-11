#version 330 core
out vec4 FragColor;

uniform vec3 tileColor; // Couleur envoyée depuis le C++

void main()
{
    FragColor = vec4(tileColor, 1.0);
}