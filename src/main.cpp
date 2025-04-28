#include "Window.hpp"
#include "Nora.hpp"

int main() {
	try {
		py::scoped_interpreter guard{};

		Window window;
		window.Run();
	}
	catch (const std::exception& e) {
		std::cerr << "Unhandled exception: " << e.what() << std::endl;
		return -1;
	}

	return 0;
}
