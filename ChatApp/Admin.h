#pragma once
#include "User.h"

class Admin : public User {
private:
	int adminId;

public:
	Admin(MyString username = "", MyString password = "", int adminId);
};