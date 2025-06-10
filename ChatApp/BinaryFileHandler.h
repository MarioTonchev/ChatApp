#pragma once

#include <iostream>
#include <fstream>
#include <stdexcept>

#include "FileHandler.h"
#include "Admin.h"

class BinaryFileHandler : public FileHandler {
private:
	void writeMyStringBin(std::ofstream& os, const MyString& str);
	MyString readMyStringBin(std::ifstream& is);
public:
	void saveUserToFile(RegularUser* user) override;
	void saveUserToFile(Admin* user) override;
	void loadUsers(MyVector<User*>& users) override;

	void saveChatToFile(IndividualChat* individualChat) override;
	void saveChatToFile(GroupChat* groupChat) override;
	void loadChats(MyVector<Chat*>& chats, MyVector<User*>& users) override;

	void saveChatIdToFile(Chat* chat) override;
	MyVector<int> getChatIds() override;

	void saveUserToChatApprovalList(const MyString& username, int chatId) override;
	void loadChatApprovalList(MyVector<Chat*>& chats, MyVector<User*>& users) override;

	void saveUserChatToFile(const MyString& username, int chatId) override;
	void loadUserChats(MyVector<User*>& users, MyVector<Chat*>& chats) override;

	void deleteChatIdFromFile(int chatId) override;
	void deleteChatFromFile(int chatId) override;
	void deleteUserFromFile(const MyString& username) override;
	void deleteUserChatRelation(const MyString& username, int chatId) override;
	void deleteUserFromApprovaList(const MyString& username, int chatId) override;
};