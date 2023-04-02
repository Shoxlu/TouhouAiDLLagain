#include "Window.h"
#include <stdio.h>


Window::Window()
{

    // Create a window
    window = glfwCreateWindow(640, 480, "th18 AI Visuals", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (window == NULL)
    {
        printf("????? %x", GetLastError());
    }

    // Set the clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    return;
}

Window::~Window()
{
    glfwTerminate();
}

void Window::draw_circle(float radius, Color color)
{

}

void Window::draw_line(Pos origin, Pos end)
{

}

void Window::draw_square(float side)
{

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
    glClear(GL_COLOR_BUFFER_BIT);

    // Swap buffers
    glfwSwapBuffers(window);

    // Poll events
    glfwPollEvents();
}