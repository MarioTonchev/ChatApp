#pragma once

#include "User.h"

class RegularUser : public User {
public:
	RegularUser(MyString username = "", MyString password = "");
};