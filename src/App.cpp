//
// Created by Lucas on 01/10/2025.
//
#include "App.hpp"
#include <iostream>


App::App(int width, int height) : renderer(width, height) {
    this->width = width;
    this->height = height;
    this->game = new Game();
}

App::~App() {delete game;}

int App::run() {
    if (!glfwInit()) {
        std::cerr << "failed to init glfw" << std::endl;
        return -1;
    }
    this->window = glfwCreateWindow(width, height, "2048", nullptr, nullptr);
    if (!this->window) {
        std::cerr << "failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    if (!renderer.init()) {
        std::cerr << "failed to init renderer" << std::endl;
        glfwTerminate();
        return -1;
    }


    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (App* app = static_cast<App*>(glfwGetWindowUserPointer(window))) {
            app->onKey(key, action);
        }
    });

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        update();
        renderer.render(game->getBoard(), game->getScore(), game->getBestScore());
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;

}
//TODO: implémentation de la classe Renderer ET Game




void App::onKey(int key, int action) {
    if (action != GLFW_PRESS) return;
    switch (key) {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(this->window, GLFW_TRUE);
            break;
        case GLFW_KEY_LEFT:
            game->move(Direction::LEFT);
            break;
        case GLFW_KEY_RIGHT:
            game->move(Direction::RIGHT);
            break;
        case GLFW_KEY_UP:
            game->move(Direction::UP);
            break;
        case GLFW_KEY_DOWN:
            game->move(Direction::DOWN);
            break;
        default:
            break;
    }
}
    void App::update(){

    }

