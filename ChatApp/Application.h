#pragma once

#include <iostream>
#include <stdexcept>

#include "MyVector.hpp"
#include "MyString.h"
#include "User.h"
#include "UserUtils.h"
#include "HelperFunctions.h"
#include "FileHandler.h"
#include "TextFileHandler.h"
#include "BinaryFileHandler.h"

using namespace std;

class Application {
private:
	FileHandler* fileHandler;
public:
	Application(const MyString& fileMode = "txt");
	void run();
};