#include "Core/Window.hpp"
#include "Api/Nora.hpp"

int main() {
	try {
		py::scoped_interpreter guard{};
		py::module_ sys = py::module_::import("sys");
		sys.attr("stderr") = sys.attr("stdout");
		Window::GetInstance().Run();
	}
	catch (const std::exception& e) {
		std::cerr << "Unhandled exception: " << e.what() << std::endl;
		return -1;
	}

	return 0;
}
