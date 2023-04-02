#pragma once
#include <GLFW/glfw3.h>
#include "utils.h"

struct Color {
	float r;
	float g;
	float b;
	float a;
};
class Window {
public:
	Window();
	~Window();
	void draw_triangle(float base, float side1, float side2, Color color);
	void draw_line(Pos origin, Pos end);
	void draw_square(float side);
	void draw_circle(float radius, Color color);
	void update();
private:
	GLFWwindow* window;

};