//
// Created by Lucas on 01/10/2025.
//
#include "App.hpp"
#include <iostream>
#include <GLFW/glfw3.h>
#include "Game.hpp"
#include "Board.hpp"
#include "Renderer.hpp"

class App {
public:
    App(int width, int height) {
        this->width = width;
        this->height = height;
        this->renderer = Renderer(width, height);
    }
    ~App();
    int run() {
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

        glfwSetKeyCallback(window, GLFWkeyfun(this));
        glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            static_cast<App*>(glfwGetWindowUserPointer(window));
        });

        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
            update();
            renderer.render(game.getBoard(),game.getScore(),game.getBestScore());
            glfwSwapBuffers(window);
        }
        glfwTerminate();
        return 0;

    }
//TODO: implémentation de la classe Renderer ET Game
private:
    int width,height;
    GLFWwindow* window = nullptr;
    Renderer* renderer;
    Game game;


    void onKey(int key, int action) {
        if (action != GLFW_PRESS) return;
        switch (key) {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GLFW_TRUE);
                break;
            case GLFW_KEY_LEFT:
                game.move(Direction::LEFT);
                break;
            case GLFW_KEY_RIGHT:
                game.move(Direction::RIGHT);
                break;
            case GLFW_KEY_UP:
                game.move(Direction::UP);
                break;
            case GLFW_KEY_DOWN:
                game.move(Direction::DOWN);
                break;
            default:
                break;
        }
    }
    void update() {

    }
};
