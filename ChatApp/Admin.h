#pragma once
#include "User.h"

class Admin : public User {
private:
	int adminId;

public:
	Admin(int adminId, const MyString& username = "", const MyString& password = "");
	int getAdminId() const;
};