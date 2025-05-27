#pragma once

#include "MyString.h"

class User {
private:
	MyString username;
	MyString password;
	//Add chats
public:
	User(MyString username = "", MyString password = "");
};