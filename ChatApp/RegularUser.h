#pragma once

#include "User.h"

class RegularUser : public User {
public:
	RegularUser(const MyString& username = "", const MyString& password = "");
};