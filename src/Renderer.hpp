//
// Created by Lucas on 01/10/2025.
//

#ifndef C_PROJETS_RENDERER_HPP
#define C_PROJETS_RENDERER_HPP
#include <string>
#include "stb_easy_font.h"
#include "Board.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Renderer {
public:
    Renderer(int width,int height);
    ~Renderer();
    void render(const Board& board, int score, int bestScore);
    bool init();
private:
    int width,height;
    GLuint VAO,VBO;
    GLuint shaderP;
    GLuint loadShaders(const char* vertex_path, const char* fragment_path);
    void renderText(const std::string& text, float x, float y, float scale, glm::vec3 color);
};


#endif //C_PROJETS_RENDERER_HPP