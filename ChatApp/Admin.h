#pragma once
#include "User.h"

class Admin : public User {
private:
	int adminId;

public:
	Admin(const MyString& username = "", const MyString& password = "", int adminId);
};