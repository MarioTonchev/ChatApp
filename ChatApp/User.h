#pragma once

#include "MyString.h"

class User {
private:
	MyString username;
	MyString password;
	//Add chats
public:
	User(const MyString& username = "", const MyString& password = "");
};