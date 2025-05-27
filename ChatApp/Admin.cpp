#include "Admin.h"

Admin::Admin(MyString username, MyString password, int adminId) : User(username, password) {
	this->adminId = adminId;
}