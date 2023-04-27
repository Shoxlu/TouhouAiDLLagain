#include "Window.h"
#include "utils.h"
#include <stdio.h>
#include <cmath>

Window::Window()
{

    // Create a window
    window = glfwCreateWindow(1280.0/1.5, 920.0/1.5, "AI Visual Tests", NULL, NULL);
    width = int(1280 / 1.5);
    height = int(920 / 1.5);
    glfwMakeContextCurrent(window);
    if (window == nullptr)
    {
        printf("????? %x", GetLastError());
    }
    else {
        printf("Window creation success \n");
    }

    // Set the clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

Window::~Window()
{
    glfwTerminate();
}

void Window::draw_circle(Pos center, float radius, Color color)
{
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(color.r, color.g, color.b);
    glVertex2f(center.x / width, center.y/ height); // Center of the circle
    for (int i = 0; i <= 180; ++i)
    {
        float angle = 6.83178 * (static_cast<float>(i) / 180);
        float x = radius * cos(angle) + center.x;
        float y = radius * sin(angle) + center.y;
        glVertex2f(x/ width, y / height);
    }
    glEnd();
}


void Window::draw_line(Pos origin, Pos end, Color color)
{
    glBegin(GL_LINES);
    glColor3f(color.r, color.b, color.g);
    glVertex2f(origin.x / width, origin.y / height);
    glVertex2f(end.x / width, end.y / height);
    glEnd();
}

void Window::draw_square(Pos pos,float side, Color color)
{
    glBegin(GL_QUADS);
    glColor3f(color.r, color.g, color.b);
    glVertex2f((pos.x - side)/ width, (pos.y - side) / height);
    glVertex2f((pos.x + side) / width, (pos.y - side) / height);
    glVertex2f((pos.x + side) / width, (pos.y + side) / height);
    glVertex2f((pos.x - side) / width, (pos.y + side) / height);
    glEnd();
}

void Window::draw_triangle(float base, float side1, float side2, Color color)
{
    glBegin(GL_TRIANGLES);
    glColor3f(color.r, color.b, color.g);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.5f, -0.5f);
    glVertex2f(0.0f, 0.5f);
    glEnd();
}

void Window::update()
{
    // Swap buffers
    glfwSwapBuffers(window);

    // Poll events
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT);
}