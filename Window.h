
#include <GLFW/glfw3.h>
#include "utils.h"
struct Pos;


class Window {
public:
	Window();
	~Window();
	void draw_triangle(float base, float side1, float side2, Color color);
	void draw_line(Pos origin, Pos end);
	void draw_square(float side, Color color);
	void draw_circle(Pos center, float radius, Color color);
	void update();
private:
	GLFWwindow* window;
};