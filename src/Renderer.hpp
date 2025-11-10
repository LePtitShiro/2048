//
// Created by Lucas on 01/10/2025.
//

#ifndef C_PROJETS_RENDERER_HPP
#define C_PROJETS_RENDERER_HPP
#include "Board.hpp"

class Renderer {
public:
    Renderer(int wight,int height);
    ~Renderer();
    void render(Board board, int score, int bestScore);
private:
    int width,height;
};


#endif //C_PROJETS_RENDERER_HPP