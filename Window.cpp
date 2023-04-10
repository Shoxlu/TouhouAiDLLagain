#include "Window.h"
#include "utils.h"
#include <stdio.h>
#include <cmath>

Window::Window()
{

    // Create a window
    window = glfwCreateWindow(640, 480, "th18 AI Visuals", NULL, NULL);
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

    return;
}

Window::~Window()
{
    glfwTerminate();
}

void Window::draw_circle(Pos center, float radius, Color color)
{
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(color.r, color.g, color.b);
    glVertex2f(center.x / 640, center.y/ 480); // Center of the circle
    for (int i = 0; i <= 360; ++i)
    {
        float angle = 2.0f * 3.141589 * (static_cast<float>(i) / 360);
        float x = radius * cos(angle) + center.x;
        float y = radius * sin(angle) + center.y;
        glVertex2f(x/640, y / 480);
    }
    glEnd();
}


void Window::draw_line(Pos origin, Pos end)
{
    glBegin(GL_LINES);
    glColor3f(1, 1, 1);
    glVertex2f(origin.x / 640, origin.y / 480);
    glVertex2f(end.x / 640, end.y / 480);
    glEnd();
}

void Window::draw_square(float side, Color color)
{
    glBegin(GL_QUADS);
    glColor3f(color.r, color.b, color.g);
    glVertex2f(-side/640, -side/480);
    glVertex2f(side / 640, -side / 480);
    glVertex2f(side / 640, side / 480);
    glVertex2f(-side / 640, side / 480);
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