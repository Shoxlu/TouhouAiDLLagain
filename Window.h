#pragma once
#include <GLFW/glfw3.h>
#include "utils.h"

class Window {
public:
	Window();
	~Window();
	void draw_triangle(float base, float side1, float side2, Color color);
	void draw_line(Pos origin, Pos end, Color color);
	void draw_square(Pos pos, float side, Color color);
	void draw_circle(Pos center, double radius, Color color);
	//void drawSequence(List* vertices);
	void update();
	GLFWwindow* window;
	int32_t width;
	int32_t height;
private:
	
};