//
// Created by Lucas on 01/10/2025.
//

#include "Renderer.hpp"

#include <cstdio>
#include <GLFW/glfw3.h>
#include "stb_easy_font.h"


Renderer::Renderer(int width, int height) {
    this->width = width;
    this->height = height;
}

Renderer::~Renderer() {
    delete this;
}

void Renderer::render(Board board, int score, int bestScore) {
    int rows = board.getRows();
    int cols = board.getCols();
    int cellHeight = this->height / rows;
    int cellWidth = this->width / cols;
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i<rows;i++) {
        for (int j = 0; j < cols; j++) {
            int value = board.getCell(i,j);
            glColor3f(1,1,1); // on met de couleur blanc pour l'instant
            float x0 = static_cast<float>(j * cellWidth) / static_cast<float>(width) * 2 - 1;
            float y0 = static_cast<float>(i * cellHeight) / static_cast<float>(height) * 2 - 1;
            float x1 = static_cast<float>((j + 1) * cellWidth) / static_cast<float>(width) * 2 - 1;
            float y1 = static_cast<float>((i + 1) * cellHeight) / static_cast<float>(height) * 2 - 1;

            glBegin(GL_QUADS);
            glVertex2f(x0, y0);
            glVertex2f(x1, y0);
            glVertex2f(x1, y1);
            glVertex2f(x0, y1);
            glEnd();

            // char text[8];
            // snprintf(text,sizeof(text), "%d", value);
            // static char buffer[200];
            // int num_quads;
            //
            // //pos du texte (centre)
            // float text_x = (x0 + x1) / 2.0f;
            // float text_y = (y0 + y1) / 2.0f;
            //
            // //on doit convertir les coordonnées de opengl (-1,1) en pixels
            // float px = (text_x + 1) * static_cast<float>(width) / 2.0f;
            // float py = (text_y + 1) * static_cast<float>(height) / 2.0f;
            // px -= stb_easy_font_width(text) * 0.5f;
            // py -= stb_easy_font_height(text) * 0.5f;
            // // Conversion pixels -> coordonnées OpenGL
            // float gl_px = px / width * 2.0f - 1.0f;
            // float gl_py = py / height * 2.0f - 1.0f;
            //
            // num_quads = stb_easy_font_print(
            //     gl_px, gl_py, text, nullptr, buffer, sizeof(buffer)
            // );
            //
            // glColor3f(0, 0, 0); // noir
            // glEnableClientState(GL_VERTEX_ARRAY);
            // glVertexPointer(2, GL_FLOAT, 16, buffer);
            // glDrawArrays(GL_QUADS, 0, num_quads * 4);
            // glDisableClientState(GL_VERTEX_ARRAY);

        }
    }
}
