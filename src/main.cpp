#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <pybind11/embed.h>
#include <iostream>
#include <string>

namespace py = pybind11;

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

int main() {
	// Start the Python interpreter
	py::scoped_interpreter guard {};

	std::string title;
	try {
		// Add the project root directory to sys.path
		py::module_ sys = py::module_::import("sys");
		sys.attr("path").cast<py::list>().insert(0, py::str("../"));

		// Import the Python module
		py::module_ nora = py::module_::import("nora");

		py::object py_title = nora.attr("__title__");
		title = py_title.cast<std::string>();
	}
	catch (const py::error_already_set& e) {
		std::cerr << "Python error: " << e.what() << std::endl;
	}

	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, title.c_str(), NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	while (!glfwWindowShouldClose(window)) {
		// input
		processInput(window);

		// glfw: swap buffers and poll IO events (keys pressed / released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	// make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
