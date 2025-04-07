#include "PerspectiveCamera.h"

void PerspectiveCamera::Inputs(GLFWwindow* window) {

	//Raw Mouse movement data.
	double Xinput, Yinput;

	//Screen sizes.
	int width, height;

	//Data you can use from raw mouse movement.
	float Xmod, Ymod;

	glfwGetCursorPos(window, &Xinput, &Yinput);

	glfwGetWindowSize(window, &width, &height);

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {

		std::cout << "Mouse input sucessful, position at "<<Xinput << " "<< Yinput << std::endl;

		Xmod = (Xinput - (width / 2)) / width;
		Ymod = (Yinput - (height / 2)) / height;

		yaw += Ymod;

		position.x += Xmod;

	}

}