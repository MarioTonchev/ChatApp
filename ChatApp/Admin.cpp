#include "Admin.h"

Admin::Admin(int adminId, const MyString& username, const MyString& password) : User(username, password) {
	this->adminId = adminId;
}

int Admin::getAdminId() const {
	return adminId;
}