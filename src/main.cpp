#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "utils/render/ShadersUtils.h"
#include "drawable/House.h"
#include "utils/color/Colors.h"

using namespace glm;
using namespace std;

GLuint shaderProgramId;

vector<Drawable *> drawables;

GLFWwindow *initializeWindow() {
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(Constants::WIDTH, Constants::HEIGHT, "macOS OpenGL", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glewInit();
    glfwSwapInterval(1);

    return window;
}

void initializeShaders() {
    shaderProgramId = ShadersUtils::loadShaders("shaders/shader.vert", "shaders/shader.frag");
    Constants::MATRIX_LOCATION = glGetUniformLocation(shaderProgramId, "matrix");
}

void initializeScene() {
    House *houseA = new House(
            100.0f, 350.0f, 0.7f, false,
            Color::fromHex("#DEDCDA"), Color::fromHex("#E9E7E8"), Color::fromHex("#7C292E"),
            Color::fromHex("#BA4C40"), Color::fromHex("#458592"), Color::fromHex("#C5A671"),
            Color::fromHex("#966D44"), Color::fromHex("#EFCF9D")
    );
    drawables.push_back(houseA);

    House *houseB = new House(
            450.0f, 375.0f, 0.4f, true,
            Color::fromHex("#DAC09B"), Color::fromHex("#D3B589"), Color::fromHex("#41494E"), Color::fromHex("#59616C"),
            Color::fromHex("#3C8898"), Color::fromHex("#FBE6BE"), Color::fromHex("#B89A6B"), Color::fromHex("#B89A6B")
    );
    drawables.push_back(houseB);
}

void render() {
    glUseProgram(shaderProgramId);

    for (auto it: drawables) {
        it->render();
    }
}

void cleanUp() {
    glDeleteProgram(shaderProgramId);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
}

int main() {
    GLFWwindow *window = initializeWindow();
    initializeShaders();
    initializeScene();

    while (!glfwWindowShouldClose(window)) {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        glViewport(0, 0, width, height);
        glClearColor(Colors::SKY.getR(), Colors::SKY.getG(), Colors::SKY.getB(), Colors::SKY.getA());
        glClear(GL_COLOR_BUFFER_BIT);

        render();

        glFlush();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    cleanUp();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}