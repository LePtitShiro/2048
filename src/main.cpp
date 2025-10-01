#include "GLFW/glfw3.h"
#include <ostream>
#include <iostream>



int main() {
    if (!glfwInit()) { // si glfw ne s'initialise pas bien -> on affiche un message d'erreur et on quitte
        std::cerr << "failed to init glfw" << std::endl;
        return -1;
    }

    GLFWwindow *window = glfwCreateWindow(800, 600, "2048", nullptr, nullptr);

    if (!window) { // si la fenetre ne s'initialise pas bien → on affiche un message d'erreur et on quitte
        std::cerr << "failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glfwSwapBuffers(window);

    }
}