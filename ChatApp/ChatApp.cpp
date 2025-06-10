#include "Application.h"
#include <stdexcept>

int main()
{
	try
	{
		Application app("bin"); //choose "txt" or "bin"
		app.run();
	}
	catch (const std::invalid_argument& e) {
		cout << e.what();
	}
	catch (const std::runtime_error& e) {
		cout << e.what();
	}
	catch (const std::exception& e) {
		cout << e.what();
	}
}