#include "GLFW/glfw3.h"
#include "GL/glut.h"
#include <cstdlib>
#include <cstdio>
#include <memory>
#include <iostream>
#include <cmath>
#include <utility>
#include <vector>
#include <map>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

bool isDay = true;

struct ColorStruct {
    std::vector<int> light;
    std::vector<int> dark;

    ColorStruct( std::vector<int> light,  std::vector<int> dark) {
        this->light = std::move(light);
        this->dark = std::move(dark);
    }
};

std::vector<GLfloat> vertices;

std::map<std::string, ColorStruct> colors = {
        {"sky", ColorStruct({176, 224, 230}, {47, 79, 79})},
        {"sky_object", ColorStruct({255, 215, 0}, {211, 211, 211})},
        {"corpus", ColorStruct{{255, 222, 173}, {222, 184, 135}}},
        {"roof", ColorStruct{{160, 82, 45}, {139, 69, 19}}},
        {"roofDecor", ColorStruct{{233, 150, 122}, {255, 160, 122}}},
        {"window", ColorStruct{{245, 245, 245}, {255, 215, 0}}},
        {"windowDecor", ColorStruct{{160, 82, 45}, {139, 69, 19}}},
        {"door", ColorStruct{{160, 82, 45}, {139, 69, 19}}},
        {"petals", ColorStruct{{255, 255, 255}, {245, 245, 220}}},
        {"grass", ColorStruct{{107, 142, 35}, {85, 107, 47}}},
        {"flowerCentre", ColorStruct({{255, 215, 0}, {255, 215, 0}})}
};

void setColor(const std::string& key) {
    if (isDay) {
        glColor3ub(colors.at(key).light[0], colors.at(key).light[1], colors.at(key).light[2]);
    }
    else {
        glColor3ub(colors.at(key).dark[0], colors.at(key).dark[1], colors.at(key).dark[2]);
    }
}

std::vector<std::pair<float, float>> flowersCoord = {
        std::pair(0, -0.5),
        std::pair(-0.5, -0.35),
        std::pair(-0.75, -0.80),
        std::pair(0.6, -0.40),
};

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

void addEllipse(float xCenter, float yCenter,
                float rx, float ry, int pointCount = 360) {

    const float step = float(2 * M_PI) / pointCount;

    vertices.push_back(xCenter);
    vertices.push_back(yCenter);

    for (float angle = 0; angle <= float(2 * M_PI); angle += step)
    {
        float a = (fabsf(angle - float(2 * M_PI)) < 0.00001f) ? 0.f : angle;
        const float dx = rx * cosf(a);
        const float dy = ry * sinf(a);

        vertices.push_back(dx + xCenter);
        vertices.push_back(dy + yCenter);
    }
}

void drawEllipse(float xCenter, float yCenter,
                 float rx, float ry, int pointCount = 360) {
    addEllipse(xCenter, yCenter, rx, ry);
    glVertexPointer(2, GL_FLOAT, 0, vertices.data());
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size() / 2);
    vertices.clear();
}

/// Цветочек
void drawFlower(float x, float y) {
    float koef = 0.15;

    glTranslated(x + 0.15 * koef, y, 0);
    glRotatef( -1 * x * 7, 0, 0, 1);
    setColor("petals");
    addEllipse(-0.15f * koef, 0, 0.2f * koef, 0.1f * koef);
    addEllipse(0, 0.05f * koef, 0.15f * koef, 0.12f * koef);
    addEllipse(0.15f * koef, 0 * koef, 0.2f * koef, 0.1f * koef);
    addEllipse(-0.09f * koef, -0.1f * koef, 0.15f * koef, 0.1f * koef);
    addEllipse(0.1f * koef, -0.1f * koef, 0.15f * koef, 0.1f * koef);
    glVertexPointer(2, GL_FLOAT, 0, vertices.data());
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size() / 2);
    vertices.clear();
    setColor("flowerCentre");
    addEllipse(0, 0, 0.12f * koef, 0.08f * koef);
    glVertexPointer(2, GL_FLOAT, 0, vertices.data());
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size() / 2);
    glLoadIdentity();
    vertices.clear();
}

/// Окно
void drawWindow(float x1, float y1, float x2, float y2) {
    glLineWidth(2.5);
    setColor("window");
    glRectf(x1, y1, x2, y2);
    setColor("windowDecor");
    vertices.insert(vertices.end(),
                    { x1, y1,
                      x1, y2,
                      x2, y2,
                      x2, y1});
    auto midx = (x2 + x1) / 2;
    auto midy = (y2 + y1) / 2;
    vertices.insert(vertices.end(),
                    {
        midx, y1,
        midx, y2,
        x1, midy,
        x2, midy
                    });
    glVertexPointer(2, GL_FLOAT, 0, vertices.data());
    glDrawArrays(GL_LINE_LOOP, 0, 4);
    glDrawArrays(GL_LINES, 4, 4);
    vertices.clear();
}

/// Домик
void drawHouse(float x, float y) {
    glTranslatef(x, y, 0);

    // Корпус
    setColor("corpus");
    glRectf(0, 0, 0.5, 0.5);
    // Крыша
    setColor("roof");
    vertices.insert(vertices.end(),
                    {
        // Крыша
        -0.05, 0.45,
        0.12, 0.65,
        0.38, 0.65,
        0.55, 0.45,

        // Дверь
        .2, 0,
        0.2, 0.2,
        0.25, 0.25,
        0.3, 0.2,
        0.3, 0,

        // Украшение крыши
        0.035, 0.55,
        0.465, 0.55
                    });

    glVertexPointer(2, GL_FLOAT, 0, vertices.data());
    glDrawArrays(GL_POLYGON, 0, 4);
    setColor("door");
    glDrawArrays(GL_POLYGON, 4, 5);
    glLineWidth(2.f);
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(2,0X11FF);
    setColor("roofDecor");
    glDrawArrays(GL_LINES, 9, 2);
    glDisable(GL_LINE_STIPPLE);
    vertices.clear();

    // Окна
    drawWindow(0.05, 0.3, 0.15, 0.4);
    drawWindow(0.35, 0.3, 0.45, 0.4);
    drawWindow(0.2, 0.3, 0.3, 0.4);
    drawWindow(0.35, 0.1, 0.45, 0.2);
    drawWindow(0.05, 0.1, 0.15, 0.2);
    drawWindow(0.22, 0.12, 0.28, 0.18);
    glLoadIdentity();
}

int main()
{
    GLFWwindow* window;
    // стандартный обработчик ошибок
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Лабораторная 2", nullptr, nullptr);

    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // переключаем текущий контекст на это окно
    glfwMakeContextCurrent(window);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D (0.0, (GLdouble) WINDOW_WIDTH, 0.0, (GLdouble) WINDOW_HEIGHT);
    glEnableClientState(GL_VERTEX_ARRAY);
    GLfloat j = 340;
    // работаем, пока не закроется окно
    while (!glfwWindowShouldClose(window))
    {
        glEnableClientState(GL_VERTEX_ARRAY);
        // Фон
        if (isDay)
            glClearColor(GLfloat(colors.at("sky").light[0]) / 255,
                         GLfloat(colors.at("sky").light[1]) / 255,
                         GLfloat(colors.at("sky").light[2]) / 255, 1);
        else glClearColor(GLfloat(colors.at("sky").dark[0]) / 255,
                     GLfloat(colors.at("sky").dark[1]) / 255,
                     GLfloat(colors.at("sky").dark[2]) / 255, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        // Солнце
        j += 0.8;
        if (j >= 380) {
            j = 340;
            isDay = !isDay;
        }
        setColor("sky_object");
        glTranslatef(0, -2.75, 0);
        glRotatef(-j, 0, 0, 1);
        drawEllipse(0, 3.4, 0.15, 0.15);
        glLoadIdentity();
        // Домик
        drawHouse(0, -0.23);
        // Трава
        setColor("grass");
        drawEllipse(0, -0.7, 1.8, 0.5);
        // Цветочки
        for (auto&& coord: flowersCoord) {
            drawFlower(coord.first, coord.second);
        }
        _sleep(100);
        glFlush();
        //выводим отрисованную (измененную картинку)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}