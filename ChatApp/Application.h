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
	MyVector<User*> users;
	MyVector<Chat*> chats;
	User* loggedUser;

	FileHandler* fileHandler;
public:
	Application(const MyString& fileMode = "txt");
	~Application();
	void run();
};