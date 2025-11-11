//
// Created by Lucas on 01/10/2025.
//

#include "Renderer.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <ostream>
#include <vector>
#include "stb_easy_font.h"




Renderer::Renderer(int width, int height) {
    this->width = width;
    this->height = height;

    // if (glfwGetCurrentContext() == nullptr) {
    //     fprintf(stderr, "No OpenGL context found. Make sure to create a GLFW window before initializing the Renderer.\n");
    //     return;
    // }
    //
    // glewExperimental = GL_TRUE;
    // if (glewInit() != GLEW_OK) {fprintf(stderr, "Failed to initialize GLEW\n"); return;}
    // glGenVertexArrays(1, &VAO);
    // glBindVertexArray(VAO);
}

Renderer::~Renderer() {
    glDeleteVertexArrays(1, &VAO);
}

bool Renderer::init() {
    if (glfwGetCurrentContext() == nullptr) {
        fprintf(stderr, "No OpenGL context found. Make sure to create a GLFW window before initializing the Renderer.\n");
        return false;
    }

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {fprintf(stderr, "Failed to initialize GLEW\n"); return false;}
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glDisable(GL_DEPTH_TEST);

    shaderP = loadShaders("./shaders/tile.vert", "./shaders/tile.frag");
    if (shaderP == 0) {
        fprintf(stderr, "Failed to load shaders\n");
        return false;
    }

    // on définit un carre
    float vertices[] = {
        // pos
        0.0f, 1.0f, // HG
        1.0f, 0.0f, // BD
        0.0f, 0.0f, // BG

        0.0f, 1.0f, // HG
        1.0f, 1.0f, // HD
        1.0f, 0.0f  // BD
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);

    // partie texte




    return true;

}

void Renderer::render(const Board& board, int score, int bestScore) {
    int rows = board.getRows();
    int cols = board.getCols();
    int cellHeight = this->height / rows;
    int cellWidth = this->width / cols;
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderP);
    glBindVertexArray(VAO);
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shaderP, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    for (int i = 0; i<rows;i++) {
        for (int j = 0; j < cols; j++) {
            int value = board.getCell(i,j);
            glm::vec3 color;
            if (value == 0) color = glm::vec3(0.7f, 0.7f, 0.7f); // Gris
            else if (value == 2) color = glm::vec3(1.0f, 0.9f, 0.8f); // Beige clair
            else if (value == 4) color = glm::vec3(1.0f, 0.8f, 0.6f); // Orange clair
            else if (value == 8) color = glm::vec3(1.0f, 0.5f, 0.3f); // Orange
            else color = glm::vec3(1.0f, 0.2f, 0.2f); // Rouge

            float xPos = j * cellWidth;
            float yPos = i * cellHeight;

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(xPos, yPos, 0.0f));
            model = glm::scale(model, glm::vec3(cellWidth, cellHeight, 1.0f));
            glUniformMatrix4fv(glGetUniformLocation(shaderP, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniform3fv(glGetUniformLocation(shaderP, "tileColor"), 1, glm::value_ptr(color));
            glDrawArrays(GL_TRIANGLES, 0, 6);

        }
    }
    glBindVertexArray(0);
    glUseProgram(0);

    std::stringstream ss_score;
    ss_score << "Score: " << std::to_string(score);
    std::stringstream ss_best_score;
    ss_best_score << "Best Score: " << std::to_string(bestScore);

    glm::vec3 colortext = glm::vec3(0.2f, 0.2f, 0.2f);
    renderText(ss_score.str(), 20.0f, 20.0f, 4.0f, colortext);
    renderText(ss_best_score.str(), 200.0f, 20.0f, 4.0f, colortext);


    cellHeight = static_cast<float>(this->height) / board.getRows();
    cellWidth = static_cast<float>(this->width) / board.getCols();

    for (int i = 0; i < board.getRows(); i++) {
        for (int j = 0; j < board.getCols(); j++) {
            int value = board.getCell(i, j);
            if (value > 0) {
                std::string s_val = std::to_string(value);

                // Centrer le texte
                float textWidth = stb_easy_font_width((char*)s_val.c_str()) * 3.0f; // scale 3
                float textHeight = stb_easy_font_height((char*)s_val.c_str()) * 3.0f;

                float xPos = (j * cellWidth) + (cellWidth / 2.0f) - (textWidth / 2.0f);
                float yPos = (i * cellHeight) + (cellHeight / 2.0f) - (textHeight / 2.0f);

                renderText(s_val, xPos, yPos, 3.0f, colortext);
            }
        }
    }

}

GLuint Renderer::loadShaders(const char* vertex_path, const char* fragment_path) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    try {
        vShaderFile.open(vertex_path);
        fShaderFile.open(fragment_path);
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }catch (std::ifstream::failure& e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
        return 0;
    }




    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. Compiler les shaders
    GLuint vertex, fragment;
    int success;
    char infoLog[512];

    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        std::cerr << "ERREUR::SHADER::VERTEX::COMPILATION_ECHOUEE\n" << infoLog << std::endl;
    }

    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        std::cerr << "ERREUR::SHADER::FRAGMENT::COMPILATION_ECHOUEE\n" << infoLog << std::endl;
    }

    // Programme de shaders (lier les shaders)
    GLuint ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, nullptr, infoLog);
        std::cerr << "ERREUR::SHADER::PROGRAMME::LIAISON_ECHOUEE\n" << infoLog << std::endl;
    }

    // Supprimer les shaders car ils sont liés au programme
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return ID;
}


void Renderer::renderText(const std::string &text, float x, float y, float scale, glm::vec3 color) {

    char buffer[1024*16]; // ~500 chars
    int num_quads = stb_easy_font_print(0,0, const_cast<char*>(text.c_str()), nullptr, buffer, sizeof(buffer));
    glColor3f(color.r, color.g, color.b);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(x, y + stb_easy_font_height(const_cast<char *>(text.c_str())) * scale, 0.0f);
    glScalef(scale, scale, 1.0f);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 16, buffer);
    glDrawArrays(GL_QUADS, 0, num_quads * 4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

