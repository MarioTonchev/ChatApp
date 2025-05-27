#include "Admin.h"

Admin::Admin(const MyString& username, const MyString& password, int adminId) : User(username, password) {
	this->adminId = adminId;
}