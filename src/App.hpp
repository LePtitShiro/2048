#pragma once
#include "Renderer.hpp"
#include "Game.hpp"


class App {


public:
    App(int width, int height);
    ~App();
    int run();
private:
    int width,height;
    GLFWwindow* window = nullptr;
    Renderer renderer;
    Game *game;
    void onKey(int key, int action);
    void update();
};
