#ifndef INPUT_HPP
#define INPUT_HPP

#include "Camera/Camera.hpp"

struct GLFWwindow;

void initializeInput(Camera &camera);
void processInput(float deltaTime);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);

#endif