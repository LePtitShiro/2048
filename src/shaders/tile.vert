#version 330 core
layout (location = 0) in vec2 aPos; // Position du sommet (0,0) à (1,1)

uniform mat4 model;       // Matrice pour déplacer/mettre à l'échelle la tuile
uniform mat4 projection;  // Matrice pour la caméra (gère les coordonnées)

void main()
{
    gl_Position = projection * model * vec4(aPos.x, aPos.y, 0.0, 1.0);
}