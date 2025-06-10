#pragma once

#include "User.h"
#include "UserUtils.h"

class Admin : public User {
private:
	int adminId;

public:
	static int createAdminId(MyVector<User*>& users);
	Admin(int adminId, const MyString& username = "", const MyString& password = "", FileHandler* fileHandler = nullptr);
	int getAdminId() const;
	void deleteUser(const MyString& username, MyVector<User*>& users, MyVector<Chat*>& chats);
	void deleteGroupChat(int chatId, MyVector<Chat*>& chats);
	void viewAllChats(MyVector<Chat*>& chats) const;
	void printActions() const override;
};